#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

// (1) W ¿adnej kolumnie i ¿adnym wierszu nie wystêpuj¹
// obok siebie wiêcej ni¿ dwie identyczne cyfry
// Funkcja zwraca 0 kiedy zasada pozostaje spe³niona
// 1 kiedy zasada jest z³amana w kolumnie
// 2 kiedy zasada jest z³amana w wierszu
// 3 kiedy zasada jest z³amana w kolumnie i wierszu
int rule1(struct fieldInfo *field, char input, int x, int y) {
	int i = 0, count1 = 0, count0 = 0, pos = 0;
	int f1 = 0, f2 = 0;
	int n = field->nSize;

	char temp = field->cell[x-1 + ((y-1)*n)].data;
	field->cell[x-1 + ((y-1)*n)].data = input;

	// Sprawdzanie w pionie
	for (i = 0; i < n; ++i) {
		pos = n * i + (x-1);

		if (field->cell[pos].data == '1') {
			++count1;
			count0 = 0;
		} else if (field->cell[pos].data == '0') {
			++count0;
			count1 = 0;
		} else {
			count0 = 0;
			count1 = 0;
		}

		if (count0 > 2 || count1 > 2) {
			f1 = 1;
			break;
		}
	}

	count0 = 0;
	count1 = 0;

	// Sprawdzanie w poziomie
	for (i = 0; i < n; ++i) {
		pos = n * (y-1) + i;

		if (field->cell[pos].data == '1') {
			++count1;
			count0 = 0;
		} else if (field->cell[pos].data == '0') {
			++count0;
			count1 = 0;
		} else {
			count0 = 0;
			count1 = 0;
		}

		if (count0 > 2 || count1 > 2) {
			f2 = 1;
			break;
		}
	}

	field->cell[x-1 + ((y-1)*n)].data = temp;

	if(f1 && f2) return 3;
	else if (f1) return 1;
	else if (f2) return 2;
	else return 0;
}


// (2) W ka¿dym wierszu i kolumnie znajduje siê
// tyle samo cyfr 0 i 1 (tzn. jest N/2 cyfr ka¿dego
// rodzaju).
// Funkcja zwraca 0 kiedy zasada pozostaje spe³niona a 1,
// kiedy zasada jest z³amana
int rule2(struct fieldInfo *field, char input, int x, int y) {
	int i = 0, j = 0,  count1 = 0, count0 = 0, pos = 0;
	int f1 = 0, f2 = 0;
	int n = field->nSize;

	char temp = field->cell[x-1 + ((y-1)*n)].data;
	field->cell[x-1 + ((y-1)*n)].data = input;

	// Sprawdzanie kolumn
	for (i = 0; i < n; ++i) {
		count0 = 0;
		count1 = 0;
		for (j = 0; j < n; ++j) {
			pos = i + (j * n);
			if (field->cell[pos].data == '1') {
				++count1;
			} else if (field->cell[pos].data == '0') {
				++count0;
			}
		}
		if (count0 > n/2 || count1 > n/2) {
			f1 = 1;
			break;
		}
	}

	count0 = 0;
	count1 = 0;

	// Sprawdzanie wierszy
	for (i = 0; i < n; ++i) {
		count0 = 0;
		count1 = 0;
		for (j = 0; j < n; ++j) {
			pos = j + (i * n);
			if (field->cell[pos].data == '1') {
				++count1;
			} else if (field->cell[pos].data == '0') {
				++count0;
			}
		}
		if (count0 > n/2 || count1 > n/2) {
			f2 = 1;
			break;
		}
	}

	field->cell[x-1 + ((y-1)*n)].data = temp;

	if(f1 && f2) return 3;
	else if (f1) return 1;
	else if (f2) return 2;
	else return 0;
}


// (3) Nie wystêpuj¹ dwa identyczne wiersze ani
// dwie identyczne kolumny.
// Funkcja zwraca 0 kiedy zasada pozostaje spe³niona a 1,
// kiedy zasada jest z³amana
int rule3(struct fieldInfo *field, char input, int x, int y) {
	int j = 0, k = 0;
	int pos1 = 0, pos2 = 0, sameCount = 0;
	char cmp1, cmp2;
	int n = field->nSize;
	int ret = 0;

	char temp = field->cell[x-1 + ((y-1)*n)].data;
	field->cell[x-1 + ((y-1)*n)].data = input;

	// Sprawdzanie kolumn

	for (j = 0; j < n; ++j) {
		sameCount = 0;
		if (j == x-1) ++j;

		for (k = 0; k < n; ++k) {
			pos1 = x-1 + (k * n);
			pos2 = j + (k * n);

			cmp1 = field->cell[pos1].data;
			cmp2 = field->cell[pos2].data;

			if ((cmp1 == '1' || cmp1 == '0') && cmp1 == cmp2) {
				++sameCount;
			}
		}
		if (sameCount == n) {
			ret += j+1;
			break;
		}
	}


	sameCount = 0;

	// Sprawdzanie wierszy

	for (j = 0; j < n; ++j) {
		sameCount = 0;
		if (j == y-1) ++j;

		for (k = 0; k < n; ++k) {
			pos1 = ((y-1) * n) + k;
			pos2 = (j * n) + k;

			char cmp1 = field->cell[pos1].data;
			char cmp2 = field->cell[pos2].data;

			if ((cmp1 == '1' || cmp1 == '0') && cmp1 == cmp2) {
				++sameCount;
			}
		}
		if (sameCount == n) {
			ret += (j+1)<<8;
			break;
		}
	}


	field->cell[x-1 + ((y-1)*n)].data = temp;

	return ret;
}
