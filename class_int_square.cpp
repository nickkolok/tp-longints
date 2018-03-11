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
