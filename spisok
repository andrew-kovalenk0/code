#include "pch.h"
#include <iostream>

using namespace std;

struct spisok {
	int inf;
	spisok *next;
	spisok *prev;
};

void AddFirst(spisok **p, int a)
{
	*p = new spisok;
	(*p)->next = NULL;
	(*p)->prev = NULL;
	(*p)->inf = a;
}

void adde(spisok **last, int a) {
	spisok *last_new = new spisok;
	last_new->inf = a;
	last_new->prev = *last;
	last_new->next = NULL;
	(*last)->next = last_new;
	*last = last_new;
}

void addb(spisok **last, int a) {
	spisok *last_new = new spisok;
	last_new->inf = a;
	last_new->next = *last;
	last_new->prev = NULL;
	(*last)->prev = last_new;
	*last = last_new;
}

void del(spisok **sp, spisok **beg) {
	if ((*sp)->next == NULL) {
		//cout << "del: " << (*sp)->inf << endl;
		(*sp)->prev->next = NULL;
	}

	else if ((*sp)->prev == NULL) {

		(*sp)->next->prev = NULL;
		*beg = (*sp)->next;

	}
	else
		(*sp)->prev->next = (*sp)->next;
}

void press(spisok *sp) {
	while (sp != NULL) {
		cout << sp->inf << endl;
		sp = sp->next;
	}
}

int main()
{
	int a, qn = 0, i =1;
	spisok *last = NULL;
	spisok *start = NULL;
	spisok *iter = NULL;
	last = start;
	while (i == 1 || i == 2) {
		cout << "To put element to end press 1, to begin press 2, to exit press 0." << endl;
		cin >> i;
		if (i == 0)
			break;
		if (i == 1) {
			cout << "Press element: ";
			cin >> a;
			if (qn == 0) {
				AddFirst(&start, a);
				last = start;
			}
			else
				adde(&last, a);
		}
		if (i == 2) {
			cout << "Press element: ";
			cin >> a;
			if (qn == 0) {
				AddFirst(&start, a);
				last = start;
			}
			else
				addb(&start, a);
		}
		qn++;
	}
	cout << endl;
	press(start);
	cout << endl;
	iter = last;
	while(iter!=NULL) {
			if (iter->inf < 0) {
				
				del(&iter, &start);
			}
		iter = iter->prev;
	}

	press(start);
}
