#ifndef FIELD_MANIP_H
#define FIELD_MANIP_H

// Alokacja pami�ci dla planszy o rozmiarze n*n
struct fieldInfo *allocateMemory (int n);

// Funkcja zeruj�ca ca�� plansz�
void clearCells (struct fieldInfo *field);

// Funkcja wpisuj�ca dane do kom�rki, je�eli nie przeczy to zasadom gry
int cellInput (struct fieldInfo *field, char input, int isDefault, int x, int y, int dontCheck);

// Czytanie z pliku
// Zwraca 1 kiedy nast�pi� b��d odczytu pliku
// lub plik nie istnieje
// Zwraca 2 kiedy plik ma nieprawid�ow� struktur�
int readFileInput (struct fieldInfo **field, char *filePath, int n, int hushPathError);

// Funkcja losowo wype�niaj�ca % p� z zakresu podanego w sta�ych
// losowymi warto�ciami, tylko wtedy, kiedy wype�nienia spe�niaj�
// wszystkie trzy zasady
void randomInput (struct fieldInfo *field, struct rndata *rndData);

// Zapisuje stan gry do pliku
void saveGameWrite (struct fieldInfo *field, int x, int y, struct messeges *show);

// Wczytuje stan gry z pliku
void saveGameRead (struct fieldInfo **field, int *x, int *y, struct messeges *show);

#endif
