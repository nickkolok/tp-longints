#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int_square.cpp"

BigInt divideByDigit(BigInt x, int d, BigInt* r = 0){
	int s = x.size();
	BigInt y(x.m_nExp, s);

	for(int i = s - 1; i > 0; i--){
		y[i    ]  =  x[i] / d;
		x[i - 1] += (x[i] % d) * x.m_nBase;
	}
	y[0]  =  x[0] / d;

	y.m_bSign = x.m_bSign;
	if (r) {
		*r = BigInt(x.m_nExp, 1, x[0] % d);
	}
	return y;
}

int normGlukhov(BigInt& x, BigInt& y) {
	int firstDigit = y[y.size() - 1];
	size_t base = y.m_nBase;
	if (firstDigit <= base/2) {
		size_t scaling = base/2 / (firstDigit+1) - 1;
		BigInt ynew(y.m_nExp);
		do{
			scaling++;
			ynew = multiply(y, scaling);
			ynew.normalizeSize();
		} while (ynew[ynew.size() - 1] <= base / 2 );
		x = multiply(x, scaling);
		y = ynew;
		return scaling;
	}
	return 1;
}



BigInt divide(BigInt x, BigInt y, BigInt* r = 0){
	x.normalizeSize();
	y.normalizeSize();

	if (y.size() == 1){
		return divideByDigit(x, y[0], r);
	}

	BigInt result(x.m_nExp);

	int cmp = compareAbs(x, y);
	if (cmp < 0) {
		if(r){
			*r = x;
		}
		return result;
	}
	if (cmp == 0) {
		*r = result;
		return BigInt(x.m_nExp, 1, 1);
	}

	// Now we know the following:
	// 1) y > x
	// 2) y has at least two digits
	// 3) Thus, x has at least two digits

	size_t scaling = normGlukhov(x, y);
	x.m_pCoeff.push_back(0);
	y.m_pCoeff.push_back(0);

	size_t sizeResult = x.size() - y.size() + 1;
	result = BigInt(x.m_nExp, sizeResult);

	int* p_x = x;
	int* p_y = y;
	int ys = y.size();
	int firstDigit = y[y.size() - 2];
	int base = y.m_nBase;

	BigInt ytest(x.m_nExp);

	for (int shift = sizeResult - 1; shift >= 0; shift--){
		if (compareAbsPointers(p_y, p_x + shift, ys) > 0){
			result[shift] = 0;
			continue;
		}
		int q;
		if (x[ys-1 + shift] == firstDigit) {
			q = base - 1;
		} else {
			q = max((x[ys - 1 + shift]*base + x[ys - 2 + shift]) / firstDigit, 2);
		}
		ytest = multiply(y, q - 2);
		result[shift] = q - 2;
		subtractNaivePointers(p_x + shift, p_x + shift, ytest, ys, ys, base);

		if (compareAbsPointers(p_y, p_x + shift, ys) <= 0){
			result[shift]++;
			subtractNaivePointers(p_x + shift, p_x + shift, p_y, ys, ys, base);
			if (compareAbsPointers(p_y, p_x + shift, ys) <= 0){
				result[shift]++;
				subtractNaivePointers(p_x + shift, p_x + shift, p_y, ys, ys, base);
			}
		}

	}

	clog << "Finished main loop!" << endl;

	if(r) {
		if (scaling != 1) {
			*r = divideByDigit(x, scaling);
		} else {
			*r = x;
		}
	}

	return result;
}
