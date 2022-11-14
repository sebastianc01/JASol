#include "Bmp.h"

Bmp::Bmp(std::string file, int noThreads) {
	readFile(file, noThreads);
	this->noThreads = noThreads;
}

Bmp::~Bmp() {}

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
	
	bmfh.bfType = *(int*)&header[0];
	bmfh.bfSize = *(int*)&header[2];
	bmfh.bfReserved1 = *(int*)&header[6];
	bmfh.bfReserved2 = *(int*)&header[8];
	bmfh.bfOffBits = *(int*)&header[10];
	bmih.biSize = *(int*)&header[14];
	bmih.biWidth = *(int*)&header[18];
	bmih.biHeight = *(int*)&header[22];
	bmih.biPlanes = *(int*)&header[26];
	bmih.biBitCount = *(int*)&header[28];
	bmih.biCompression = *(int*)&header[30];
	bmih.biSizeImage = *(int*)&header[34];
	bmih.biXPelsPerMeter = *(int*)&header[38];
	bmih.biYPelsPerMeter = *(int*)&header[42];
	bmih.biClrUsed = *(int*)&header[46];
	bmih.biClrImportant = *(int*)&header[50];
	const int paddingSize = (4 - (bmih.biWidth * 3) % 4) % 4;
	//dt.resize(bmih.biWidth * bmih.biHeight);
	data = new float[bmfh.bfSize - BMP_File_Header];
	//fread(data, sizeof(float), bmfh.bfSize - BMP_File_Header, f);
	for (int y = 0; y < bmih.biHeight; ++y) {
		for (int x = 0; x < bmih.biWidth; ++x) {
			unsigned char colour[3] = { 0,0,0 };
			f.read(reinterpret_cast<char*>(colour), 3);
			/*dt[y * bmih.biWidth + x].red = static_cast<float>(colour[2]) / 255.0f;
			dt[y * bmih.biWidth + x].green = static_cast<float>(colour[1]) / 255.0f;
			dt[y * bmih.biWidth + x].blue = static_cast<float>(colour[0]) / 255.0f;*/
			data[3 * y * bmih.biWidth + 3 * x] = static_cast<float>(colour[2]) / 255.0f;
			data[3 * y * bmih.biWidth + 3 * x + 1] = static_cast<float>(colour[1]) / 255.0f;
			data[3 * y * bmih.biWidth + 3 * x + 2] = static_cast<float>(colour[0]) / 255.0f;
		}
		f.ignore(paddingSize);
	}
}


void Bmp::filter(bool cpp) {
	std::vector<std::thread> vecOfThreads;
	float** modifiedData = new float*[noThreads];
	/*for (int i = 0; i < noThreads; ++i) {
		modifiedData[i] = new float [bmih.biHeight / noThreads];
	}*/
	//std::vector<std::future<float*>> vecOfThreads;
	/*for (int i = 0; i < 3 * bmih.biWidth * bmih.biHeight; ++i) {
		modifiedData[i] = 0;
	}*/
	HINSTANCE hinstLib = cpp ? LoadLibrary(TEXT("Dll1.dll")) : LoadLibrary(TEXT("JADll.dll"));
	float mask[] = { 1.0f, 1.0f, 1.0f, 1.0f, -8.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	BOOL fFreeResult;
	if (hinstLib) {
		if (cpp) {
			laplaceCpp laplace = (laplaceCpp)GetProcAddress(hinstLib, "laplaceFilter");
			if (laplace) {
				//laplace(bmih.biWidth, bmih.biHeight, noThreads, 5, data, std::ref(modifiedData), mask);
				for (int i = 0; i < noThreads; ++i) {
					//laplace(bmih.biWidth, bmih.biHeight, noThreads, i, data, *modifiedData, mask);
					int noRows = bmih.biHeight - (noThreads * (bmih.biHeight / noThreads)) > i ? bmih.biHeight / noThreads + 1 : bmih.biHeight / noThreads;
					modifiedData[i] = new float[3 * noRows * bmih.biWidth];
					for (int k = 0; k < 3 * noRows * bmih.biWidth; ++k) {
						modifiedData[i][k] = 0.0f;
					}
					std::thread a(laplace, data, modifiedData[i], mask, bmih.biWidth, bmih.biHeight, noThreads, i);
					vecOfThreads.emplace_back(std::move(a));
					/*std::future<float*> a = std::async(laplace, bmih.biWidth, bmih.biHeight, noThreads, i, data, mask);
					vecOfThreads.emplace_back(std::move(a));*/
				}
			}
		}
		else {
			laplaceAsm laplace = (laplaceAsm)GetProcAddress(hinstLib, "laplaceFilter");
			if (laplace) {
				for (int i = 0; i < noThreads; ++i) {
					int noRows = bmih.biHeight - (noThreads * (bmih.biHeight / noThreads)) > i ? bmih.biHeight / noThreads + 1 : bmih.biHeight / noThreads;
					modifiedData[i] = new float[3 * noRows * bmih.biWidth];
					std::thread a(laplace, data, modifiedData[i], mask, bmih.biWidth, bmih.biHeight, noThreads, i);
					vecOfThreads.emplace_back(std::move(a));
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
		FreeLibrary(hinstLib);
	}
	float* finalData = new float[3 * bmih.biHeight * bmih.biWidth];
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

void Bmp::saveImage(float* modifiedData, const char* destinationFile) {
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
	const int fileSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + 3 * bmih.biHeight;
	dt.resize(bmih.biWidth * bmih.biHeight);
	//bmfht.bfSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + 3 * bmih.biHeight;
	header[2] = fileSize;
	header[3] = fileSize >> 8;
	header[4] = fileSize >> 16;
	header[5] = fileSize >> 24;
	for (int y = 0; y < bmih.biHeight; ++y) {
		for (int x = 0; x < bmih.biWidth; ++x) {
			//std::cout << data[3 * bmih.biWidth * y + 3 * x] << "  " << data[3 * bmih.biWidth * y + 3 * x+1] << "  " << data[3 * bmih.biWidth * y + 3 * x +2] << std::endl;
			setColour(Colour(modifiedData[3 * bmih.biWidth * y + 3 * x], modifiedData[3 * bmih.biWidth * y + 3 * x + 1], modifiedData[3 * bmih.biWidth * y + 3 * x + 2]), x, y);
			//setColour(Colour((float)x / (float)bmih.biWidth, 1.0f - ((float)x / (float)bmih.biWidth), (float)y / (float)bmih.biHeight), x, y);
		}
	}
	
	//setColour(Colour(0, 0, 55), 0, 100);
	file.write(reinterpret_cast<char*>(header), BMP_File_Header);
	for (int y=0; y < bmih.biHeight; ++y) {
		for (int x = 0; x < bmih.biWidth; ++x) {
			unsigned char r = static_cast<unsigned char>(getColour(x, y).red * 255.0f);
			unsigned char g = static_cast<unsigned char>(getColour(x, y).green * 255.0f);
			unsigned char b = static_cast<unsigned char>(getColour(x, y).blue * 255.0f);
			unsigned char c[] = {b, g, r};
			file.write(reinterpret_cast<char*>(c), 3);
		}
		file.write(reinterpret_cast<char*>(padding), paddingSize);
	}
	file.close();
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


