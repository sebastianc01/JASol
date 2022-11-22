#include <windows.h> 
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <intrin.h>
#include "../Dll1/JAcpp.h"
#include "Bmp.h"


int main(void)
{
    
    Bmp image("4.bmp", 10);
    image.filter(true);
    return 0;

}