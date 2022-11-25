#include "Bmp.h"

Bmp::Bmp(std::string file, int noThreads) {
	readFile(file, noThreads);
	this->noThreads = noThreads;
}

Bmp::~Bmp() {
	delete[] data;
}

void Bmp::readFile(std::string file, int noThreads) {
	//FILE* f;
	//fopen_s(&f, file.c_str(), "r");
	std::ifstream f;
	f.open(file.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "\nThe file cannot be opened.\n";
		return;
	}

	header = new unsigned char[BMP_File_Header];
	f.read(reinterpret_cast<char*>(header), BMP_File_Header);
	//fread(reinterpret_cast<char*>(header), sizeof(unsigned char), BMP_File_Header, f);
	//currentHeader = new BmpHeader(header);
	f.clear();
	f.seekg(0);
	f.read((char*)&bmfh, FileHeaderSize);
	f.read((char*)&bmih, InfoHeaderSize);

	const int paddingSize = (4 - (bmih.biWidth * 3) % 4) % 4;
	//dt.resize(bmih.biWidth * bmih.biHeight);
	unsigned char *rawData = new unsigned char[bmfh.bfSize - BMP_File_Header];
	//std::cout << bmfh.bfSize - BMP_File_Header << "  " << bmih.biWidth * bmih.biHeight * 3;
	data = new unsigned char[bmih.biWidth * bmih.biHeight * 3];
	f.read(reinterpret_cast<char*>(rawData), bmfh.bfSize - BMP_File_Header);
	//fread(data, sizeof(float), bmfh.bfSize - BMP_File_Header, f);
	for (int iRaw = 0, iFinal = 0; iFinal < bmih.biWidth * bmih.biHeight * 3; iFinal+=3, iRaw+=3) {
		if (iFinal != 0 && iFinal % bmih.biWidth == 0) {
			iRaw += paddingSize;
		}
		data[iFinal] = rawData[iRaw + 2];
		data[iFinal + 1] = rawData[iRaw + 1];
		data[iFinal + 2] = rawData[iRaw];
	}
	//for (int y = 0; y < bmih.biHeight; ++y) {
	//	for (int x = 0; x < bmih.biWidth; ++x) {
	//		unsigned char colour[3] = { 0,0,0 };
	//		f.read(reinterpret_cast<char*>(colour), 3);
	//		/*dt[y * bmih.biWidth + x].red = static_cast<float>(colour[2]) / 255.0f;
	//		dt[y * bmih.biWidth + x].green = static_cast<float>(colour[1]) / 255.0f;
	//		dt[y * bmih.biWidth + x].blue = static_cast<float>(colour[0]) / 255.0f;*/
	//		data[3 * y * bmih.biWidth + 3 * x] = static_cast<unsigned char>(colour[2]);
	//		data[3 * y * bmih.biWidth + 3 * x + 1] = static_cast<unsigned char>(colour[1]);
	//		data[3 * y * bmih.biWidth + 3 * x + 2] = static_cast<unsigned char>(colour[0]);
	//	}
	//	f.ignore(paddingSize);
	//}
	delete[] rawData;
}


void Bmp::filter(bool cpp) {
	std::vector<std::thread> vecOfThreads;
	unsigned char** modifiedData = new unsigned char*[noThreads];
	/*for (int i = 0; i < noThreads; ++i) {
		modifiedData[i] = new float [bmih.biHeight / noThreads];
	}*/
	//std::vector<std::future<float*>> vecOfThreads;
	/*for (int i = 0; i < 3 * bmih.biWidth * bmih.biHeight; ++i) {
		modifiedData[i] = 0;
	}*/

	HINSTANCE hinstLib = cpp ? LoadLibrary(TEXT("Dll1.dll")) : LoadLibrary(TEXT("JADll.dll"));
	unsigned char mask[] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };
	BOOL fFreeResult;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
	int position = 0;
	if (hinstLib) {
		if (cpp) {
			laplaceCpp laplace = (laplaceCpp)GetProcAddress(hinstLib, "laplaceFilter");
			if (laplace) {
				//laplace(bmih.biWidth, bmih.biHeight, noThreads, 5, data, std::ref(modifiedData), mask);
				start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < noThreads; ++i) {
					//laplace(bmih.biWidth, bmih.biHeight, noThreads, i, data, *modifiedData, mask);
					int noRows = bmih.biHeight - (noThreads * (bmih.biHeight / noThreads)) > i ? bmih.biHeight / noThreads + 1 : bmih.biHeight / noThreads;
					modifiedData[i] = new unsigned char[3 * noRows * bmih.biWidth];
					for (int k = 0; k < 3 * noRows * bmih.biWidth; ++k) {
						modifiedData[i][k] = 0;
					}
					//std::cout << "Thread number: " << i << ", position: " << position << std::endl;
					std::thread a(laplace, data, modifiedData[i], mask, bmih.biWidth, bmih.biHeight, noThreads, position);
					vecOfThreads.emplace_back(std::move(a));
					position += noRows;
					/*std::future<float*> a = std::async(laplace, bmih.biWidth, bmih.biHeight, noThreads, i, data, mask);
					vecOfThreads.emplace_back(std::move(a));*/
				}
			}
		}
		else {
			laplaceAsm laplace = (laplaceAsm)GetProcAddress(hinstLib, "laplaceFilter");
			if (laplace) {
				start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < noThreads; ++i) {
					//laplace(bmih.biWidth, bmih.biHeight, noThreads, i, data, *modifiedData, mask);
					int noRows = bmih.biHeight - (noThreads * (bmih.biHeight / noThreads)) > i ? bmih.biHeight / noThreads + 1 : bmih.biHeight / noThreads;
					modifiedData[i] = new unsigned char[3 * noRows * bmih.biWidth];
					for (int k = 0; k < 3 * noRows * bmih.biWidth; ++k) {
						modifiedData[i][k] = 0;
					}
					//std::cout << "Thread number: " << i << ", position: " << position << std::endl;
					std::thread a(laplace, data, modifiedData[i], mask, bmih.biWidth, bmih.biHeight, noThreads, position, noRows);
					vecOfThreads.emplace_back(std::move(a));
					position += noRows;
					/*std::future<float*> a = std::async(laplace, bmih.biWidth, bmih.biHeight, noThreads, i, data, mask);
					vecOfThreads.emplace_back(std::move(a));*/
				}
			}
		}
		for (int i = 0, pos = 0; i < noThreads; ++i) {
			//vecOfThreads.at(i).get();
			/*int tSize = sizeof(vecOfThreads.at(i).get());
			for (int k = 0; k < tSize; ++k, ++pos) {
				modifiedData[pos] = vecOfThreads.at(i).get()[k];
			}*/
			vecOfThreads.at(i).join();
		}
		end = std::chrono::high_resolution_clock::now();
		FreeLibrary(hinstLib);
	}
	std::chrono::duration<double> diff = end - start;
	std::cout<<"Time: "<<diff.count();
	unsigned char* finalData = new unsigned char[3 * bmih.biHeight * bmih.biWidth];
	//saveImage(*modifiedData, "result.bmp");
	for (int i = 0, pos = 0; i < noThreads; ++i) {
		int noRows = bmih.biHeight - (noThreads * (bmih.biHeight / noThreads)) > i ? bmih.biHeight / noThreads + 1 : bmih.biHeight / noThreads;
		//pos = pos + 3 * noRows * bmih.biWidth;
		//std::copy(std::begin(modifiedData[i]), std::end(modifiedData[i]), std::end(finalData));
		for (int k = 0; k < 3 * noRows * bmih.biWidth; ++k, ++pos) {
			finalData[pos] = modifiedData[i][k];
		}
		delete[] modifiedData[i];
	}
	saveImage(finalData, "result.bmp");
	delete[] modifiedData;
	delete[] finalData;
}

void Bmp::saveImage(unsigned char* modifiedData, const char* destinationFile) {
	std::ofstream file;
	file.open(destinationFile, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Pliku nie udalo sie zapisac.";
		return;
	}
	unsigned char padding[3] = { 0, 0, 0 };
	const int paddingSize = (4 - (bmih.biWidth * 3) % 4) % 4;
	//BITMAPFILEHEADER bmfht = bmfh;
	//BITMAPINFOHEADER bmiht = bmih;
	const int fileSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + paddingSize * bmih.biHeight;
	//dt.resize(bmih.biWidth * bmih.biHeight);
	unsigned char *finalData = new unsigned char[bmfh.bfSize - BMP_File_Header];
	//bmfht.bfSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + 3 * bmih.biHeight;
	header[2] = fileSize;
	header[3] = fileSize >> 8;
	header[4] = fileSize >> 16;
	header[5] = fileSize >> 24;
	//std::cout<<"curr: " << bmfh.bfSize - BMP_File_Header << ", new: "<<
	for (int iData = 0, iOut = 0; iData < 3 * bmih.biWidth * bmih.biHeight; iData += 3, iOut += 3) {
		if (iData != 0 && iData == 3 * (iData / 3) && iData / 3 % bmih.biWidth == 0) {
			for (int i = 0; i < paddingSize; ++i, ++iOut) {
				if (iOut >= 231560) { 
					std::cout << "l"; }
				finalData[iOut] = 0;
			}
		}
		else {
			finalData[iOut] = modifiedData[iData + 2];
			finalData[iOut + 1] = modifiedData[iData + 1];
			finalData[iOut + 2] = modifiedData[iData];
		}
	}
	//finalData[] 
	file.write(reinterpret_cast<char*>(header), BMP_File_Header);
	file.write(reinterpret_cast<char*>(finalData), bmfh.bfSize - BMP_File_Header);
	//for (int y = 0; y < bmih.biHeight; ++y) {
	//	for (int x = 0; x < bmih.biWidth; ++x) {
	//		//std::cout << data[3 * bmih.biWidth * y + 3 * x] << "  " << data[3 * bmih.biWidth * y + 3 * x+1] << "  " << data[3 * bmih.biWidth * y + 3 * x +2] << std::endl;
	//		setColour(Colour(modifiedData[3 * bmih.biWidth * y + 3 * x], modifiedData[3 * bmih.biWidth * y + 3 * x + 1], modifiedData[3 * bmih.biWidth * y + 3 * x + 2]), x, y);
	//		//setColour(Colour((float)x / (float)bmih.biWidth, 1.0f - ((float)x / (float)bmih.biWidth), (float)y / (float)bmih.biHeight), x, y);
	//	}
	//}
	//
	////setColour(Colour(0, 0, 55), 0, 100);
	//file.write(reinterpret_cast<char*>(header), BMP_File_Header);
	//for (int y=0; y < bmih.biHeight; ++y) {
	//	for (int x = 0; x < bmih.biWidth; ++x) {
	//		unsigned char r = static_cast<unsigned char>(getColour(x, y).red);
	//		unsigned char g = static_cast<unsigned char>(getColour(x, y).green);
	//		unsigned char b = static_cast<unsigned char>(getColour(x, y).blue);
	//		unsigned char c[] = {b, g, r};
	//		file.write(reinterpret_cast<char*>(c), 3);
	//	}
	//	file.write(reinterpret_cast<char*>(padding), paddingSize);
	//}
	file.close();
	delete[] finalData;
}

Colour Bmp::getColour(int x, int y) const
{
	return dt.at(y * bmih.biWidth + x);
}

void Bmp::setColour(const Colour& colour, int x, int y)
{
	dt.at(y * bmih.biWidth + x).red = colour.red;
	dt.at(y * bmih.biWidth + x).blue = colour.blue;
	dt.at(y * bmih.biWidth + x).green = colour.green;
}


