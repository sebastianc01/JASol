// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <string>

#include "JAcpp.h"

void laplaceFilter(unsigned char* data, unsigned char* (&modifiedData), size_t width, size_t height, size_t size, size_t position, const int* mask) {
	for (size_t H = position; H < height; H += position) { //height of the whole image
		for (size_t W = 0; W < width; ++W) { //width of the whole image
			for (int i = 0; i < 3; ++i) { //considering 3 colours of all bits
				for (int h = -1; h <= 1; ++h) { //height of the mask
					if (h + H >= 0 && h + H < height) { //considered height has to be greater than 0 and smaller then maximum height of the image
						for (int w = -1; w <= 1; ++w) { //width of the mask
							if (w + W >= 0 && w + W < width) { //considered width has to be greater than 0 and smaller then maximum width of the image
								modifiedData[(H + h) * 3 * 3 + (W + w) * 3 + i] = modifiedData[(H + h) * 3 * 3 + (W + w) * 3 + i] + data[(H + h) * 3 * 3 + (W + w) * 3 + i] * mask[h * 3 + w];
							}
						}
					}
				}
			}
		}
	}
}