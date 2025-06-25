#include "GaulishCipher.h" 
using namespace GaulishCipher;


Encryptor::Encryptor() {

	action = 0;
}

Encryptor::~Encryptor() {
}

void Encryptor::getUserAction() {
	cout << "Do you want to encrypt/decrypt a file (1/2): " << endl;

	cin >> action;

	cout  << endl;
}

string replace(string INstr, char target, char replacement) {

	string str = INstr;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == target) {
			str[i] = replacement;
		}
	}

	return str;
}

void Encryptor::getTargetPath() {
	cout << "Enter target file path: " << endl;

	cin >> targetPath;

	cout << endl ;
}

int count(string str, char ch) {
	int cnt = 0;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ch) {
			cnt++;
		}
	}

	return cnt;
}

int lastIdx(string str, char ch) {
	int idx = -1;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ch) {
			idx = i;
		}
	}

	return idx;
}

void Encryptor::cleanUserInput() {

	if (targetPath[0] == '\"' || targetPath[0] == '\'') {
		targetPath.erase(0, 1);
	}
	if (targetPath[targetPath.length() - 1] == '\"' || targetPath[0] == '\'') {
		targetPath.erase(targetPath.length() - 1, 1);
	}

	targetPath = replace(targetPath, '/', '\\');

	if (resultPath[0] == '\"' || resultPath[0] == '\'') {
		resultPath.erase(0, 1);
	}
	if (resultPath[resultPath.length() - 1] == '\"' || resultPath[0] == '\'') {
		resultPath.erase(resultPath.length() - 1, 1);
	}
	resultPath = replace(resultPath, '/', '\\');


	// name
	// name.ext
	// some\file\dir\
	// some\file\dir\name
	// some\file\dir\name.ext


	if (resultPath[resultPath.length() - 1] == '\\') {
		if (action == 1) {
			resultPath += "enc.gaul";
		}
		if (action == 2) {
			resultPath += "dec.txt";
		}
	}

	if (lastIdx(resultPath, '\\') == -1) {
		resultPath = targetPath.substr(0, lastIdx(targetPath, '\\') + 1) + resultPath;
	}

	if (lastIdx(resultPath, '.') == -1) {
		if (action == 1) {
			resultPath += ".gaul";
		}
		if (action == 2) {
			resultPath += ".txt";
		}
	}

	if (count(key, '.') != 5) {
		cout << "ERROR INVALID KEY" << endl;
		//abort();
	}

}



void Encryptor::getResultPath() {
	cout << "Enter result path: " << endl;

	cin >> resultPath;

	cout << endl;
}

void Encryptor::getKey() {
	cout << "Enter Encryption Key in provided format: " << endl;
	cout << "offset.initSalt.posPower.offPower.prevPower.cap" << endl;

	cin >> key;

	if (count(key, '.') != 5) {
		cout << "ERROR INVALID KEY" << endl;
		abort();
	}

	cout << endl;
}
//offset.init.posPower.offPower.prevPower

void Encryptor::parseKey() {


	void (Cipher:: * funcArray[])(int x) = { &Cipher::setOfst, &Cipher::setInitSalt, &Cipher::setPosPow, &Cipher::setOfstPow, &Cipher::setPrevPow, &Cipher::setCap };
	int functionIdx = 5;

	int num = 0;
	int decimalPlace = 0;

	for (int i = key.length()-1; i >= 0; i--) {
			
		if (key[i] == '.') {

			if (num <= 0) {
				cout << "ERROR! Encryption key could not be parsed. Make sure you entered a valid encryption key" << endl << endl;
				abort();
			}


			(cipher.*funcArray[functionIdx])(num);
			decimalPlace = 0;
			num = 0;
			functionIdx--;
			continue;

		}

		num += ((int)key[i] - 48) * pow(10, decimalPlace);
		decimalPlace++;

	}
	(cipher.*funcArray[functionIdx])(num);

}

void Encryptor::loadFile() {

	std::ios_base::openmode 
	mode = ios::in;
	mode |= std::ios::binary;

	ifstream file(targetPath, mode);
	if (!file) {
		cout << "ERROR! Could not open file at path: " << targetPath << endl;
		return;
	}

	stringstream buffer;
	buffer << file.rdbuf(); // Read entire file into buffer

	if (action == 1) {
		cipher.setPlaintext(buffer.str());
	}
	else if (action == 2) {
		cipher.setEncrypted(buffer.str());
	}
	else {
		cout << "ERROR! Invalid Action "<< endl;
		return;
	}

}

void Encryptor::writeResult() {

	std::ios_base::openmode 
	mode = std::ios::out;
	mode |= std::ios::binary;

	ofstream outFile(resultPath, mode);


	if (!outFile) {
		cout << "Error: Could not open file for writing at path: " << resultPath << endl;
		return;
	}


	if (action == 1) {
		outFile << cipher.getEncrypted();
		cout << "SUCCESS: Encrypted contents saved to " << resultPath << endl;
	}
	else if (action == 2) {
		outFile << cipher.getDecrypted();
		cout << "SUCCESS: Decrypted contents saved to " << resultPath << endl;
	}
	else {
		cout << "ERROR! Invalid Action " << endl;
		return;
	}



	outFile.close();

}

void Encryptor::takeArgs(int INact, string INtarget, string INkey, string INdest) {
	action = INact;
	targetPath = INtarget;
	key = INkey;
	resultPath = INdest;

}

void Encryptor::doAction() {
	cleanUserInput();
	parseKey();
	loadFile();
	if (action == 1) {
		cipher.encrypt();
	}
	if (action == 2) {
		cipher.decrypt();
	}

	writeResult();
}


Cipher::Cipher() {
	offset = 0;
	initSalt = 0;
	positionPower = 0;
	offsetPower = 0;
	prevPower = 0;
	cap = 0;
}

Cipher::~Cipher() {

}

const int Cipher::getInitSalt() {
	return initSalt;
}
const int Cipher::getOfst() {
	return offset;
}
const int Cipher::getPrevPow() {
	return prevPower;
}
const int Cipher::getPosPow() {
	return positionPower;
}
const int Cipher::getOfstPow() {
	return offsetPower;
}
void Cipher::setInitSalt(int salt) {
	initSalt = salt;
}
void Cipher::setOfst(int ofst) {
	offset = ofst;
}
void Cipher::setPrevPow(int prevPow) {
	prevPower = prevPow;
}
void Cipher::setPosPow(int posPow) {
	positionPower = posPow;
}
void Cipher::setOfstPow(int ofstPow) {
	offsetPower = ofstPow;
}
void Cipher::setCap(int cp) {
	cap = cp;
}
void Cipher::setPlaintext(string text) {
	plaintext = text;
}
void Cipher::setEncrypted(string encryption) {
	encrypted = encryption;
}
string Cipher::getDecrypted() {
	return decrypted;
}

int Cipher::generateMagicNumber(int modifier, int idx) {


	if (offset == idx) {
		return -2;
	}
	if (idx < 10 && modifier % 10 < 4) {
		return 1;
	}
	if (idx < 10 && modifier % 10 > 4) {
		return 2;
	}
	if (modifier % offsetPower == 1) {
		return 15;
	}
	if (modifier % prevPower == 1) {
		return 15;
	}
	if (modifier % positionPower == 1) {
		return 15;
	}
	if ((prevPower*(modifier % offsetPower))% positionPower == 9) {
		return 8;
	}
	if (modifier % initSalt == 0) {
		return 7;
	}
	if (initSalt % cap == 7) {
		return 3;
	}
	if (cap % modifier == 0) {
		return 6;
	}
	if (cap % idx == initSalt % idx) {
		return 4;
	}
	if ((cap * idx) % 10 == (modifier * idx) % 10) {
		return 10;
	}
	if (idx % modifier > 5) {
		return 9;
	}
	if (idx % modifier < 5) {
		return 6;
	}
	if (idx % modifier == 5) {
		return 11;
	}

	return 0;
}

void Cipher::encrypt() {
	encrypted = plaintext;

	int modifier = offset * offsetPower + initSalt * prevPower;

	encrypted[0] += (modifier % cap) + 1;

	for (int i = 1; i < encrypted.length(); i++) {
		
		modifier = offset * offsetPower + i * positionPower + (int)encrypted[i - 1] * prevPower;

		encrypted[i] += (modifier % cap) + 1;

		encrypted[i] += generateMagicNumber(modifier, i);
	}
}

string Cipher::getEncrypted() {
	return encrypted;
}

void Cipher::decrypt() {

	decrypted = encrypted;

	int modifier = 0;

	for (int i = decrypted.length() - 1; i > 0; i--) {

		modifier = offset * offsetPower + i * positionPower + (int)encrypted[i - 1] * prevPower;

		decrypted[i] -= generateMagicNumber(modifier, i);
		decrypted[i] -= (modifier % cap) + 1;
	}

	modifier = offset * offsetPower + initSalt * prevPower;

	decrypted[0] -= (modifier % cap) + 1;
}