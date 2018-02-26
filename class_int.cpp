#include <cstring>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <vector>

using namespace std;

class BigInt {
	int m_nExp; // степень числа 10

	// http://www.cplusplus.com/reference/vector/vector/reserve/

	vector<int> m_pCoeff; // Массив коэффициентов
public:
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
		m_pCoeff.reserve(nPow);
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
		m_pCoeff.reserve(nPow);
		fillWith0();
		for (size_t i = 0; i < nPow; i++) {
			m_pCoeff[i] = stol( str.substr(len - (i+1)*m_nExp, m_nExp) );
		}
	}

	BigInt(istream& sin, int nExp){
		m_nExp = nExp;
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
