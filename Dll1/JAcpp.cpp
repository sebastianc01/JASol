// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <string>
#include <iostream>
#include "JAcpp.h"

void laplaceFilter(int width, int height, const int noThreads, int position, float* data,float* modifiedData, float* mask) {
	const int noRows = height - (noThreads * (height / noThreads)) > position ? height / noThreads + 1 : height / noThreads;
	modifiedData[position] = 0.78f;
	return;
	//float* modifiedData = new float[3 * noRows * width];
	for (int i = 0; i < 3 * noRows * width; ++i) {
		modifiedData[i] = 0.0f;
	}
	for (size_t H = 0; H < noRows; ++H) {
		for (size_t W = 0; W < width; ++W) {
			for (int i = 0; i < 3; ++i) {
				for (int h = -1; h <= 1; ++h) {
					if (h + H >= 0 && h + H < height) {
						for (int w = -1; w <= 1; ++w) {
							if (w + W >= 0 && w + W < width) {
								int modDataArg = (H + h) * 3 * width + (W + w) * 3 + i;
								int dataArg = 3 * noRows * position * width + (H + h) * 3 * width + (W + w) * 3 + i;
								int maskArg = h * 3 + w + 3 + 1;
								if (H == noRows - 1 && position == 10) {
									std::cout << "s";
								}
								modifiedData[(H + h) * 3 * width + (W + w) * 3 + i] =
									modifiedData[(H + h) * 3 * width + (W + w) * 3 + i]
									+ data[3 * noRows * position * width + (H + h) * 3 * width + (W + w) * 3 + i]
									* mask[h * 3 + w + 3 + 1]; //+1, because w starts with value -1, +3 because h starts with value -1
							}
						}
					}
				}
			}
		}
	}
	//for (size_t H = position; H < height; H += noThreads) { //height of the whole image
	//	for (size_t W = 0; W < width; ++W) { //width of the whole image
	//		for (int i = 0; i < 3; ++i) { //considering 3 colours of every bit
	//			for (int h = -1; h <= 1; ++h) { //height of the mask
	//				if (h + H >= 0 && h + H < height) { //considered height has to be greater than 0 and smaller then maximum height of the image
	//					for (int w = -1; w <= 1; ++w) { //width of the mask
	//						if (w + W >= 0 && w + W < width) { //considered width has to be greater than 0 and smaller then maximum width of the image
	//							modifiedData[(H + h) * 3 * width + (W + w) * 3 + i] =
	//								modifiedData[(H + h) * 3 * width + (W + w) * 3 + i]
	//								+ data[(H + h) * 3 * width + (W + w) * 3 + i] * mask[h * 3 + w];
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	
	//modifiedData = new float[3 * noRows * width];
	/*for (int i = 0; i < noRows; ++i) {
		for (int w = 0; w < width; ++w) {
			modifiedData[3 * i * width + 3 * w] = position;
			modifiedData[3 * i * width + 3 * w + 1] = position + 1.0f;
			modifiedData[3 * i * width + 3 * w + 2] = position + 2.0f;
		}
	}*/
}