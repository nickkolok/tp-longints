#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int_square.cpp"


int main(){

	cout << "Введите основание степени:" << endl;
	BigInt b1(cin, 4);

	cout << "Введите показатель степени:" << endl;
	BigInt b2(cin, 9);

	BigInt b3 = raiseToPower(b1, b2);
	cout << "Результат:" << endl;
	b3.writeToStream(cout);


	return 0;
}
