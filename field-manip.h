#ifndef FIELD_MANIP_H
#define FIELD_MANIP_H

// Alokacja pamiêci dla planszy o rozmiarze n*n
struct fieldInfo *allocateMemory (int n);

// Funkcja zeruj¹ca ca³¹ planszê
void clearCells (struct fieldInfo *field);

// Funkcja wpisuj¹ca dane do komórki, je¿eli nie przeczy to zasadom gry
int cellInput (struct fieldInfo *field, char input, int isDefault, int x, int y, int dontCheck);

// Czytanie z pliku
// Zwraca 1 kiedy nast¹pi³ b³¹d odczytu pliku
// lub plik nie istnieje
// Zwraca 2 kiedy plik ma nieprawid³ow¹ strukturê
int readFileInput (struct fieldInfo **field, char *filePath, int n, int hushPathError);

// Funkcja losowo wype³niaj¹ca % pó³ z zakresu podanego w sta³ych
// losowymi wartoœciami, tylko wtedy, kiedy wype³nienia spe³niaj¹
// wszystkie trzy zasady
void randomInput (struct fieldInfo *field, struct rndata *rndData);

// Zapisuje stan gry do pliku
void saveGameWrite (struct fieldInfo *field, int x, int y, struct messeges *show);

// Wczytuje stan gry z pliku
void saveGameRead (struct fieldInfo **field, int *x, int *y, struct messeges *show);

#endif
