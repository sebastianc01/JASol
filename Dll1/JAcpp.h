#pragma once
#include "pch.h"
#ifdef FILTERLIBRARY_EXPORTS
#define FILTERLIBRARY_API __declspec(dllexport)
#else
#define FILTERLIBRARY_API __declspec(dllimport)
#endif


extern "C" FILTERLIBRARY_API void laplaceFilter(size_t width, size_t height, const int noThreads, size_t position, unsigned char* data, unsigned char* (&modifiedData), const int* mask);
