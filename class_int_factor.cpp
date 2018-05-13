#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int_roots.cpp"

class Factors : public vector<BigInt> {
};



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
		x.normalizeSize();
		y.normalizeSize();
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

ostream& operator << (ostream& sout, Factors f){
	int s = f.size() - 1;
	for(int i = 0; i < s; i++){
		sout << f[i] << " * ";
	}
	sout << f[s];
	return sout;
}

Factors findFactors(BigInt n){
	Factors factors;
	int nExp = n.m_nExp;
	BigInt big2 = BigInt(nExp, 1, 2);
	BigInt big1 = BigInt(nExp, 1, 1);
	while(isEven(n)){
		factors.push_back(big2);
		n.divideByTwo();
	}
	BigInt rem(nExp), quot3(nExp);
	do{
		quot3=divideByDigit(n, 3, &rem);
		if(!rem){
			factors.push_back(BigInt(nExp, 1, 3));
			n = quot3;
		} else {
			break;
		}

	}while(true);
	clog << "n became " << n << endl;
	BigInt root3 = mthRoot(n, 3);

	clog << "Cubic root: " << root3 << endl;

	BigInt border = BigInt(nExp, 1, 5);
	while(compareAbs(border, root3)<=0){
		clog << "border = " << border << endl;

		BigInt brick = border;

		// TODO: honest multiplyByDigit
		BigInt probe = sumNaive(square(brick),multiply(brick,big2));
		brick.addDigit(6);
		while(probe.size() < n.size()){
			probe = multiply(
				probe,
				sumNaive(square(brick),multiply(brick,big2))
			);
			brick.addDigit(6);
		}

		clog << "probe = " << probe << endl;
		BigInt divisor = gcdNoCoeff(n, probe);
		clog << divisor << endl;

		while(compareAbs(divisor, big1) > 0){
			clog << "Found a divisor: " << divisor << endl;
			n = divide(n, divisor);
			clog << "n became " << n << endl;
			root3 = mthRoot(n, 3);
			clog << "Cubic root became: " << root3 << endl;
			for(BigInt probePrime = border; compareAbs(divisor, big1) > 0; probePrime.addDigit(2)){
				BigInt probeQuot = divide(divisor, probePrime, &rem);
				while(!rem){
					divisor = probeQuot;
					factors.push_back(probePrime);
					probeQuot = divide(divisor, probePrime, &rem);
				}
			}
			divisor = gcdNoCoeff(n, probe);
		}
		border = brick;

/*
		while(compareAbs(divisor, big1) > 0){
			for(BigInt probePrime = border; compareAbs(probePrime, brick) < 0; probePrime.addDigit(2)){
				BigInt probeQuot = divide(divisor, probePrime, rem);
				while(!rem){
					divisor = probeQuot;
					n = divide(n,
				}
			}
			root3 = mthRoot(n, 3);
		}
*/
	}


	return factors;
}


