#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int_division.cpp"

inline bool isEven(BigInt& x){
	return !(x[0]&1);
}

inline void gcd_adjust(BigInt& x, BigInt& y, BigInt& A, BigInt& B){
	if(isEven(A) && isEven(B)){
		A.divideByTwo();
		B.divideByTwo();
	} else {
		A = sumSigned(A,y);
		A.divideByTwo();
		B = subtractSigned(B,x);
		B.divideByTwo();
	}
}

BigInt gcd(BigInt a, BigInt b, BigInt* u=0, BigInt* v=0){
	BigInt g(a.m_nExp, 1, 1);
	BigInt A=g, D=g;
	BigInt B=BigInt(a.m_nExp, 1), C=B;
	while(isEven(a) && isEven(b)){
		a.divideByTwo();
		b.divideByTwo();
		g = multiply(g, 2);
	}
	BigInt x=a, y=b;



	while(!(!x)) { // x != 0
		while(isEven(x)){
			x.divideByTwo();
			gcd_adjust(a, b, A, B);
		}
		while(isEven(y)){
			y.divideByTwo();
			gcd_adjust(a, b, C, D);
		}
		if(compareAbs(x,y) >= 0) {
			x = subtractNaive(x, y);
			A = subtractSigned(A, C);
			B = subtractSigned(B, D);
		} else {
			y = subtractNaive(y, x);
			C = subtractSigned(C, A);
			D = subtractSigned(D, B);
		}
	}
	if(u){
		*u=C;
	}
	if(v){
		*v=D;
	}
	return multiply(g,y);
}
