#include <iostream>
#include <cstdlib>


using namespace std;

#include "class_int.cpp"

int main(){
//	cout << sizeof(int) << endl;
//	cout << (atoi("123")-atoi("10")) << endl;

/*
	BigInt b(cin, 9);
	//b.readFromStream(cin);
	//cout << b[0] << endl;
	//cout << b[1] << endl;
	cout << b.size() << endl;
	b.normalizeSize();
	cout << b.size() << endl;
	b.writeToStream(cout);
	cout << endl;

	BigInt b2(cin, 7);
	//b.readFromStream(cin);
	//cout << b2[0] << endl;
	//cout << b2[1] << endl;
	cout << b2.size() << endl;
	b2.normalizeSize();
	cout << b2.size() << endl;
	b2.writeToStream(cout);
	cout << endl;
*/


	BigInt b1(cin, 9);
	BigInt b2(cin, 9);

	//BigInt b3 = sumNaive(b1, b2);
	//b3.writeToStream(cout);

/*
	cout << b1.size() << " " << b2.size() << endl;
	cout << compareAbs(b1, b2) << endl;
	cout << compareAbs(b2, b1) << endl;
*/

	BigInt b3 = subtractNaive(b1, b2);
	b3.writeToStream(cout);


	return 0;
}
