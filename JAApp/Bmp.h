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
#include "Colour.h"


#define BMP_File_Header 54
#define FileHeaderSize 14
#define InfoHeaderSize 40

typedef float*(_stdcall* laplaceAsm)(unsigned char*, unsigned char*, unsigned char* , int, int, const int, int);
typedef void(*laplaceCpp)(unsigned char*, unsigned char*, unsigned char* , int, int, const int, int, int);

class Bmp {
	//size_t width;
	//size_t height;
	size_t size;
	unsigned char* header;
	unsigned char* data;
	int noThreads;
	BITMAPINFOHEADER bmih;
	BITMAPFILEHEADER bmfh;
	std::vector<Colour> dt;
public:
	Bmp(std::string file, int noThreads);
	~Bmp();
	void readFile(std::string file, int noThreads);
	void filter(bool cpp);
	void saveImage(unsigned char* modifiedData, const char* destinationFile);
	Colour getColour(int x, int y) const;
	void setColour(const Colour& colour, int x, int y);
};