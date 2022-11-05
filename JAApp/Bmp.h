#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include "../Dll1/JAcpp.h"
//#include "BmpHeader.h"

#define BMP_File_Header 54

typedef int(_stdcall* laplaceFilterAsm)();

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