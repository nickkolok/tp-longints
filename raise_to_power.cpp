#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int_square.cpp"


int main(){

	cout << "Введите основание степени:" << endl;
	BigInt b1(cin, 9);

//	cout << "Введите показатель степени:" << endl;
//	BigInt b2(cin, 5);


	b1.divideByTwo();
//	b2.divideByTwo();
	b1.writeToStream(cout);
	cout << endl;
//	b2.writeToStream(cout);

/*	BigInt b3 = multiply(b1, b2);
	cout << "Произведение" << endl;
	b3.writeToStream(cout);
*/

	return 0;
}
