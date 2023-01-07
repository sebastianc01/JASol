#include <windows.h> 
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <intrin.h>
#include "../Dll1/JAcpp.h"
#include <iostream>
#include <fstream>
#include "Bmp.h"


int main(void)
{
    //std::ofstream file;
    //file.open("resultsasm.txt");
    //int noCalculations = 600;
    //bool cpp = true;
    //if (file.is_open()) {
    //    for (int i = 0; i < 64; ++i) {
    //        double result = 0;
    //        
    //        for (int k = 0; k < noCalculations; ++k) {
    //            Bmp image("3.bmp", i + 1);
    //            result += image.filter(cpp);
    //        }
    //        //result /= noCalculations;
    //        file << i + 1 << " threads: " << result / noCalculations << "\n";
    //    }
    //}
    //file.close();
    Bmp image("3.bmp", 1);
    double s = image.filter(false);
    std::cout << s;
    return 0;
}