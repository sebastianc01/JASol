#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <chrono>


#define BMP_File_Header 54
#define FileHeaderSize 14
#define InfoHeaderSize 40

typedef void*(_stdcall* laplaceAsm)(unsigned char*, unsigned char*, int, int, int, const int, int, int);
typedef void*(_stdcall* TestAsm)(unsigned char*, unsigned char*, int, int, int, const int, int, int);
typedef void(*laplaceCpp)(unsigned char*, unsigned char*, int, int, int, const int, int, const int);

class Bmp {
	//size_t width;
	//size_t height;
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
	double filter(bool cpp);
	void saveImage(unsigned char* modifiedData, const char* destinationFile);
};