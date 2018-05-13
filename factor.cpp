#include <iostream>
#include <cstdlib>

using namespace std;

#include "class_int_factor.cpp"

int main(){
	cout << "Введите число:" << endl;
	BigInt a(cin, 4);

	Factors f = findFactors(a);
	cout << a << " = " << f << endl;

	return 0;
}
