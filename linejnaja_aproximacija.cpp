#include "pch.h"
#include <iostream>
#include <cmath>

using namespace std;

double func(double a) {
	a = log(a) - 5 * pow(sin(a), 2);
	return a;
}

int main()
{
	int a = 3, b = 6, m = 11, n = 21, q = 0, l = 1;
	double *xi, *xj, *yi, *yj, *fi, *dif;
	xi = new double[m];
	xj = new double[n];
	yi = new double[m];
	yj = new double[n];
	fi = new double[n];
	dif = new double[n];
	for (int i = 0; i < m; i++){
		xi[i] = 3 + i * 0.3;
		yi[i] = func(xi[i]);
	}
	for (int i = 0; i < n; i++){
		xj[i] = 3 + i * 0.15;
		yj[i] = func(xj[i]);
	}
	for (int i = 0; i < n; i++){
		while (xj[i] > xi[l])
			l++;
		fi[i] = yi[l - 1] + (xj[i] - xi[l - 1])*(yi[l] - yi[l - 1]) / (xi[l] - xi[l - 1]);
		dif[i] = yj[i] - fi[i];
		l = 1;
	}

	for (int i = 0; i < n; i++){
		cout << xj[i] << "            ";
		cout << yj[i] << "            ";
		cout << fi[i] << "            ";
		cout<< dif[i] << endl;
	}
}
