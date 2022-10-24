#include "Bmp.h"

Bmp::Bmp(std::string file, int noThreads) {
	readFile(file, noThreads);
	this->noThreads = noThreads;
}

void Bmp::readFile(std::string file, int noThreads) {
	FILE* f;
	fopen_s(&f, file.c_str(), "r");
	if (!f) {
		std::cerr << "\nThe file cannot be opened.\n";
		return;
	}
	header = new unsigned char[55];
	fread(header, sizeof(unsigned char), 54, f); //54 to wielkoœæ headera
	width = *(int*)&header[18]; //http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp_en.html
	height = *(int*)&header[22];
	size = *(int*)&header[2] - 54; //wielkoœæ pliku bez headera
	data = new unsigned char[size];
	fread(data, sizeof(unsigned char), size, f);
}

void Bmp::filterCpp() {
	std::vector<std::thread> vecOfThreads;
	unsigned char* modifiedData = new unsigned char[size];
	for (int i = 0; i < noThreads; ++i) {
		std::thread a(laplaceFilter, data, std::ref(modifiedData), width, height, size, i);
		vecOfThreads.emplace_back(std::move(a));
	}
	for (int i = 0; i < noThreads; ++i) {
		vecOfThreads.at(i).join();
	}
}

void Bmp::filterAsm() {
	std::vector<std::thread> vecOfThreads;
	unsigned char* modifiedData = new unsigned char[size];
	HINSTANCE hinstLib;
	laplaceFilterAsm CIA;
	BOOL fFreeResult;
	hinstLib = LoadLibrary(TEXT("JADll.dll"));;
	if (hinstLib != NULL)
	{

		CIA = (laplaceFilterAsm)GetProcAddress(hinstLib, "laplaceFilterAsm");
		/*if (NULL != CIA)
		{
			
		}*/
		fFreeResult = FreeLibrary(hinstLib);
	}
}