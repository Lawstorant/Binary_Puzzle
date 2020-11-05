#ifndef PRINT_H
#define PRINT_H

// Wy�wietlanie �adniejszej ramki
void printBetterFrame (int x, int y, int fpx, int fpy, int isField);

// Wy�wietlanie legendy
void printLegend (char *path, int x, int y, struct pos *positions, struct color *colors);

// Wype�nianie planszy danymi
void printFieldData (struct fieldInfo *field, struct pos *positions, struct color *colors) ;

// Pod�wietlenie aktualnie zaznaczonej kom�rki na planszy
// Funkcja dla �adniejszej ramki
void showBetterPosition (int x, int y, int n, struct pos *positions, struct color *colors);

// Wy�wietlanie pomocy dost�pnych znak�w
int help (struct fieldInfo *field, int x, int y, struct pos *positions, struct color *colors, int actuallyDoIt);

// Wy�wietla ilo�� jedynek i zer przy kolumnach/wierszach
void showAdditionalHelp (struct fieldInfo *field, struct pos *positions, struct color *colors);

// Zaznacza pola do kt�rych nie mo�na wpisa� �adnej warto�ci
int checkGameEnd (struct fieldInfo *field, struct pos *positions, struct color *colors, int helpMessage);

// Pod�wietlenie jednoznacznych p�l
int oneShallStand (struct fieldInfo **field, struct pos *positions, struct color *colors, int printOrWrite);

#endif
