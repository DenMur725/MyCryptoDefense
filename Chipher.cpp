#include "Chipher.h"
Int *pq;
Int * _encrData;
std::string *strMemText;
Int _encr, _decr, _sizeField;
int countDigitPos, lengthText;
std::string strText;

Int BigRand(Int & left, Int & right) {
	Int res, tmp, n;
	n = right - left;
	tmp = rand();
	res.generationBySize(n.getSize());
	return (left + ((res + tmp) % n));
}
Int LongPow(Int & x, Int y, Int & n) {
	Int res("1"), _0("0"), _2("2");
	while (y > _0) {
		if (y[0] % 2 == 1)
			res = (res * x) % n;
		x = (x * x) % n;
		y = y / _2;
	}
	return res;
}
Int AlgEuclid(Int a, Int b, Int & x, Int & y) {
	Int _0("0");
	if (b == _0) {
		x = 1;
		y = 0;
		return a;
	}
	Int x1("0"), y1("0");
	Int d1;

	d1 = AlgEuclid(b, a % b, x1, y1);
	x = y1;
	y = x1 - ((a / b) * y1);
	return d1;
}
int testMillerRabin(Int & n, int  k) {
	Int n_1, n_2, t, _1("1"), _2("2"), a, x;
	n_1 = n - _1;
	n_2 = n - _2;
	t = n_1;
	int s = 0, f = 0;
	while (t[0] % 2 != 1) {
		s++;
		t = t / _2;
	}
	for (int i = 0; i < k; i++) {
		a = BigRand(_2, n_2);
		x = LongPow(a, t, n);
		if ((x == _1) || (x == n_1))
			continue;
		for (int j = 1; j < s; j++) {
			x = (x * x) % n;
			if (x == _1)
				return 0;
			if (x == n_1) {
				f = 1;
				break;
			}
		}
		if (f) {
			f = 0;
			continue;
		}
		return 0;
	}
	return 1;
}

void GenerationNumber(Object^ _size) {
	int size = (int)_size;
	Int x, _1("1"), _2("2");
	Int tmp;
	x.generationBySize(size - (size % 2));
	tmp = rand() + (__int64)rand() * BIGINT_MAX * (size % 2);
	x = x + tmp;
	int f = 0;
	if (x[0] % 2 == 0)
		x = x - _1;
	while (!f) {
		x = x - _2;
		f = testMillerRabin(x);
	}
	pq[size % 2] = x;
}
void Chipher::GenerationKey() {
	Int k, x("0"), y("0"), t("0"), _1("1");
	Int* en = new Int[4];
	en[0] = Int("3");
	en[1] = Int("17");
	en[2] = Int("257");
	en[3] = Int("65537");
	pq = new Int[2];
	while (t != _1) {
		Thread^ newThread = gcnew Thread(gcnew ParameterizedThreadStart(GenerationNumber));
		newThread->Start(sizeKey);
		GenerationNumber(sizeKey + 1);
		newThread->Join();
		encr = en[rand() % 4];
		sizeField = pq[0] * pq[1];
		k = (pq[0] - _1)*(pq[1] - _1);
		t = AlgEuclid(encr, k, x, y);
	}
	if (x.getSign() == -1) decr = x + k;
	else decr = x;
	delete[] pq;
	delete[] en;
}

void EncryptElement(Object^ _j) {
	int j = (int)_j;
	Int x, y, f, l, u;
	u.generationBySize(countDigitPos);
	u[countDigitPos - 1] = 0;
	for (int i = 0; (i < countDigitPos) && (j * countDigitPos + i < strText.length()); i += 3) {
		u[i / 3] = (unsigned char)strText[j * countDigitPos + i] * 65536;
		if (j * countDigitPos + i + 1 < strText.length()) {
			u[i / 3] += (unsigned char)strText[j * countDigitPos + i + 1] * 256;
			if (j * countDigitPos + i + 2 < strText.length())
				u[i / 3] += (unsigned char)strText[j * countDigitPos + i + 2];
		}
		u[i / 3] = BIGINT_MAX - u[i / 3] + lengthText;
	}
	l = LongPow(u, _encr, _sizeField);
	f = AlgEuclid(l, _sizeField, x, y);
	if (x.getSign() == -1)
		_encrData[j] = x + _sizeField;
	else
		_encrData[j] = x;
}
Int * Chipher::Encrypt(std::string & text, Int * encrData, int sizeStr, BackgroundWorker^ worker) {
	strText = text;
	_encr = encr;
	_sizeField = sizeField;
	lengthText = sizeStr;
	countDigitPos = (sizeField.getSize() - 1) * 3;
	int len = (sizeStr / countDigitPos) + 1;

	array<Thread^>^newThread;
	newThread = gcnew array<Thread^>(len);
	_encrData = new Int[len];
	for (int j = 0; j < len; j++) {
		newThread[j] = gcnew Thread(gcnew ParameterizedThreadStart(EncryptElement));
		newThread[j]->Start(j);
		worker->ReportProgress((int)((float)j / len * 100));
		if (worker->CancellationPending) {
			for (int i = 0; i <= j; i++)
				newThread[i]->Join();
			lengthText = 0;
			countDigitPos = 0;
			strText = "";
			_encr = Int();
			_sizeField = Int();
			delete[] _encrData;
			return NULL;
		}
	}
	for (int j = 0; j < len; j++) {
		newThread[j]->Join();
		encrData[j] = _encrData[j];
	}
	lengthText = 0;
	countDigitPos = 0;
	strText = "";
	_encr = Int();
	_sizeField = Int();
	delete[] _encrData;
	return encrData;
}

void DecryptElement(Object^ _j) {
	int j = (int)_j;
	Int x, y, f, l, u;
	int tmp3;
	u = _encrData[j];
	f = AlgEuclid(u, _sizeField, x, y);
	if (x.getSign() == -1)
		x = x + _sizeField;
	l = LongPow(x, _decr, _sizeField);
	for (int i = 0; i < (countDigitPos / 3); i++) {
		tmp3 = l[i];
		tmp3 = BIGINT_MAX - tmp3 + lengthText;
		strMemText[j] += (unsigned char)(tmp3 / 65536);
		strMemText[j] += (unsigned char)(tmp3 / 256);
		strMemText[j] += (unsigned char)(tmp3 % 256);
	}
}
std::string Chipher::Decrypt(Int * encrData, int sizeStr, BackgroundWorker^ worker) {
	_encrData = encrData;
	_decr = decr;
	_sizeField = sizeField;
	lengthText = sizeStr;
	std::string s = "";
	countDigitPos = (sizeField.getSize() - 1) * 3;
	int len = (sizeStr / countDigitPos) + 1;
	array<Thread^>^newThread;
	newThread = gcnew array<Thread^>(len);
	strMemText = new std::string[len];
	for (int j = 0; j < len; j++) {
		strMemText[j] = "";
		newThread[j] = gcnew Thread(gcnew ParameterizedThreadStart(DecryptElement));
		newThread[j]->Start(j);
		worker->ReportProgress((int)((float)j / len * 100));
		if (worker->CancellationPending) {
			for (int i = 0; i <= j; i++)
				newThread[i]->Join();
			lengthText = 0;
			countDigitPos = 0;
			_encrData = NULL;
			_decr = Int();
			_sizeField = Int();
			delete[] strMemText;
			return "";
		}
	}
	for (int j = 0; j < len; j++) {
		newThread[j]->Join();
		s += strMemText[j];
	}
	s.erase(sizeStr, countDigitPos * len - sizeStr);
	lengthText = 0;
	countDigitPos = 0;
	_encrData = NULL;
	_decr = Int();
	_sizeField = Int();
	delete[] strMemText;
	return s;
}

Chipher::Chipher(int _sizeKey) {
	sizeKey = 2;
	for (int i = 1; i < _sizeKey; i++)
		sizeKey += 2;
	encr = Int();
	decr = Int();
	sizeField = Int();
}
Chipher::~Chipher() {}
Chipher::Chipher(const Chipher & ch) {
	sizeKey = ch.sizeKey;
	encr = ch.encr;
	decr = ch.decr;
	sizeField = ch.sizeField;
}

void Chipher::setSizeKey(int _sizeKey) {
	sizeKey = _sizeKey;
}
void Chipher::setKey(Int _encr, Int _sizeField) {
	encr = _encr;
	sizeField = _sizeField;
}
std::string Chipher::getKey() {
	std::string res = encr.BigIntToString() + "." + sizeField.BigIntToString();
	return res;
}