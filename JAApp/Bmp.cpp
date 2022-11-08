#include "Bmp.h"

Bmp::Bmp(std::string file, int noThreads) {
	readFile(file, noThreads);
	this->noThreads = noThreads;
}

Bmp::~Bmp() {
	//delete currentHeader;
}

void Bmp::readFile(std::string file, int noThreads) {
	FILE* f;
	fopen_s(&f, file.c_str(), "r");
	if (!f) {
		std::cerr << "\nThe file cannot be opened.\n";
		return;
	}
	header = new unsigned char[BMP_File_Header];
	fread(header, sizeof(unsigned char), BMP_File_Header, f); //54 to wielkoœæ headera
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

	data = new unsigned char[bmfh.bfSize - BMP_File_Header];
	fread(data, sizeof(unsigned char), bmfh.bfSize - BMP_File_Header, f);
}

//void Bmp::filterCpp() {
//	std::vector<std::thread> vecOfThreads;
//	int mask[] = {1, 1, 1, 1, -8, 1, 1, 1, 1};
//	unsigned char* modifiedData = new unsigned char[bmfh.bfSize - BMP_File_Header];
//	HINSTANCE hinstLib;
//	BOOL fFreeResult;
//	hinstLib = LoadLibrary(TEXT("Dll1.dll"));
//	if (hinstLib != NULL)
//	{
//		laplaceFilter f = (laplaceFilter)GetProcAddress(hinstLib, "laplaceFilter");
//		if (NULL != f)
//		{
//			f(bmih.biWidth, bmih.biHeight, noThreads, 10, data, modifiedData, mask);
//		}
//		fFreeResult = FreeLibrary(hinstLib);
//	}
//	for (int i = 0; i < bmfh.bfSize - BMP_File_Header; ++i) {
//		modifiedData[i] = 0;
//	}
//	/*for (int i = 0; i < noThreads; ++i) {
//		std::thread a(filter, bmih.biWidth, bmih.biHeight, noThreads, i, data, std::ref(modifiedData), mask);
//		vecOfThreads.emplace_back(std::move(a));
//	}
//	for (int i = 0; i < noThreads; ++i) {
//		vecOfThreads.at(i).join();
//	}*/
//	/*for (int i = 0; i < bmih.biHeight; ++i) {
//		for (int k = 0; k < bmih.biWidth; ++k) {
//			std::cout << modifiedData[i * 9 + k * 3]<<"."<< modifiedData[i * 9 + k * 3 + 1] << "." << modifiedData[i * 9 + k * 3 + 2];
//		}
//		std::cout << std::endl;
//	}*/
//	saveImage(modifiedData, "modifd.bmp");
//}

void Bmp::filter(bool cpp) {
	std::vector<std::thread> vecOfThreads;
	unsigned char* modifiedData = new unsigned char[bmfh.bfSize - BMP_File_Header];

	HINSTANCE hinstLib = cpp ? LoadLibrary(TEXT("Dll1.dll")) : LoadLibrary(TEXT("JADll.dll"));
	int mask[] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };
	BOOL fFreeResult;
	if (hinstLib) {
		if (cpp) {
			laplaceCpp laplace = (laplaceCpp)GetProcAddress(hinstLib, "laplaceFilter");
			if (laplace) {
				//laplace(bmih.biWidth, bmih.biHeight, noThreads, 5, data, std::ref(modifiedData), mask);
				for (int i = 0; i < noThreads; ++i) {
					std::thread a(laplace, bmih.biWidth, bmih.biHeight, noThreads, i, data, std::ref(modifiedData), mask);
					vecOfThreads.emplace_back(std::move(a));
				}
			}
		}
		else {
			laplaceAsm laplace = (laplaceAsm)GetProcAddress(hinstLib, "laplaceFilter");
			if (laplace) {
				data[0] = 'A';
				data[1] = 'B';
				data[2] = 'C';
				modifiedData[0] = 'A';
				modifiedData[1] = 'B';
				modifiedData[2] = 'C';
				for (int i = 0; i < noThreads; ++i) {
					std::thread a(laplace, bmih.biWidth, bmih.biHeight, noThreads, i, data, modifiedData, mask);
					vecOfThreads.emplace_back(std::move(a));
				}
			}
		}
		for (int i = 0; i < noThreads; ++i) {
			vecOfThreads.at(i).join();
		}
		FreeLibrary(hinstLib);
	}
}

void Bmp::saveImage(unsigned char* modifiedData, const char* destinationFile) {
	std::ofstream file(destinationFile, std::ios_base::binary);
	if (file.is_open()) {
		/*fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), file);
		fwrite(&bmih, 1, sizeof(BITMAPINFOHEADER), file);
		fwrite(&modifiedData, sizeof(unsigned char), sizeof(modifiedData), file);
		fclose(file);*/
		file.write((const char*)&bmfh, sizeof(BITMAPFILEHEADER));
		UINT nWrittenFileHeaderSize = file.tellp();

		// And then the bitmap info header
		file.write((const char*)&bmih, sizeof(BITMAPINFOHEADER));
		UINT nWrittenInfoHeaderSize = file.tellp();

		unsigned char colourTable[8] = {0};
		colourTable[4] = 255;
		colourTable[5] = 255;
		colourTable[6] = 255;
		file.write((char*)colourTable, 8);

		// Finally, write the image data itself
		//-- the data represents our drawing
		//for(auto it = std::begin(modifiedData);)
		char* row = new char[3 * bmih.biWidth];
		for (int h = 0; h < bmih.biHeight; ++h) {
			//std::copy(modifiedData[h * 9], modifiedData[h * 9 + 3 * bmih.biWidth], row);
			std::memcpy(row, &modifiedData[h * 3 * bmih.biWidth], 3 * bmih.biWidth);
			file.write(row, bmih.biWidth);
		}
		delete[] row;
		//file.write(reinterpret_cast<char*>(modifiedData), sizeof(modifiedData));
		//UINT nWrittenDIBDataSize = file.tellp();
		file.close();
	}
	else {
		std::cout << "Saving unsuccessful.";
	}
}