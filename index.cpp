#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int.cpp"

int main(){
//	cout << sizeof(int) << endl;
//	cout << (atoi("123")-atoi("10")) << endl;

	BigInt b(9, 100);
	b.readFromStream(cin);
	cout << b[0] << endl;
	cout << b[1] << endl;
	
	
	return 0;
}
