// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <string>
#include <iostream>
#include "JAcpp.h"

void laplaceFilter(unsigned char* data, unsigned char* modifiedData, int paddingSize, int width, int height, const int noThreads, int position, const int noRows) {
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
				}
			}
		}
	}
}