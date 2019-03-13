#include "pch.h"
#include <iostream>

using namespace std;

struct stack {
	int inf;
	stack *next;
};

void ad(stack **top, int value) {
	stack *q = new stack();
	q->inf = value;
	if (top == NULL) {
		*top = q;
	}
	else {
		q->next = *top;
		*top = q;
	}
}

int read(stack *top) {
	stack *q = top;
	return q -> inf;
}

void press(stack *top) {
	stack *q = top;
	while (q) {
		cout << q -> inf << endl;
		q = q -> next;
	}
}

int main() {
	int i = 1, value = 0, qt = 0, max = 0, min = 0;
	stack *top = NULL;
	while (i == 1) {
		cout << "To add one more element press 1, or press 0:" << endl;
		cin >> i;
		if (i == 0) break;
		cout << "Enter value: ";
		cin >> value;
		ad(&top, value);
		qt++;
	}
	stack *q = top;
	max = read(q);
	min = read(q);
	cout << endl;
	int maxi = 0, mini = 0;
	press(q);
	for (int j = 0; j < qt; j++) {
		if (read(q) > max) {
			max = read(q);
			maxi = j;
		}
		if (read(q) < min) {
			min = read(q);
			mini = j;
		}
		q = q -> next;
	}
	int kv = 0;
	kv = fabs(mini - maxi) - 1;
	cout << endl;
	cout << "Minimal value: " << min << endl;
	cout << "Maximal value: " << max << endl;
	cout << "Kol-vo: " << kv << endl;
}
