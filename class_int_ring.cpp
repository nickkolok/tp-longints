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
