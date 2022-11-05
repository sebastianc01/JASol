#include "BmpHeader.h"

BmpHeader::BmpHeader(unsigned char* headerData) {
    if (sizeof(headerData)/sizeof(unsigned char) < 54) {
        signature = *(int*)&headerData[0];
        fileSize = *(int*)&headerData[2];
        reserved = *(int*)&headerData[6];
        dataOffset = *(int*)&headerData[10];
        dibHeaderSize = *(int*)&headerData[14];
        widthPx = *(int*)&headerData[18];
        heightPx = *(int*)&headerData[22];
        planes = *(int*)&headerData[26];
        bitCount = *(int*)&headerData[28];
        compression = *(int*)&headerData[30];
        imageSize = *(int*)&headerData[34];
        xResolution_ppm = *(int*)&headerData[38];
        yResolution_ppm = *(int*)&headerData[42];
        coloursUsed = *(int*)&headerData[46];
        importantColours = *(int*)&headerData[50];
    }
    else {
        std::cout << "Header data string is too small.";
    }
}