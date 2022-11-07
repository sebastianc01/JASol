#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "../Dll1/JAcpp.h"
//#include "BmpHeader.h"

#define BMP_File_Header 54

typedef void(_stdcall* laplaceFilterAsm)(size_t width, size_t height, const int noThreads, size_t position, unsigned char* data, unsigned char* (&modifiedData), const int* mask);

class Bmp {
	size_t width;
	size_t height;
	size_t size;
	unsigned char* header;
	unsigned char* data;
	int noThreads;
	//BmpHeader *currentHeader;
	BITMAPINFOHEADER bmih;
	BITMAPFILEHEADER bmfh;
public:
	Bmp(std::string file, int noThreads);
	~Bmp();
	void readFile(std::string file, int noThreads);
	void filterCpp();
	void filterAsm();
	void saveImage(unsigned char* modifiedData, const char* destinationFile);
};