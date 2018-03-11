#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int_square.cpp"


int main(){

	cout << "Введите число для возведения в квадрат:" << endl;
	BigInt b1(cin, 4);


	BigInt b3 = square(b1);
	cout << "Результат возведения в квадрат:" << endl;
	b3.writeToStream(cout);


	return 0;
}
