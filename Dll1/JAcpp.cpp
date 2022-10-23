// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include "JAcpp.h"

unsigned char* laplaceFilter(unsigned char* data, size_t width, size_t height, size_t size, int noThreads) {
	std::vector<std::thread> vecOfThreads;
	auto calculatePixel = [](unsigned char (*d)[], int n, size_t s, size_t h, size_t w) {
		for (int i = n; i < s; i += n) {

		}
	};
	unsigned char* modifiedData = new unsigned char[size];
	for (int i = 0; i < noThreads; ++i) {
		std::thread a(calculatePixel, data, i, size, height, width);
		vecOfThreads.push_back(a);
	}
	for (int i = 0; i < noThreads; ++i) {
		vecOfThreads.at(i).join();
	}
	return data;
}