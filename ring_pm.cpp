#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int_ring.cpp"

int main(){

	cout << "Введите модуль кольца вычетов:" << endl;
	BigInt modulo(cin, 9);

	cout << "Введите первый операнд:" << endl;
	BigInt b1(cin, 9);
	cout << "Введите второй операнд:" << endl;
	BigInt b2(cin, 9);
	cout << "Введите знак операции:  +  или  -" << endl;
	char act;
	cin >> act;

	BigInt b3(9);

	switch(act){
		case '+':
			b3 = sumRing(b1, b2, modulo);
		break;
		case '-':
			b3 = subtractRing(b1, b2, modulo);
		break;
		default:
			cout << "Некорректная операция" << endl;
			return -2;
	}
	cout << b3 << endl;
	return 0;
}
