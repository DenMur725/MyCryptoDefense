#include <string>
#define BIGINT_MAX 1000000000 
class BigInt {
	int size;
	int sizeMem;
	char sign;
	int * mem;

	int realSize ();
	void removingZeros ();
public:
	BigInt (int _sizeMem = 1);
	BigInt (const BigInt & b);
	BigInt (const std::string & st);
	~BigInt ();
	BigInt & operator = (const BigInt & b);
	BigInt & operator = (const __int64 & b);
	BigInt operator + (BigInt & b);
	BigInt operator - (BigInt & b);
	BigInt operator * (BigInt & b);
	BigInt operator / (BigInt & b);
	BigInt operator % (BigInt & b);
	char operator < (BigInt & b);
	char operator > (BigInt & b);
	char operator <= (BigInt & b);
	char operator >= (BigInt & b);
	char operator == (BigInt & b);
	char operator != (BigInt & b);
	int & operator [] (int k);
	int getSize ();
	int getSign ();
	std::string BigIntToString ();
	__int64 BigIntToInt();
	void generationBySize (int _size);
};