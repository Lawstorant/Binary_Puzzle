#ifndef RULES_H
#define RULES_H

// (1) W �adnej kolumnie i �adnym wierszu nie wyst�puj�
// obok siebie wi�cej ni� dwie identyczne cyfry
// Funkcja zwraca 0 kiedy zasada pozostaje spe�niona
// 1 kiedy zasada jest z�amana w kolumnie
// 2 kiedy zasada jest z�amana w wierszu
// 3 kiedy zasada jest z�amana w kolumnie i wierszu
int rule1(struct fieldInfo *field, char input, int x, int y);

// (2) W ka�dym wierszu i kolumnie znajduje si�
// tyle samo cyfr 0 i 1 (tzn. jest N/2 cyfr ka�dego
// rodzaju).
// Funkcja zwraca 0 kiedy zasada pozostaje spe�niona a 1,
// kiedy zasada jest z�amana
int rule2(struct fieldInfo *field, char input, int x, int y);

// (3) Nie wyst�puj� dwa identyczne wiersze ani
// dwie identyczne kolumny.
// Funkcja zwraca 0 kiedy zasada pozostaje spe�niona a 1,
// kiedy zasada jest z�amana
int rule3(struct fieldInfo *field, char input, int x, int y);

#endif
