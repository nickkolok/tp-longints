#include <iostream>
#include <cstdlib>

using namespace std;

#include "class_int_roots.cpp"

int main(){
	cout << "Введите число для извлечения корня:" << endl;
	BigInt b1(cin, 4);

	BigInt b3 = squareRoot(b1);
	cout << "Квадратный корень:" << endl;
	cout << b3 << endl;

	BigInt b4 = mthRoot(b1, 3);
	cout << "Кубический корень:" << endl;
	cout << b4 << endl;

	cout << "************" << endl;
	cout << "Квадратный корень:" << endl;
	cout << b3 << endl;
	cout << "Кубический корень:" << endl;
	cout << b4 << endl;

	return 0;
}
