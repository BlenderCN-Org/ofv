#include <iostream>
#include "tdlib.h"
using namespace std;

int main()
{
	vector a(1, 1, 1);
	cout << a.normalized() << endl;
	cout << a.magnitude() << endl;
	cout << u << endl;
	cout << r << endl;
	cout << b << endl;
	cout << b + r << endl;
	cout << r - u << endl;
	cout << r * 5 << endl;
	cout << r * b << endl;
	cout << r * u << endl;
	cout << -u + r + b << endl;

	a += u;
	cout << a << endl;

	a -= u * 2;
	a -= r;
	a -= b;
	cout << a << endl;

	vector asdf = a;
	asdf += u;
	cout << asdf << a << endl;
}