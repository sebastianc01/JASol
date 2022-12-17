// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <string>
#include <iostream>
#include "JAcpp.h"

void laplaceFilter(unsigned char* data, unsigned char* modifiedData, int paddingSize, int width, int height, const int noThreads, int position) {
	const int noRows = height - (noThreads * (height / noThreads)) > position ? height / noThreads + 1 : height / noThreads;
	int mask;
	//for (int i = 0; i < 3 * noRows * width; ++i) {
	//	int dataArg = position * width + i;
	//	modifiedData[i] = data[3 * position * width + i]; //works fine
	//}
	//return;
	//for (int H = 0; H < noRows; ++H) {
	//	if (!(H == 0 && position == 0) && !(H == noRows - 1 && position + noRows >= height)) {
	//		for (int W = 1; W < width - 1; ++W) {
	//			for (int i = 0; i < 3; ++i) {
	//				for (int h = -1; h <= 1; ++h) {
	//					//if (h + H >= 0 && h + H < height) {
	//						for (int w = -1; w <= 1; ++w) {
	//							//if (w + W >= 0 && w + W < width) {
	//								mask = w == 0 && h == 0 ? 8 : -1;
	//								int modDataArg = 3 * position * (width + paddingSize) + 3 * H * (width + paddingSize) + 3 * W + i;
	//								int dataArg = 3 * position * (width + paddingSize) + 3 * (H + h) * (width + paddingSize) + 3 * (W + w) + i;
	//								if (3 * position * (width + paddingSize) + 3 * H * (width + paddingSize) + 3 * W + i < 0 || 3 * position * (width + paddingSize) + 3 * H * (width + paddingSize) + 3 * W + i > 3 * height * (width + paddingSize)) {
	//									std::cout << "Nieprawidlowy adres modifiedData, max: " << 3 * height * (width + paddingSize) << ", curr: " << 3 * (H + position) * (width + paddingSize) + 3 * W + i << std::endl;
	//								}
	//								if (3 * position * (width + paddingSize) + 3 * (H + h) * (width + paddingSize) + 3 * (W + w) + i <0|| 3 * position * (width + paddingSize) + 3 * (H + h) * (width + paddingSize) + 3 * (W + w) + i> 3 * height * (width + paddingSize)) {
	//									std::cout << "Nieprawidlowy adres data. max: " << 3 * height * (width + paddingSize) << ", curr: " << 3 * (position + H + h) * (width + paddingSize) + 3 * (W + w) + i << std::endl;
	//								}
	//								/*if (modDataArg > 3 * height * (width + paddingSize) -900) {
	//									int max = 3 * height * (width + paddingSize);
	//									int currY = position + H;
	//								}*/
	//								if (3 * position * (width + paddingSize) + 3 * H * (width + paddingSize) + 3 * W + i == 34809) {
	//									std::cout << "position: " << position << ", width: " << W << ", height: " << H << ", i:" << i << ", paddingSize:"<< paddingSize << std::endl;
	//								}
	//								unsigned char a = modifiedData[3 * (position + H) * (width + paddingSize) + 3 * W + i];
	//								unsigned char b = data[3 * position * (width + paddingSize) + 3 * (H + h) * (width + paddingSize) + 3 * (W + w) + i];
	//								modifiedData[3 * (position + H) * (width + paddingSize) + 3 * W + i] =
	//									modifiedData[3 * (position + H) * (width + paddingSize) + 3 * W + i]
	//									+ (data[3 * (position + H + h) * (width + paddingSize) + 3 * (W + w) + i]
	//										* mask);
	//								
	//							//}
	//							//else {
	//								//std::cout << "W: " << w << ", w: " << w << "H: " << H << ", h: " << h << ", position: " << position << std::endl;
	//							//}
	//						}
	//					//}
	//				}
	//			}
	//			if (W < 10 && H < 10 && position == 0) {
	//				std::cout << "H: " << H << ", W: " << W << ", position: " << position << ", curr modified data: " << (int)modifiedData[3 * (position + H) * (width + paddingSize) + 3 * W] << ", curr mod data arg: " << 3 * (position + H) * (width + paddingSize) + 3 * W << ", curr data: " << (int)data[3 * (position + H) * (width + paddingSize) + 3 * (W)] << ", curr data arg: "<< 3 * (position + H) * (width + paddingSize) + 3 * (W) << std::endl;
	//			}
	//		}
	//	}
	//}
	/*modifiedData[3 * width * (noRows - 1) + 3 * 20 + 1] = 100;
	modifiedData[3 * width * (noRows - 1) + 3 * 20 + 2] = 100;
	modifiedData[3 * width * (noRows - 1) + 3 * 20 + 3] = 100;*/
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
	for (int H = position; H <  position + noRows; ++H) {
		if (!(H == 0 || H == height - 1)) {
			for (int W = 1; W < width - 1; ++W) {
				for (int i = 0; i < 3; ++i) {
					modifiedData[3 * H * (width + paddingSize) + 3 * W + i] =
						data[3 * H * (width + paddingSize) + 3 * W + i] * (8) +
						data[3 * (H - 1) * (width + paddingSize) + 3 * (W - 1) + i] * (-1) +
						data[3 * (H - 1) * (width + paddingSize) + 3 * W + i] * (-1) +
						data[3 * (H - 1) * (width + paddingSize) + 3 * (W + 1) + i] * (-1) +
						data[3 * H * (width + paddingSize) + 3 * (W - 1) + i] * (-1) +
						data[3 * H * (width + paddingSize) + 3 * (W + 1) + i] * (-1) +
						data[3 * (H + 1) * (width + paddingSize) + 3 * (W - 1) + i] * (-1) +
						data[3 * (H + 1) * (width + paddingSize) + 3 * W + i] * (-1) +
						data[3 * (H + 1) * (width + paddingSize) + 3 * (W + 1) + i] * (-1);
					/*if (W == 50) {
						modifiedData[3 * H * (width + paddingSize) + 3 * W + i] = 200;
					}
					if (W == 100) {
						modifiedData[3 * H * (width + paddingSize) + 3 * W + i] = 200;
					}*/
					if (H < 10) {
						//std::cout << "H: " << H << ", W: " << W << ", arg: " << 3 * H * (width + paddingSize) + 3 * W + i << ", padding size: " << paddingSize<< std::endl;
					}
				}
			}
			//std::cout << "End of the row, padding size:" << paddingSize << ", paddingSize*3: " << paddingSize * 3 << ", last padding arg: " << 3 * (H+1) * (width + paddingSize) << std::endl;
		}
	}
}