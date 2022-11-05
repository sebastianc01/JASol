#pragma once
#include <cstdint>
#include <iostream>

class BmpHeader {
public:
    uint16_t signature;              
    uint32_t fileSize;              
    uint32_t reserved;                 
    uint32_t dataOffset;            
    uint32_t dibHeaderSize;   
    int32_t  widthPx;          
    int32_t  heightPx;         
    uint16_t planes;        
    uint16_t bitCount;    
    uint32_t compression;       
    uint32_t imageSize;  
    int32_t  xResolution_ppm;  
    int32_t  yResolution_ppm;  
    uint32_t coloursUsed;        
    uint32_t importantColours;

    BmpHeader(unsigned char* headerData);
};

