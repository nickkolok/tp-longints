#include <iostream>
#include <cstdlib>

using namespace std;

#include "class_int_gcd.cpp"

int main(){
	cout << "Введите первое число:" << endl;
	BigInt a(cin, 4);

	cout << "Введите второе число:" << endl;
	BigInt b(cin, 4);


	BigInt u(4);
	BigInt v(4);


	BigInt c = gcd(a,b, &u, &v);
	cout << "НОД(" << endl << a << endl << "," << endl << b << endl
		 << ") = " << endl << c << endl;

	cout << "Коэффициенты линейного представления:" << endl
		 << u << endl << v << endl;

	return 0;
}
