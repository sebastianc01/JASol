#include <windows.h> 
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <intrin.h>
#include "../Dll1/JAcpp.h"
#include "Bmp.h"


int main(void)
{
    Bmp image("pingwin.bmp", 1);
    image.filter(true);
    return 0;
}