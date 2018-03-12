#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int.cpp"

BigInt square(BigInt x){
	int size = x.size();
	BigInt y(x.m_nExp, 2*size + 1);
	for (int i = 0; i < size; i++){
		y[2*i    ] = x[i]   * x[i];
		// TODO: which is faster: % / or while -- ?
		y[2*i + 1] = y[2*i] / y.m_nBase;
		y[2*i    ] = y[2*i] % y.m_nBase;
	}
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			y[i + j    ] += 2 * x[i] * x[j];
			y[i + j + 1] += y[i + j] / y.m_nBase;
			y[i + j    ]  = y[i + j] % y.m_nBase;
		}
	}
	return y;
}


BigInt multiply(BigInt x1, BigInt x2){
	int size1 = x1.size(), size2 = x2.size();
	BigInt y(x1.m_nExp, size1 + size2 + 1);
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			y[i + j    ] += x1[i] * x2[j];
			y[i + j + 1] += y[i + j] / y.m_nBase;
			y[i + j    ]  = y[i + j] % y.m_nBase;
		}
	}
	y.m_bSign = x1.m_bSign ^ x2.m_bSign;
	return y;
}

BigInt raiseToPower(BigInt exp, BigInt pow){
	BigInt multiplier = exp; // TODO: is exp destroyed?
	BigInt restPow = pow; // TODO: is pow destroyed?
	BigInt result(exp.m_nExp);
	result[0] = 1;
	while (!(!restPow)){
		cout << "! " << restPow << endl;
		if(restPow.divideByTwo()){
			cout << result << "  " << multiplier << endl;
			result = multiply(result, multiplier);
			cout << result.size() << "   " << result << endl;
		}
		multiplier = square(multiplier);
		cout << multiplier << endl;
	}
	return result;
}

