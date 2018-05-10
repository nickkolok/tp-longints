#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int_division.cpp"

inline bool isEven(BigInt& x){
	return !(x[0]&1);
}


BigInt gcdNoCoeff(BigInt x, BigInt y){
	while(!(!x)) { // x != 0
		while(isEven(x)){
			x.divideByTwo();
		}
		while(isEven(y)){
			y.divideByTwo();
		}
		if(compareAbs(x,y) >= 0) {
			x = subtractNaive(x, y);
		} else {
			y = subtractNaive(y, x);
		}
	}
	return y;
}

BigInt findDivisor(BigInt dividend, BigInt upTo){
	if(isEven(dividend)){
		return BigInt(dividend.m_nExp, 1, 2);
	}
	BigInt rem(dividend.m_nExp);
	divideByDigit(dividend, 3, &rem);
	if(rem[0]==0){
		return BigInt(dividend.m_nExp, 1, 3);
	}

	BigInt the1(dividend.m_nExp, 1, 1);
	BigInt probe = BigInt(dividend.m_nExp, 1, 3);
	while(compareAbs(upTo, probe)>0){
		probe.addDigit(2);
		BigInt companion = probe;
		probe.addDigit(2);
		companion = multiply(companion, probe);
		probe.addDigit(2);
		BigInt gcd = gcdNoCoeff(companion, dividend);
		if(compareAbs(gcd, the1)>0){
			return gcd;
		}
	}
	return dividend;
}
