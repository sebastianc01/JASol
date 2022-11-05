#pragma once
#include "pch.h"
#ifdef FILTERLIBRARY_EXPORTS
#define FILTERLIBRARY_API __declspec(dllexport)
#else
#define FILTERLIBRARY_API __declspec(dllimport)
#endif


extern "C" FILTERLIBRARY_API void laplaceFilter(unsigned char* data, unsigned char* (&modifiedData), size_t width, size_t height, size_t size, size_t position, const int* mask);
