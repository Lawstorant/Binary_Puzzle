#ifndef RULES_H
#define RULES_H

// (1) W ¿adnej kolumnie i ¿adnym wierszu nie wystêpuj¹
// obok siebie wiêcej ni¿ dwie identyczne cyfry
// Funkcja zwraca 0 kiedy zasada pozostaje spe³niona
// 1 kiedy zasada jest z³amana w kolumnie
// 2 kiedy zasada jest z³amana w wierszu
// 3 kiedy zasada jest z³amana w kolumnie i wierszu
int rule1(struct fieldInfo *field, char input, int x, int y);

// (2) W ka¿dym wierszu i kolumnie znajduje siê
// tyle samo cyfr 0 i 1 (tzn. jest N/2 cyfr ka¿dego
// rodzaju).
// Funkcja zwraca 0 kiedy zasada pozostaje spe³niona a 1,
// kiedy zasada jest z³amana
int rule2(struct fieldInfo *field, char input, int x, int y);

// (3) Nie wystêpuj¹ dwa identyczne wiersze ani
// dwie identyczne kolumny.
// Funkcja zwraca 0 kiedy zasada pozostaje spe³niona a 1,
// kiedy zasada jest z³amana
int rule3(struct fieldInfo *field, char input, int x, int y);

#endif
