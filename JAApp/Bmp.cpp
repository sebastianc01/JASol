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
	header = new unsigned char[55];
	fread(header, sizeof(unsigned char), 54, f); //54 to wielkoœæ headera
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
	data = new unsigned char[size];
	fread(data, sizeof(unsigned char), size, f);
}

void Bmp::filterCpp() {
	std::vector<std::thread> vecOfThreads;
	int mask[] = {1, 1, 1, 1, -8, 1, 1, 1, 1};
	unsigned char* modifiedData = new unsigned char[size];
	for (int i = 0; i < noThreads; ++i) {
		std::thread a(laplaceFilter, data, std::ref(modifiedData), width, height, size, i, mask);
		vecOfThreads.emplace_back(std::move(a));
	}
	for (int i = 0; i < noThreads; ++i) {
		vecOfThreads.at(i).join();
	}
	saveImage(modifiedData, "modifd.bmp");
}

void Bmp::filterAsm() {
	std::vector<std::thread> vecOfThreads;
	unsigned char* modifiedData = new unsigned char[size];
	HINSTANCE hinstLib;
	laplaceFilterAsm CIA;
	BOOL fFreeResult;
	hinstLib = LoadLibrary(TEXT("JADll.dll"));;
	if (hinstLib != NULL)
	{

		CIA = (laplaceFilterAsm)GetProcAddress(hinstLib, "laplaceFilterAsm");
		/*if (NULL != CIA)
		{
			
		}*/
		fFreeResult = FreeLibrary(hinstLib);
	}
}

void Bmp::saveImage(unsigned char* modifiedData, const char* destinationFile) {
	FILE* file;
	fopen_s(&file, destinationFile, "wb");
	if (file) {
		fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), file);
		fwrite(&bmih, 1, sizeof(BITMAPFILEHEADER), file);
		fwrite(&modifiedData, sizeof(unsigned char), sizeof(modifiedData), file);
		fclose(file);
	}
	else {
		std::cout << "Saving unsuccessful.";
	}
}