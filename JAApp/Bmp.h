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
#include "Colour.h"


#define BMP_File_Header 54
#define FileHeaderSize 14
#define InfoHeaderSize 40

typedef float*(_stdcall* laplaceAsm)(int , int, const int, int, float*, float*);
typedef void(*laplaceCpp)(int, int, const int, int, float*, float*, float*);

class Bmp {
	size_t width;
	size_t height;
	size_t size;
	unsigned char* header;
	float* data;
	int noThreads;
	BITMAPINFOHEADER bmih;
	BITMAPFILEHEADER bmfh;
	std::vector<Colour> dt;
public:
	Bmp(std::string file, int noThreads);
	~Bmp();
	void readFile(std::string file, int noThreads);
	void filter(bool cpp);
	void saveImage(float* modifiedData, const char* destinationFile);
	Colour getColour(int x, int y) const;
	void setColour(const Colour& colour, int x, int y);
};