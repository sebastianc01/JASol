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

	data = new float[bmfh.bfSize - BMP_File_Header];
	fread(data, sizeof(float), bmfh.bfSize - BMP_File_Header, f);
	std::cout << "a";
}


void Bmp::filter(bool cpp) {
	std::vector<std::thread> vecOfThreads;
	float* modifiedData = new float[bmfh.bfSize - BMP_File_Header];

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
	saveImage(modifiedData, "result.bmp");
}

void Bmp::saveImage(float* modifiedData, const char* destinationFile) {
	//std::ofstream file(destinationFile, std::ios_base::binary);
	//if (file.is_open()) {
	//	/*fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), file);
	//	fwrite(&bmih, 1, sizeof(BITMAPINFOHEADER), file);
	//	fwrite(&modifiedData, sizeof(unsigned char), sizeof(modifiedData), file);
	//	fclose(file);*/
	//	file.write((const char*)&bmfh, sizeof(BITMAPFILEHEADER));
	//	UINT nWrittenFileHeaderSize = file.tellp();

	//	// And then the bitmap info header
	//	file.write((const char*)&bmih, sizeof(BITMAPINFOHEADER));
	//	UINT nWrittenInfoHeaderSize = file.tellp();

	//	unsigned char colourTable[8] = {0};
	//	colourTable[4] = 255;
	//	colourTable[5] = 255;
	//	colourTable[6] = 255;
	//	file.write((char*)colourTable, 8);

	//	// Finally, write the image data itself
	//	//-- the data represents our drawing
	//	//for(auto it = std::begin(modifiedData);)
	//	char* row = new char[3 * bmih.biWidth];
	//	for (int h = 0; h < bmih.biHeight; ++h) {
	//		//std::copy(modifiedData[h * 9], modifiedData[h * 9 + 3 * bmih.biWidth], row);
	//		std::memcpy(row, &modifiedData[h * 3 * bmih.biWidth], 3 * bmih.biWidth);
	//		file.write(row, bmih.biWidth);
	//	}
	//	delete[] row;
	//	//file.write(reinterpret_cast<char*>(modifiedData), sizeof(modifiedData));
	//	//UINT nWrittenDIBDataSize = file.tellp();
	//	file.close();
	//}
	//else {
	//	std::cout << "Saving unsuccessful.";
	//}
	std::ofstream file;
	file.open(destinationFile, std::ios::out || std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Pliku nie udalo sie zapisac.";
		return;
	}
	unsigned char padding[3] = { 0, 0, 0 };
	const int paddingSize = (4 - (bmih.biWidth * 3) % 4) % 4;
	//BITMAPFILEHEADER bmfht = bmfh;
	//BITMAPINFOHEADER bmiht = bmih;
	const int fileSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + 3 * bmih.biHeight;
	dt.resize(fileSize);
	//bmfht.bfSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + 3 * bmih.biHeight;
	header[2] = fileSize;
	header[3] = fileSize >> 8;
	header[4] = fileSize >> 16;
	header[5] = fileSize >> 24;
	//char* a = new char (sizeof(bmfht));
	//send(a, (char*)&bmfht, sizeof(bmfht), 0);
	//send(a, lpbitmap, dwBmpSize, 0);
	//file.write(reinterpret_cast<char*>(bmfht), FileHeaderSize);
	//file.write(&bmiht, InfoHeaderSize);
	for (int y = 0; y < bmih.biHeight; ++y) {
		for (int x = 0; x < bmih.biWidth; ++x) {
			setColour(Colour(data[3 * bmih.biWidth * y + 3 * x], data[3 * bmih.biWidth * y + 3 * x + 1], data[3 * bmih.biWidth * y + 3 * x + 2]), x, y);
		}
	}
	setColour(Colour(55, 55, 55), 0, 100);
	file.write(reinterpret_cast<char*>(header), BMP_File_Header);
	for (int y=0; y < bmih.biHeight; ++y) {
		for (int x = 0; x < bmih.biWidth; ++x) {
			unsigned char r = getColour(x, y).red * 255.0f;
			unsigned char g = getColour(x, y).green * 255.0f;
			unsigned char b = getColour(x, y).blue * 255.0f;
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
}


