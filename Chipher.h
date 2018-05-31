#include "BigInt.h"
#include <time.h>
#include <string>
#define Int BigInt
using namespace System;
using namespace System::Threading;
using namespace System::ComponentModel;

class Chipher {
	int sizeKey;
	Int encr;
	Int decr;
	Int sizeField;

	friend Int BigRand(Int & left, Int & right);
	friend Int LongPow(Int & x, Int y, Int & n);
	friend Int AlgEuclid(Int a, Int b, Int & x, Int & y);
	friend int testMillerRabin(Int & n, int  k = 60);
	friend void GenerationNumber(Object^ _size);
	friend void EncryptElement(Object^ _j);
	friend void DecryptElement(Object^ _j);
public:
	Chipher(int _sizeKey = 2);
	~Chipher();
	Chipher(const Chipher & ch);
	void GenerationKey();
	Int * Encrypt(std::string & text, Int * encrData, int sizeStr, BackgroundWorker^ worker);
	std::string Decrypt(Int * encrData, int sizeStr, BackgroundWorker^ worker);
	void setKey (Int _encr, Int _sizeField);
	std::string getKey();
	void setSizeKey(int _sizeKey);
};