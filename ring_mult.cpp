#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int_ring.cpp"

int main(){

	cout << "Введите модуль кольца вычетов:" << endl;
	BigInt modulo(cin, 2);

	cout << "Введите первый операнд:" << endl;
	BigInt b1(cin, 2);
	cout << "Введитe второй операнд:" << endl;
	BigInt b2(cin, 2);
	cout << "Введите знак операции:  *  или  ^" << endl;
	char act;
	cin >> act;

	BigInt b3(4);

	switch(act){
		case '*':
			b3 = multiplyRingDirect(b1, b2, modulo);
			cout << b3 << endl;
			b3 = multiplyRing(b1, b2, modulo);
		break;
		case '^':
			b3 = raiseToPowerRingDirect(b1, b2, modulo);
		break;
		default:
			cout << "Некорректная операция" << endl;
			return -2;
	}
	cout << "Результат:" << endl;
	cout << b3 << endl;
	return 0;
}
