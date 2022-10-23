// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <string>
#include <iostream>
#include "JAcpp.h"

void laplaceFilter(std::string file, size_t width, size_t height) {
	FILE* f;
	fopen_s(&f, file.c_str(), "r");
	if (!f) {
		std::cerr << "\nThe file cannot be opened.\n";
		return;
	}
	size_t Size = width * height;
	//Calculating size
	size_t calcSize = 0;
	fseek(f, 0, SEEK_END);
	calcSize = ftell(f);
	rewind(f);
	std::cout << "Bity: " << 200 * 200 * sizeof(unsigned char)<<std::endl << calcSize/3;
	size_t bytes = 0;
	size_t read = 0;
	unsigned char *data = new unsigned char[Size];
	FILE* fp;
	fopen_s(&fp, "result.bmp", "w+");
	while (bytes < Size)
	{
		read = fread(data, sizeof(unsigned char), Size, f);
		bytes += read;
		for (int i = 0; i < read; ++i)
		{
			fprintf(fp, "%d", data[i]);
			if ((i % width) == 0 && i != 0)
				fprintf(fp, "\n");
		}
	}

}