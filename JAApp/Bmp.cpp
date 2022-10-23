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