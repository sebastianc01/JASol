#pragma once
#include "pch.h"
#ifdef FILTERLIBRARY_EXPORTS
#define FILTERLIBRARY_API __declspec(dllexport)
#else
#define FILTERLIBRARY_API __declspec(dllimport)
#endif


extern "C" FILTERLIBRARY_API void laplaceFilter(float* data, float* modifiedData, float* mask, int width, int height, const int noThreads, int position);
