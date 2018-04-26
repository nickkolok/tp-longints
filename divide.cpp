#include <iostream>
#include <cstdlib>

using namespace std;

#include "class_int_division.cpp"

int main(){
	cout << "Введите делимое" << endl;
	BigInt b1(cin, 4);

	int d;
	cout << "Введите делитель" << endl;
	cin >> d;

	BigInt b3 = divideByDigit(b1, d);
	cout << b3 << endl;

	return 0;
}
