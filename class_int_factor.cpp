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

		//clog << "probe = " << probe << endl;
		BigInt divisor = gcdNoCoeff(n, probe);
		//clog << divisor << endl;

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
	}
	n.normalizeSize();
	BigInt big23 = BigInt(nExp, 1, 23);
	if(compareAbs(n, big23) < 0){
		if(compareAbs(n, big1) > 0){
			factors.push_back(n);
		}
		return factors;
	}

	//clog << "Starting Leman..." << endl;

	BigInt kn(nExp);
	BigInt root6u = mthRoot(n, 6, root3); //TODO: square root?
	root6u.addDigit(1);
	for(BigInt k(nExp,1,1); compareAbs(k,root3)<=0; k.addDigit(1)){
		clog << "k = " << k << endl;
		kn = sumNaive(kn, n);
		BigInt sqrt_kn_2 = multiply(squareRoot(kn),2);
		BigInt kn_4 = multiply(kn, 4);
		kn_4.normalizeSize();
		BigInt upTo = sumNaive(
			sqrt_kn_2,
			divide(
				root6u,
				subtractNaive(
					multiply(
						squareRoot(k),
						4
					),
					1
				)
			)
		);
		upTo.addDigit(4);
		upTo.normalizeSize();
		clog << "For a from " << sqrt_kn_2 << " to " << upTo << endl;
		for(BigInt a = sqrt_kn_2; compareAbs(a, upTo) < 0; a.addDigit(1)){
			clog << "a = " << a << endl;
			BigInt c = subtractSigned(
				square(a),
				kn_4
			);
			//c.normalizeSize();
			if (c.m_bSign){
				continue;
			}
			BigInt b = squareRoot(c);
			BigInt b2 = square(b);
			//clog << "b = " << b << endl;
			//clog << "b2 = " << b2 << endl;
			//clog << "c = " << c << endl;
			if(!compareAbs(b2, c)){
				//clog << "Good a!" << endl;
				BigInt p = gcdNoCoeff(sumNaive(a, b), n);
				//clog << p << " " << compareAbs(p, big1) << " " << compareAbs(p, n) << endl;
				if(compareAbs(p, big1) && compareAbs(p, n)){
					factors.push_back(p);
					factors.push_back(divide(n,p));
					return factors;
				}
				p = gcdNoCoeff(subtractSigned(a, b), n);
				//clog << p << endl;
				if(compareAbs(p, big1) && compareAbs(p, n)){
					factors.push_back(p);
					factors.push_back(divide(n,p));
					return factors;
				}
			}
		}
	}

	factors.push_back(n);
	return factors;
}


