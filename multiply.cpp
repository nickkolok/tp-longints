#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int_square.cpp"


int main(){

	cout << "Введите первый множитель:" << endl;
	BigInt b1(cin, 4);
	cout << "Введите второй множитель:" << endl;
	BigInt b2(cin, 4);



	BigInt b3 = multiply(b1, b2);
	cout << "Произведение" << endl;
	b3.writeToStream(cout);


	return 0;
}
