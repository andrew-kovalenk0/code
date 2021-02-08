#include "pch.h"
#include <iostream>

using namespace std;

int main() {
	double **m, *o, *an, *x, *xn, *s, *dif, eps = 0, min = 1;
	int  a = 0, b = 0, f = 0;

	cout << "Press number of variables: ";
	cin >> a;
	cout << endl;
	cout << "Press accuracy: ";
	cin >> eps;
	cout << endl;

	m = new double*[a];
	o = new double[a];
	x = new double[a];
	xn = new double[a];
	s = new double[a];
	dif = new double[a];
	for (int i = 0; i < a; i++)
	{
		m[i] = new double[a];
		x[i] = 1;
		dif[i] = 1;
	}
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < a; j++)
		{
			cout << "Press element m[" << i + 1 << "][" << j + 1 << "]: ";
			cin >> m[i][j];
		}
	}
	cout << endl;
	for (int i = 0; i < a; i++)
	{
		cout << "Press " << i + 1 << "th solution: ";
		cin >> o[i];
	}

	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < a; j++)
		{
			if (m[i][i] < m[i][j])
				f = 1;
		}
	}

	if (f == 0) {
		while (min > eps) {
			cout << endl << "Iteration #" << b+1 << endl << endl;
			for (int i = 0; i < a; i++)
			{
				min = dif[0];
				s[i] = o[i];
				for (int j = 0; j < a; j++)
				{
					if (i != j)
					{
						s[i] -= m[i][j] * x[i];
					}
				}
				xn[i] = s[i] / m[i][i];
				dif[i] = fabs(x[i] - xn[i]);
				if (dif[i] < min)
					min = dif[i];
				x[i] = xn[i];
				cout << "X" << i + 1 << " = " << x[i] << endl;
			}
			b += 1;
			cout << endl << "Difference = " << min << endl << endl << "-------------------------" << endl;
		}
	}
	else
		cout << endl << "Matrix don't converge!" << endl;
}
