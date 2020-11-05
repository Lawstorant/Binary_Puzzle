#ifndef PRINT_H
#define PRINT_H

// Wyœwietlanie ³adniejszej ramki
void printBetterFrame (int x, int y, int fpx, int fpy, int isField);

// Wyœwietlanie legendy
void printLegend (char *path, int x, int y, struct pos *positions, struct color *colors);

// Wype³nianie planszy danymi
void printFieldData (struct fieldInfo *field, struct pos *positions, struct color *colors) ;

// Podœwietlenie aktualnie zaznaczonej komórki na planszy
// Funkcja dla ³adniejszej ramki
void showBetterPosition (int x, int y, int n, struct pos *positions, struct color *colors);

// Wyœwietlanie pomocy dostêpnych znaków
int help (struct fieldInfo *field, int x, int y, struct pos *positions, struct color *colors, int actuallyDoIt);

// Wyœwietla iloœæ jedynek i zer przy kolumnach/wierszach
void showAdditionalHelp (struct fieldInfo *field, struct pos *positions, struct color *colors);

// Zaznacza pola do których nie mo¿na wpisaæ ¿adnej wartoœci
int checkGameEnd (struct fieldInfo *field, struct pos *positions, struct color *colors, int helpMessage);

// Podœwietlenie jednoznacznych pól
int oneShallStand (struct fieldInfo **field, struct pos *positions, struct color *colors, int printOrWrite);

#endif
