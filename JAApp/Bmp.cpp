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
	//std::cout << bmfh.bfSize - BMP_File_Header << "  " << bmih.biWidth * bmih.biHeight * 3;
	data = new unsigned char[3 * bmih.biHeight * (bmih.biWidth + paddingSize)];
	f.read(reinterpret_cast<char*>(data), 3 * bmih.biHeight * (bmih.biWidth + paddingSize));
	//fread(data, sizeof(float), bmfh.bfSize - BMP_File_Header, f);
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
}


double Bmp::filter(bool cpp) {
	std::vector<std::thread> vecOfThreads;
	/*for (int i = 0; i < noThreads; ++i) {
		modifiedData[i] = new float [bmih.biHeight / noThreads];
	}*/
	//std::vector<std::future<float*>> vecOfThreads;
	/*for (int i = 0; i < 3 * bmih.biWidth * bmih.biHeight; ++i) {
		modifiedData[i] = 0;
	}*/
	const int paddingSize = (4 - (bmih.biWidth) % 4) % 4;
	HINSTANCE hinstLib = cpp ? LoadLibrary(TEXT("Dll1.dll")) : LoadLibrary(TEXT("JADll.dll"));
	unsigned char* modifiedData = new unsigned char[3 * bmih.biHeight * (bmih.biWidth + paddingSize)];
	for (int i = 0; i < 3 * bmih.biHeight * (bmih.biWidth + paddingSize); ++i) {
		modifiedData[i] = 0;
	}
	//unsigned char mask[] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };
	BOOL fFreeResult;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
	int position = 0;
	if (hinstLib) {
		if (cpp) {
			laplaceCpp laplace = (laplaceCpp)GetProcAddress(hinstLib, "laplaceFilter");
			if (laplace) {
				start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < noThreads; ++i) {
					int noRows = bmih.biHeight - (noThreads * (bmih.biHeight / noThreads)) > i ? bmih.biHeight / noThreads + 1 : bmih.biHeight / noThreads;
					std::thread a(laplace, data, modifiedData, paddingSize, bmih.biWidth, bmih.biHeight, noThreads, position, noRows);
					vecOfThreads.emplace_back(std::move(a));
					position += noRows;
				}
			}
		}
		else {
			//laplaceAsm laplace = (laplaceAsm)GetProcAddress(hinstLib, "laplaceFilter");
			laplaceAsm laplace = (laplaceAsm)GetProcAddress(hinstLib, "laplaceAsm");
			if (laplace) {
				start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < noThreads; ++i) {
					//laplace(bmih.biWidth, bmih.biHeight, noThreads, i, data, *modifiedData, mask);
					int noRows = bmih.biHeight - (noThreads * (bmih.biHeight / noThreads)) > i ? bmih.biHeight / noThreads + 1 : bmih.biHeight / noThreads;
					//std::cout << "Thread number: " << i << ", position: " << position << std::endl;
					std::thread a(laplace, data, modifiedData, paddingSize, bmih.biWidth, bmih.biHeight, noThreads, position, noRows);
					vecOfThreads.emplace_back(std::move(a));
					position += noRows;
					/*std::future<float*> a = std::async(laplace, bmih.biWidth, bmih.biHeight, noThreads, i, data, mask);
					vecOfThreads.emplace_back(std::move(a));*/
				}
			}
		}
		for (int i = 0, pos = 0; i < noThreads; ++i) {
			vecOfThreads.at(i).join();
		}
		end = std::chrono::high_resolution_clock::now();
		FreeLibrary(hinstLib);
	}
	std::chrono::duration<double> diff = end - start;
	//std::cout<<"Time: "<<diff.count();
	saveImage(modifiedData, "result.bmp");
	delete[] modifiedData;
	return diff.count();
}

void Bmp::saveImage(unsigned char* modifiedData, const char* destinationFile) {
	std::ofstream file;
	file.open(destinationFile, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Pliku nie udalo sie zapisac.";
		return;
	}
	const int paddingSize = (4 - (bmih.biWidth * 3) % 4) % 4;
	//BITMAPFILEHEADER bmfht = bmfh;
	//BITMAPINFOHEADER bmiht = bmih;
	const int fileSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + paddingSize * bmih.biHeight;
	//dt.resize(bmih.biWidth * bmih.biHeight);
	//bmfht.bfSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + 3 * bmih.biHeight;
	header[2] = fileSize;
	header[3] = fileSize >> 8;
	header[4] = fileSize >> 16;
	header[5] = fileSize >> 24;
	file.write(reinterpret_cast<char*>(header), BMP_File_Header);
	file.write(reinterpret_cast<char*>(modifiedData), 3 * bmih.biHeight * (bmih.biWidth + paddingSize));
	file.close();
}

