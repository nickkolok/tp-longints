#include <iostream>
#include <cstdlib>

using namespace std;

#include "class_int_division.cpp"

int main(){
	cout << "Введите делимое" << endl;
	BigInt b1(cin, 4);

	cout << "Введите делитель" << endl;
	BigInt b2(cin, 4);

	BigInt r(4);
	BigInt b3 = divide(b1, b2, &r);
	cout << "Частное:" << endl;
	cout << b3 << endl;
	cout << "Остаток:" << endl;
	cout << r << endl;

	return 0;
}
