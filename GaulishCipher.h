#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

namespace GaulishCipher {

	using std::cin;
	using std::endl;
	using std::cout;
	using std::ifstream;
	using std::stringstream;
	using std::ofstream;
	using std::string;
	using std::ios;
	using std::abort;

	class Cipher {
	private:
		int offset;
		int initSalt;


		int positionPower;
		int offsetPower;
		int prevPower;

		int cap;

		string plaintext;
		string encrypted;
		string decrypted;
	public:

		Cipher();
		~Cipher();

		const int getInitSalt();
		const int getOfst();
		const int getPrevPow();
		const int getPosPow();
		const int getOfstPow();

		void setInitSalt(int salt);
		void setOfst(int ofst);
		void setPrevPow(int prevPow);
		void setPosPow(int posPow);
		void setOfstPow(int ofstPow);
		void setCap(int cp);


		void setPlaintext(string text);
		void setEncrypted(string encryption);

		string getDecrypted();
		string getEncrypted();


		int generateMagicNumber(int modifier, int idx);

		void encrypt();
		void decrypt();

	};


	//offset.init.posPower.offPower.prevPower.cap

	class Encryptor {

	private:
		int action;
		string targetPath;
		string key;
		string resultPath;

		Cipher cipher;

	public:

		Encryptor();
	    ~Encryptor();

		void takeArgs(int INact, string INtarget, string INkey, string INdest);

		void cleanUserInput();
		void getUserAction();
		void getTargetPath();
		void getKey();
		void getResultPath();

		void loadFile();
		void writeResult();
		void parseKey();

		void doAction();
	};
}
