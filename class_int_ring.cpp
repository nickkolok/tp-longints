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
	//clog << "Started createMontgomery()" << endl;
	N.normalizeSize();
	R = BigInt(N.m_nExp, N.size()+1);
	R[N.size()] = 1;
//	Ns = ringReverse(N, R);
	Ns = subtractNaive(R, ringReverse(R, N));
	BigInt R2(N.m_nExp, 2*N.size()+1);
	R2[2*N.size()] = 1;
	divide(R2, N, &w);
	//clog << "Finished createMontgomery()" << endl;
}

BigInt phiMontgomery(BigInt x, BigInt y, BigInt& N, BigInt& R, BigInt& Ns){
	BigInt t(N.m_nExp, 1, 0);
	x.normalizeSize();
	int s = x.size();
	for(int i = 0; i < s; i++){
		BigInt xiy = multiply(y, x[i]);
		int u=(xiy[0]+t[0])%x.m_nBase;
		cout << "u = " << u << endl;
		int v = (u*Ns[0])%x.m_nBase;
		cout << "v = " << v << endl;
		t = sumSigned(t, xiy);
		//t = subtractSigned(xiy, t);
		t = sumSigned(t, multiply(N,v));
		cout << "t = " << t << "  x_i y = " << xiy << "  vN = ";
		multiply(N,v).writeToStream(cout);
		cout << " " << endl;
		t.pop_front();
	}
	if (compareAbs(t, N) > 0){
		cout << "t < N" << endl;
		t = subtractNaive(t, N);
		cout << "t = " << t << endl;
	}
	cout << "phi(" << x << ", " << y << ") = " << t << endl;
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
	BigInt R(4), Ns(4), w(4);
	createMontgomery(N, R, Ns, w);
	cout << "N = " << N << "  R = " << R << "  N' = " << Ns << "  w = " << w << endl;
	return multiplyRingMontgomery(x, y, N, R, Ns, w);
}
