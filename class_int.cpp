#include <cstring>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

class BigInt {

	// http://www.cplusplus.com/reference/vector/vector/reserve/

public:
	vector<int> m_pCoeff; // Массив коэффициентов
	int m_nExp; // степень числа 10
	int m_nBase; // 10 в степени m_nExp
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

	BigInt(int nExp, int nPow) {
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
		str = string(m_nExp + 1, '0') + str; // +1 это с запасом // TODO: а надо ли?
		size_t len = str.length();
		size_t nPow = len / m_nExp; // Эта длина уже учитывает "запасные" нули в начале строки, поэтому можно округлить до меньшего
		// http://www.cplusplus.com/reference/string/stol/
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
		while (!m_pCoeff[i]) {
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

	~BigInt(){
		//delete[] m_pCoeff;
	}

	int& operator[] (size_t i) {
		return m_pCoeff[i];
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
		if(sum[i - 1] > x1.m_nBase) {
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
	// >0, if a is bigger than b
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
