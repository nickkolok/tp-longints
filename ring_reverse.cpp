#include <iostream>
#include <cstdlib>

using namespace std;

#include "class_int_ring.cpp"

int main(){
	cout << "Введите модуль кольца:" << endl;
	BigInt modulo(cin, 4);

	cout << "Введите обращаемый элемент:" << endl;
	BigInt element(cin, 4);

	BigInt reverse = ringReverse(modulo, element);
	if(!reverse){
		cout << "Обратного элемента не существует" << endl;
	} else {
		cout << "Обратный элемент:" << endl << reverse << endl;
	}

	return 0;
}
