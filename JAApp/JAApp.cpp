#include <windows.h> 
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <intrin.h>
#include "../Dll1/JAcpp.h"
#include "Bmp.h"

typedef int(_stdcall* CpuIdAsm)();


int main(void)
{
    HINSTANCE hinstLib;
    CpuIdAsm CIA;
    BOOL fFreeResult;

    hinstLib = LoadLibrary(TEXT("JADll.dll"));
    //laplaceFilter("asd.bmp", 200, 200);
    //readFile("asd");
    Bmp image("asd.bmp", 10);
    if (hinstLib != NULL)
    {
        CIA = (CpuIdAsm)GetProcAddress(hinstLib, "CpuIdAsm");
        // If the function address is valid, call the function.
        if (NULL != CIA)
        {
            if ((CIA)()) {
            }
        }
        fFreeResult = FreeLibrary(hinstLib);
    }

    
    return 0;

}