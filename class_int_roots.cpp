#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

#include "class_int_division.cpp"

BigInt squareRootIteration(BigInt origin, BigInt currentApproximation){
	return divideByDigit(
		sumNaive(
			currentApproximation,
			divide(
				origin,
				currentApproximation
			)
		)
	, 2);
}

BigInt squareRoot(BigInt x){
	x.normalizeSize();
	//clog << "Counting square root from " << x << endl;
	if(x.m_bSign){
		//clog << "Negative number!" << endl;
		return BigInt(x.m_nExp); // 0
	}
	int size = x.size();
	if(size==1){
		return BigInt(x.m_nExp, 1, sqrt(x[0]));
	}
	BigInt y(x.m_nExp, size/2 + 3);

	// Choosing initial approximation

	if (size%2==0) {
		x.m_pCoeff.push_back(0);
		size++;
	}
	y[size/2]=sqrt(x[size-1])+1;

	if(!y){
		y[0]=1;
	}
	y.normalizeSize();
	//clog << "Начальное приближение:" << endl;
	//clog << y << endl;

	BigInt previousIteration = y;
	BigInt currentIteration = y;
	do {
		previousIteration = currentIteration;
		currentIteration = squareRootIteration(x, previousIteration);
		//clog << "Текущее приближение:" << endl;
		//clog << currentIteration << endl;
	}while(compareAbs(previousIteration, currentIteration) > 0);

	return previousIteration;
}

BigInt mthRootIteration(BigInt origin,BigInt currentApproximation,int m){
	BigInt raised = raiseToPower(
						currentApproximation,
						m-1
					);
	//clog << raised << endl;
	BigInt quotient =
			divide(
				origin,
				multiply(
					raised,
					m-1
				)
			);
	//cout << quotient << endl;
	return divideByDigit(multiply(
		sumSigned(
			currentApproximation,
			quotient
		),
	m-1), m);
}

BigInt mthRoot(BigInt x, int m, BigInt currentIteration){
	x.normalizeSize();
	//clog << "Counting " << m <<"-th root from " << x << endl;
	BigInt previousIteration = x;
	do {
		previousIteration = currentIteration;
		if(!previousIteration){
			previousIteration[0]=1;
		}
		currentIteration = mthRootIteration(x, previousIteration, m);
		if(!currentIteration){
			currentIteration[0]=1;
		}
		//clog << "Текущее приближение:" << endl;
		//clog << currentIteration << endl;
	}while(compareAbs(previousIteration, currentIteration) > 0);

	BigInt probeProduct = raiseToPower(previousIteration, m);

	int cmp = compareAbs(probeProduct, x);

	if(cmp==0){
		// Full mth power!
		return previousIteration;
	}

	BigInt theOne(x.m_nExp, 1, 1);

	if(cmp > 0){
		// Bigger than actual value
		do{
			previousIteration = subtractNaive(previousIteration, theOne);
			probeProduct = raiseToPower(previousIteration, m);
		}while(compareAbs(probeProduct, x)>0);
		return previousIteration;
	}

	// cmp < 0
	currentIteration = previousIteration;
	do{
		previousIteration = currentIteration;
		currentIteration = sumNaive(previousIteration, theOne);
		probeProduct = raiseToPower(currentIteration, m);
	}while(compareAbs(probeProduct, x)<0);
	return previousIteration;
}

BigInt mthRoot(BigInt x, int m){
	return mthRoot(x, m, x);
}
