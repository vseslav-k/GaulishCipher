// ComplexEncryption.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GaulishCipher.h"
#include<windows.h>
using namespace GaulishCipher;

int main(int argc, char* argv[]) {
    Encryptor enc;



	if(argc == 5){
		enc.takeArgs(argv[1][0] -48, argv[2], argv[3], argv[4]);
	}
	else if (argc == 4) {
		enc.takeArgs(argv[1][0] - 48, argv[2], argv[3], argv[1][0] - 48 == 1?"enc.gaul":"dec.txt");
	}
	else {
		enc.getUserAction();
		enc.getTargetPath();
		enc.getKey();
		enc.getResultPath();
	}
	enc.doAction();



	Sleep(1500);

	return 0;
}