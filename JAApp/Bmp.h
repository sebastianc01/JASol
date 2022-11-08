#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <iterator>


#define BMP_File_Header 54

typedef void(_stdcall* laplaceAsm)(int , int, const int, int, unsigned char*, unsigned char*, const int*);
typedef void(*laplaceCpp)(int, int, const int, int, unsigned char*, unsigned char*, const int*);

class Bmp {
	size_t width;
	size_t height;
	size_t size;
	unsigned char* header;
	unsigned char* data;
	int noThreads;
	BITMAPINFOHEADER bmih;
	BITMAPFILEHEADER bmfh;
public:
	Bmp(std::string file, int noThreads);
	~Bmp();
	void readFile(std::string file, int noThreads);
	void filter(bool cpp);
	void saveImage(unsigned char* modifiedData, const char* destinationFile);
};