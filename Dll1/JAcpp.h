#pragma once
#include "pch.h"
#ifdef FILTERLIBRARY_EXPORTS
#define FILTERLIBRARY_API __declspec(dllexport)
#else
#define FILTERLIBRARY_API __declspec(dllimport)
#endif


extern "C" FILTERLIBRARY_API void laplaceFilter(unsigned char* data, unsigned char* modifiedData, unsigned char* mask, int width, int height, const int noThreads, int position);
