#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int_square.cpp"

BigInt divideByDigit(BigInt x, int d){
	int s = x.size();
	BigInt y(x.m_nExp, s);

	for(int i = s - 1; i > 0; i--){
		y[i    ]  =  x[i] / d;
		x[i - 1] += (x[i] % d) * x.m_nBase;
	}
	y[0]  =  x[0] / d;

	y.m_bSign = x.m_bSign;
	return y;
}
