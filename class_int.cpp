#include <cstring>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

class BigInt {

public:
	vector<int> m_pCoeff; // Массив коэффициентов ("цифр" в m_nBase'-чной системе)
	int m_nExp; // степень числа 10
	int m_nBase; // 10 в степени m_nExp, основание системы счисления
	bool m_bSign; // знак числа


	size_t size(){
		return m_pCoeff.size();
	}

	void fillWith0(){
		size_t nSize = m_pCoeff.size();
		for(size_t i = 0; i < nSize; i++){
			m_pCoeff[i] = 0;
		}
	}

	BigInt(int nExp, int nPow = 1) {
		m_bSign = false; // +
		m_nExp = nExp;
		m_nBase = pow(10, nExp);
		m_pCoeff.resize(nPow);
		fillWith0();
	}

	void readFromStream(istream& sin){
		m_bSign = false;
		string str;
		sin >> str;
		if(str[0] == '-'){
			m_bSign = true;
			str[0] = '0';
		} else if (str[0] == '+'){
			str[0] = '0';
		}
		str = string(m_nExp + 1, '0') + str;
		size_t len = str.length();
		size_t nPow = len / m_nExp; // Эта длина уже учитывает "запасные" нули в начале строки, поэтому можно округлить до меньшего
		m_pCoeff.resize(nPow);
		fillWith0();
		for (size_t i = 0; i < nPow; i++) {
			m_pCoeff[i] = stol( str.substr(len - (i+1)*m_nExp, m_nExp) );
		}
	}

	BigInt(istream& sin, int nExp){
		m_nExp = nExp;
		m_nBase = pow(10, nExp);
		readFromStream(sin);
	}

	void writeToStream(ostream& sout){
		if(m_bSign){
			sout << "-";
		}
		int i = size() - 1;
		while (!m_pCoeff[i] && i) {
			// Пропускаем ведущие нули.
			// TODO: их вообще не должно быть
			i--;
		}
		sout << m_pCoeff[i];
		i--;
		for(; i >= 0 ; i--){
			sout << setw(m_nExp) << setfill('0') << m_pCoeff[i];
		}
	}

	void normalizeSize(){
		while(!m_pCoeff.back()){
			m_pCoeff.pop_back();
		}
	}

	int& operator[] (size_t i) {
		return m_pCoeff[i];
	}

	bool divideByTwo(){
		int s = size();
		int buf;
		for(int i = s - 1; i > 0; i--){
			buf = m_pCoeff[i] & 1;
			m_pCoeff[i]>>=1;
			if(buf){
				m_pCoeff[i-1]+=m_nBase;
			}
		}
		buf = m_pCoeff[0] & 1;
		m_pCoeff[0]>>=1;
		if(!m_pCoeff[s-1] && s-1){
			m_pCoeff.pop_back();
		}
		return buf;
	}

	bool operator ! (){
		int s = size() - 1;
		while(s){
			if(m_pCoeff[s]){
				return false;
			}
			s--;
		}
		return !m_pCoeff[0];
	}
};

BigInt sumNaive(BigInt x1, BigInt x2) {
	// TODO: check if x1 and x2 have equal exponents
	size_t size1 = x1.size(), size2 = x2.size();
	if (size2 > size1){
		return sumNaive(x2, x1);
	}

	BigInt sum(x1.m_nExp, size1);

	sum.m_pCoeff[0] = x1[0] + x2[0];

	for (size_t i = 1; i < size2; i++){
		sum[i] = x1[i] + x2[i];
		if(sum[i - 1] >= x1.m_nBase) {
			sum[i - 1] -= x1.m_nBase;
			sum[i]++;
		}
	}

	for (size_t i = size2; i < size1; i++){
		sum[i] = x1[i];
		if(sum[i - 1] >= x1.m_nBase) {
			sum[i - 1] -= x1.m_nBase;
			sum[i]++;
		}
	}


	if (sum[size1 - 1] >= x1.m_nBase){
		sum.m_pCoeff.push_back(1);
		sum[size1 - 1] -= x1.m_nBase;
	}

	return sum;
}

int compareAbs(BigInt a1, BigInt a2){
	// Returns 0, if equal
	// >0, if a1 is bigger than a2
	// <0 otherwise
	a1.normalizeSize();
	a2.normalizeSize();

	size_t size1 = a1.size(), size2 = a2.size();

	if( size1 > size2){
		return size1;
	}
	if( size2 > size1){
		return -size2;
	}

	//sizes of arrays are equal
	for (int i = size1 - 1; i>=0; i--){
		if (a1[i] > a2[i]) {
			return i + 1;
		}
		if (a1[i] < a2[i]) {
			return - i - 1;
		}
	}
	return 0;
}


BigInt subtractNaive(BigInt x1, BigInt x2) {
	// TODO: check if x1 and x2 have equal exponents

	// Assume x1 > x2

	size_t size1 = x1.size(), size2 = x2.size();

	BigInt sum(x1.m_nExp, size1);

	sum.m_pCoeff[0] = x1[0] - x2[0];

	for (size_t i = 1; i < size2; i++){
		sum[i] = x1[i] - x2[i];
		if(sum[i - 1] < 0) {
			sum[i - 1] += x1.m_nBase;
			sum[i]--;
		}
	}

	for (size_t i = size2; i < size1; i++){
		sum[i] = x1[i];
		if(sum[i - 1] < 0) {
			sum[i - 1] += x1.m_nBase;
			sum[i]--;
		}
	}

	return sum;
}

BigInt sumSigned(BigInt x1, BigInt x2){
	BigInt res(x1.m_nExp, max(x1.size(), x2.size()) + 1);
	if(!x1.m_bSign && !x2.m_bSign) {
		res = sumNaive(x1, x2);
		res.m_bSign = false;
		return res;
	}
	if(x1.m_bSign && x2.m_bSign) {
		res = sumNaive(x1, x2);
		res.m_bSign = true;
		return res;
	}
	if(x1.m_bSign && !x2.m_bSign) {
		return sumSigned(x2, x1);
	}
	// So, x1 is positive and x2 is negative
	int cmp = compareAbs(x1, x2);
	if(!cmp) {
		res.m_pCoeff.resize(1);
		res[0] = 0;
		return res;
	}
	if(cmp < 0) {
		// |x2| > |x1|
		res = subtractNaive(x2, x1);
		res.m_bSign = true;
		return res;
	}
	// cmp > 0
	res = subtractNaive(x1, x2);
	res.m_bSign = false;
	return res;
}

ostream& operator << (ostream& sout, BigInt&b) {
	b.writeToStream(sout);
	return sout;
}
