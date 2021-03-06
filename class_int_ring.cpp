#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int_gcd.cpp"

BigInt ringReverse(BigInt modulo, BigInt x){
	BigInt rev(x.m_nExp);
	BigInt divisor = gcd(x, modulo, &rev);
	if(compareAbs(divisor,BigInt(x.m_nExp, 1, 1))){ // divisor != 1
		return BigInt(x.m_nExp, 1, 0); // 0
	}
	BigInt remainder(x.m_nExp);
	divide(rev, modulo, &remainder);

	if(rev.m_bSign){ // rev < 0
		remainder = subtractNaive(modulo, remainder);
	}
	return remainder;
}

BigInt sumRing(BigInt x, BigInt y, BigInt modulo){
	BigInt t = sumNaive(x, y);
	if(compareAbs(t, modulo) > 0) {
		return subtractNaive(t, modulo);
	}
	return t;
}

BigInt subtractRing(BigInt x, BigInt y, BigInt modulo){
	BigInt t = subtractSigned(x, y);
	if(t.m_bSign) {
		return subtractNaive(modulo, t);
	}
	return t;
}

BigInt multiplyRingDirect(BigInt x, BigInt y, BigInt modulo){
	BigInt t = multiply(x, y);
	BigInt remainder(x.m_nExp);
	divide(t, modulo, &remainder);
	return remainder;
}

BigInt raiseToPowerRingDirect(BigInt exp, BigInt pow, BigInt modulo){
	BigInt multiplier = exp;
	BigInt restPow = pow;
	BigInt result(exp.m_nExp);
	result[0] = 1;
	while (!(!restPow)){
		if(restPow.divideByTwo()){
			divide(multiply(result, multiplier), modulo, &result);
		}
		divide(square(multiplier), modulo, &multiplier);
	}
	return result;
}

void createMontgomery(BigInt N, BigInt& R, BigInt& Ns, BigInt& w){
	N.normalizeSize();
	R = BigInt(N.m_nExp, N.size()+1);
	R[N.size()] = 1;
	Ns = subtractNaive(R, ringReverse(R, N));
	BigInt R2(N.m_nExp, 2*N.size()+1);
	R2[2*N.size()] = 1;
	divide(R2, N, &w);
}


BigInt multiplyDigits(BigInt x1, BigInt x2, int digits){
	int size1 = x1.size(), size2 = x2.size();
	BigInt y(x1.m_nExp, digits + 1);
	for (int i = 0; i < size1; i++) {
		for (int j = 0; (j < digits-i)&&(j < size2); j++) {
			y[i + j    ] += x1[i] * x2[j];
			y[i + j + 1] += y[i + j] / y.m_nBase;
			y[i + j    ]  = y[i + j] % y.m_nBase;
		}
	}
	y.m_bSign = x1.m_bSign ^ x2.m_bSign;
	return y;
}

BigInt phiMontgomery(BigInt x, BigInt y, BigInt& N, BigInt& R, BigInt& Ns){
	int rs = R.size()-1;
	BigInt xy = multiply(x,y);
	BigInt m = multiplyDigits(xy, Ns,rs);
	m.m_pCoeff.resize(rs);
	BigInt t = sumNaive(xy,  multiply(m,N));
	t.pop_front(R.size()-1);
	if(compareAbs(t, N) > 0) {
		t = subtractNaive(t, N);
	}
	//cout << "phi(" << x << ", " << y << ") = " << t << endl;
	return t;

}

BigInt multiplyRingMontgomery(
	BigInt x, BigInt y, BigInt& N, BigInt& R, BigInt& Ns, BigInt& w
){
	return (
		phiMontgomery(w, phiMontgomery(
				x, y
				, N, R, Ns
			), N, R, Ns
		)
	);
}

BigInt multiplyRing(BigInt x, BigInt y, BigInt N){
	if(isEven(N) || N[0]%5==0){
		return multiplyRingDirect(x,y,N);
	}
	BigInt R(4), Ns(4), w(4);
	createMontgomery(N, R, Ns, w);
	//cout << "N = " << N << "  R = " << R << "  N' = " << Ns << "  w = " << w << endl;
	return multiplyRingMontgomery(x, y, N, R, Ns, w);
}


BigInt powerRingMontgomery(
	BigInt x, BigInt y, BigInt& N, BigInt& R, BigInt& Ns, BigInt& w
){
	BigInt the1 = BigInt(x.m_nExp, 1 ,1);
	BigInt result = phiMontgomery(the1, w, N, R, Ns);
	BigInt multiplier = phiMontgomery(x, w, N, R, Ns);;
	while (!(!y)){
		if(y.divideByTwo()){
			result = phiMontgomery(result, multiplier, N, R, Ns);
		}
		multiplier = phiMontgomery(multiplier, multiplier, N, R, Ns);
	}
	return phiMontgomery(result, the1, N, R, Ns);
}

BigInt powerRing(BigInt x, BigInt y, BigInt N){
	if(isEven(N) || N[0]%5==0){
		return raiseToPowerRingDirect(x,y,N);
	}
	BigInt R(4), Ns(4), w(4);
	createMontgomery(N, R, Ns, w);
	//cout << "N = " << N << "  R = " << R << "  N' = " << Ns << "  w = " << w << endl;
	return powerRingMontgomery(x, y, N, R, Ns, w);
}
