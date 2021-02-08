#include "pch.h"
#include <iostream>

using namespace std;

double func(double a) {
	a = log(a) - 5 * pow(sin(a), 2);
	return a;
}

int main()
{
	double a = 3, b = 9, i = 0, x0 = a, e = 0.0001, eps = 1, x1 = 0, h = (b - a) / 10;
	while (a <= b) {
		if (func(a)*func(a + h) < 0) {
			x0 = a;
			i = 0;
			eps = 1;
			while (eps > e || i == 20) {
				x1 = x0 + 0.01*fabs(func(x0));
				eps = fabs(x1 - x0);
				x0 = x1;
				i++;
			}
			cout << x0 << endl;
			a = a + h;
		}
		else
			a = a + h;
		
	}
}
