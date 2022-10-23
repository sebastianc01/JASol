#pragma once
#include <string>
#include <iostream>
#include "../Dll1/JAcpp.h"

class Bmp {
	size_t width;
	size_t height;
	size_t size;
	unsigned char* header;
	unsigned char* data;
	int noThreads;
public:
	Bmp(std::string file, int noThreads);
	void readFile(std::string file, int noThreads);
	void filterCpp();
};