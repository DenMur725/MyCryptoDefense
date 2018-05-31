#include "BigInt.h"
BigInt::BigInt(int _sizeMem) {
	size = 1;
	sizeMem = _sizeMem;
	sign = 1;
	mem = new int[sizeMem];
	for (int i = 0; i < sizeMem; i++)
		mem[i] = 0;
}
BigInt::BigInt(const BigInt & b) {
	size = b.size;
	sizeMem = b.sizeMem;
	sign = b.sign;
	mem = new int[sizeMem];
	for (int i = 0; i < sizeMem; i++)
		mem[i] = b.mem[i];
}
BigInt::BigInt(const std::string & st) {
	std::string str = st;
	sizeMem = st.length();
	sign = 1;
	int i = 0;
	if (st[0] == '-') {
		sign = -1;
		sizeMem--;
		str.erase(0, 1);
	}
	int len = sizeMem;
	sizeMem = (sizeMem / 9);
	if (len % 9 != 0)
		sizeMem++;
	size = sizeMem;
	mem = new int[sizeMem];
	for (int i = 0; (i < size - 1) || ((i < size) && (len % 9 == 0)); i++) {
		mem[i] = 0;
		for (int j = 9; j > 0; j--)
			mem[i] = 10 * mem[i] + (st[len - i * 9 - j] - '0');
	}
	if (len % 9 != 0) {
		mem[size - 1] = 0;
		for (int i = 0; i < len % 9; i++)
			mem[size - 1] = 10 * mem[size - 1] + (st[i] - '0');
	}
}
BigInt::~BigInt() {
	delete[] mem;
}
BigInt & BigInt::operator = (const BigInt & b) {
	if (sizeMem != b.sizeMem) {
		if (sizeMem != 0)
			delete[] mem;
		sizeMem = b.sizeMem;
		mem = new int[sizeMem];
	}
	size = b.size;
	sign = b.sign;
	for (int i = 0; i < sizeMem; i++)
		mem[i] = b.mem[i];
	return *this;
}
BigInt & BigInt ::operator = (const __int64 & b) {
	if (sizeMem != 0)
		delete[] mem;
	sign = 1;
	if (b < 0)
		sign = -1;
	size = 1;
	if (b >= BIGINT_MAX) {
		size++;
		if (b >(BIGINT_MAX * BIGINT_MAX))
			size++;
	}
	sizeMem = size;
	mem = new int[sizeMem];
	mem[0] = b % BIGINT_MAX;
	if (size == 2)
		mem[1] = b / BIGINT_MAX;
	if (size == 3) {
		long long tmp = b / BIGINT_MAX;
		mem[1] = tmp % BIGINT_MAX;
		mem[2] = tmp / BIGINT_MAX;
	}
	return *this;
}

char BigInt::operator < (BigInt & b) {
	if (sign * b.sign == -1)
		if (sign == -1) return 1;
		else return 0;
		if ((sign == -1) && (b.sign == -1)) {
			sign = 1;
			b.sign = 1;
			int res = (*this) > b;
			sign = -1;
			b.sign = -1;
			return res;
		}
		if (size < b.size) return 1;
		if (size > b.size) return 0;
		for (int i = size - 1; i >= 0; i--)
			if (mem[i] != b.mem[i])
				if (mem[i] < b.mem[i])
					return 1;
				else return 0;
				return 0;
}
char BigInt::operator > (BigInt & b) {
	if (sign * b.sign == -1)
		if (sign == -1) return 0;
		else return 1;
		if ((sign == -1) && (b.sign == -1)) {
			sign = 1;
			b.sign = 1;
			int res = (*this) < b;
			sign = -1;
			b.sign = -1;
			return res;
		}
		if (size > b.size) return 1;
		if (size < b.size) return 0;
		for (int i = size - 1; i >= 0; i--)
			if (mem[i] != b.mem[i])
				if (mem[i] > b.mem[i])
					return 1;
				else return 0;
				return 0;
}
char BigInt::operator <= (BigInt & b) {
	if (sign * b.sign == -1)
		if (sign == -1) return 1;
		else return 0;
		if ((sign == -1) && (b.sign == -1)) {
			sign = 1;
			b.sign = 1;
			int res = (*this) >= b;
			sign = -1;
			b.sign = -1;
			return res;
		}
		if (size < b.size) return 1;
		if (size > b.size) return 0;
		for (int i = size - 1; i >= 0; i--)
			if (mem[i] != b.mem[i])
				if (mem[i] < b.mem[i])
					return 1;
				else return 0;
				return 1;
}
char BigInt::operator >= (BigInt & b) {
	if (sign * b.sign == -1)
		if (sign == -1) return 0;
		else return 1;
		if ((sign == -1) && (b.sign == -1)) {
			sign = 1;
			b.sign = 1;
			int res = (*this) <= b;
			sign = -1;
			b.sign = -1;
			return res;
		}
		if (size > b.size) return 1;
		if (size < b.size) return 0;
		for (int i = size - 1; i >= 0; i--)
			if (mem[i] != b.mem[i])
				if (mem[i] > b.mem[i])
					return 1;
				else return 0;
				return 1;
}
char BigInt::operator == (BigInt & b) {
	if (sign * b.sign == -1)
		return 0;
	if (size != b.size) return 0;
	for (int i = size - 1; i >= 0; i--)
		if (mem[i] != b.mem[i])
			return 0;
	return 1;
}
char BigInt::operator != (BigInt & b) {
	if (sign * b.sign == -1)
		return 1;
	if (size != b.size) return 1;
	for (int i = size - 1; i >= 0; i--)
		if (mem[i] != b.mem[i])
			return 1;
	return 0;
}
int & BigInt::operator [] (int k) {
	return mem[k];
}

int BigInt::getSize() {
	return size;
}
int BigInt::getSign() {
	return sign;
}

int BigInt::realSize() {
	int len = 0;
	for (int i = size - 1; i >= 0; i--)
		if (mem[i] != 0) {
			len = i + 1;
			break;
		}
	return len;
}
void BigInt::removingZeros() {
	int len = this->realSize();
	if (len == 0) len = 1;
	BigInt tmp(len);
	tmp.size = tmp.sizeMem;
	tmp.sign = sign;
	for (int i = 0; i < tmp.size; i++)
		tmp[i] = mem[i];
	*this = tmp;
}

std::string BigInt::BigIntToString() {
	std::string st = "";
	if ((size == 1) && (mem[0] == 0))
		return "0";
	int tmp;
	for (int i = 0; i < size - 1; i++) {
		tmp = mem[i];
		for (int j = 0; j < 9; j++) {
			st = (char)((tmp % 10) + '0') + st;
			tmp /= 10;
		}
	}
	tmp = mem[size - 1];
	while (tmp > 0) {
		st = char((tmp % 10) + '0') + st;
		tmp /= 10;
	}
	if (sign == -1)
		st = '-' + st;
	return st;
}
__int64 BigInt::BigIntToInt() {
	__int64 res = 0;
	for (int i = size - 1; i >= 0; i--)
		res = res * BIGINT_MAX + mem[i];
	res *= sign;
	return res;
}
void BigInt::generationBySize(int _size) {
	*this = BigInt(_size);
	mem[_size - 1] = 1;
	size = _size;
}

BigInt BigInt::operator + (BigInt & b) {
	if (size < b.size)
		return b + (*this);
	BigInt res(size + 1);
	if ((sign > 0) && (b.sign < 0)) {
		b.sign = 1;
		res = (*this) - b;
		b.sign = -1;
		return res;
	}
	if ((sign < 0) && (b.sign > 0)) {
		sign = 1;
		res = b - (*this);
		sign = -1;
		return res;
	}
	if ((sign < 0) && (b.sign < 0))
		res.sign = -1;
	for (int i = 0; i < b.size; i++) {
		res.mem[i] += (mem[i] + b.mem[i]);
		res.mem[i + 1] += (res.mem[i] / BIGINT_MAX);
		res.mem[i] %= BIGINT_MAX;
	}
	for (int i = b.size; i < size; i++) {
		res.mem[i] += mem[i];
		res.mem[i + 1] = (res.mem[i] / BIGINT_MAX);
		res.mem[i] %= BIGINT_MAX;
	}
	res.size = size + 1;
	res.removingZeros();
	return res;
}
BigInt BigInt::operator - (BigInt & b) {
	BigInt res;
	if ((sign > 0) && (b.sign < 0)) {
		b.sign = 1;
		res = (*this) + b;
		b.sign = -1;
		return res;
	}
	if ((sign < 0) && (b.sign > 0)) {
		sign = 1;
		res = (*this) + b;
		sign = -1;
		res.sign = -1;
		return res;
	}
	if ((sign < 0) && (b.sign < 0)) {
		b.sign = 1;
		sign = 1;
		res = (*this) - b;
		res.sign = -1;
		b.sign = -1;
		sign = -1;
		return res;
	}
	if ((*this) < b) {
		res = b - (*this);
		res.sign = -1;
		return res;
	}
	res = *this;
	for (int i = 0; i < b.size; i++) {
		res.mem[i] -= b.mem[i];
		if (res.mem[i] < 0) {
			res.mem[i + 1]--;
			res.mem[i] += BIGINT_MAX;
		}
	}
	for (int i = b.size; i < (size - 1); i++)
		if (res.mem[i] < 0) {
			res.mem[i + 1]--;
			res.mem[i] += BIGINT_MAX;
		}
	res.removingZeros();
	return res;
}
BigInt BigInt::operator * (BigInt & b) {
	int len = size + b.size + 1;
	BigInt res(len);
	res.sign = sign * b.sign;
	unsigned __int64 tmp;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < b.size; j++) {
			tmp = res.mem[i + j] + ((unsigned __int64)mem[i] * b.mem[j]);
			res.mem[i + j + 1] += tmp / BIGINT_MAX;
			res.mem[i + j] = tmp % BIGINT_MAX;
		}
	res.size = len;
	res.removingZeros();
	return res;
}
BigInt BigInt::operator / (BigInt & b) {
	int sg1 = sign, sg2 = b.sign;
	sign = 1;
	b.sign = 1;
	if ((*this) < b) {
		BigInt r;
		return r;
	}
	int k = size - b.size;
	BigInt tmp(size), res(k + 1), _b(size), tmp_b(size + 1);
	tmp.size = size;
	res.size = k + 1;
	_b.size = size;
	tmp_b.size = size;
	for (int i = k; i < size; i++)
		tmp.mem[i] = b.mem[i - k];
	unsigned __int64 p;
	BigInt tmp_(size + 1);
	unsigned __int64 l, r, c;
	while (1) {
		l = 0;
		r = BIGINT_MAX - 1;
		while (l <= r) {
			c = (l + r) / 2;
			for (int i = k; i <= size; i++) {
				tmp_b.mem[i] = 0;
				tmp_.mem[i] = 0;
			}
			tmp_b.size = size;
			for (int i = 0; i < size - k; i++) {
				p = tmp_.mem[i + k] + ((unsigned __int64)tmp.mem[i + k] * c);
				tmp_.mem[i + k + 1] += p / BIGINT_MAX;
				tmp_.mem[i + k] = p % BIGINT_MAX;
			}
			for (int i = k; i < size; i++) {
				p = tmp_b.mem[i] + ((unsigned __int64)tmp_.mem[i] + _b.mem[i]);
				tmp_b.mem[i + 1] = p / BIGINT_MAX;
				tmp_b.mem[i] = p % BIGINT_MAX;
			}
			tmp_b.mem[size] += tmp_.mem[size];
			if (tmp_b.mem[size] != 0)
				tmp_b.size++;
			if (tmp_b >(*this))
				r = c - 1;
			else
				l = c + 1;
		}
		for (int i = k; i <= size; i++) {
			tmp_b.mem[i] = 0;
			tmp_.mem[i] = 0;
		}
		for (int i = 0; i < size - k; i++) {
			p = tmp_.mem[i + k] + ((unsigned __int64)tmp.mem[i + k] * r);
			tmp_.mem[i + k + 1] += p / BIGINT_MAX;
			tmp_.mem[i + k] = p % BIGINT_MAX;
		}
		for (int i = k; i < size; i++) {
			p = tmp_b.mem[i] + ((unsigned __int64)tmp_.mem[i] + _b.mem[i]);
			tmp_b.mem[i + 1] = p / BIGINT_MAX;
			tmp_b.mem[i] = p % BIGINT_MAX;
		}
		tmp_b.mem[size] += tmp_.mem[size];
		for (int i = k; i < size; i++)
			_b.mem[i] = tmp_b.mem[i];
		res.mem[k] = r;
		k--;
		if (k < 0) break;
		for (int i = k; i < tmp.size; i++)
			tmp.mem[i] = tmp.mem[i + 1];
		tmp.mem[b.size + k] = 0;
		tmp.size--;
	}
	res.removingZeros();
	sign = sg1;
	b.sign = sg2;
	res.sign = sign * b.sign;
	return res;
}
BigInt BigInt::operator % (BigInt & b) {
	return (*this) - (b * ((*this) / b));
}