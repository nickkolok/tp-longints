#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int.cpp"

int main(){

	cout << "Введите первый операнд:" << endl;
	BigInt b1(cin, 9);
	cout << "Введите второй операнд:" << endl;
	BigInt b2(cin, 9);
	cout << "Введите знак операции:  +  или  -" << endl;
	char act;
	cin >> act;

	switch(act){
		case '+':
			// Nothing to do!
		break;
		case '-':
			b2.m_bSign = !b2.m_bSign;
		break;
		default:
			cout << "Некорректная операция" << endl;
			return -2;
	}


	BigInt b3 = sumSigned(b1, b2);
	b3.writeToStream(cout);


	return 0;
}
