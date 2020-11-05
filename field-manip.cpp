#include <stdlib.h>
#include <stdio.h>
#include "conio2.h"
#include "structures.h"
#include "rules.h"


// Alokacja pamiêci dla planszy o rozmiarze n*n
struct fieldInfo *allocateMemory (int n) {
	struct fieldInfo *allocate;
	allocate = (struct fieldInfo *)malloc(sizeof(struct fieldInfo) + (n*n*sizeof(struct cells)));
	allocate->nSize = n;
	return allocate;
}


// Funkcja zeruj¹ca ca³¹ planszê
void clearCells (struct fieldInfo *field) {
	int i, j, obl;
	int n = field->nSize;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			obl = j + i*n;
			field->cell[obl].data = 0;
			field->cell[obl].isDefault = 0;
		}
	}
}


// Funkcja wpisuj¹ca dane do komórki, je¿eli nie przeczy to zasadom gry
// Zwraca 0 kiedy wpisywanie powiod³o siê
// Zwraca 1 kiedy wpisywanie nie powiodo siê (³ama³o zasady)
int cellInput (struct fieldInfo *field, char input, int setDefault, int x, int y, int dontCheck) {
	int pos = field->nSize;
	pos = (y-1)*pos + (x-1);

	if (!dontCheck) {
		if(!field->cell[pos].isDefault){
			int t1 = rule1(field, input, x, y);
			int t2 = rule2(field, input, x, y);
			int t3 = rule3(field, input, x, y);

			if(t1 == 0 && t2 == 0 && t3 == 0) {
				field->cell[pos].data = input;
				if(setDefault) field->cell[pos].isDefault = 1;
				return 0;
			}
			else return 1;
		}
		else return 1;
	} else {
		field->cell[pos].data = input;
		if(setDefault) field->cell[pos].isDefault = 1;
		return 0;
	}
}


// Czytanie z pliku
// Zwraca 0 kiedy nie wyst¹pi³y ¿adne problemy
// Zwraca 1 kiedy nast¹pi³ b³¹d odczytu pliku
// lub plik nie istnieje
// Zwraca 2 kiedy plik ma nieprawid³ow¹ strukturê
// Zwraca 3 kiedy dane w pliku s¹ niezgodne z regu³ami gry
int readFileInput (struct fieldInfo **field, char *filePath, int n, int hushPathError) {
	FILE *inputFile = fopen(filePath, "r");
	char character = 0;
	//int n = field->nSize;
	int f1 = 0, f2 = 0;
	int x = 1, y = 1;
	struct fieldInfo *tempField = allocateMemory(n);

	if (inputFile) {
		clearCells(tempField);
		while (character != EOF) {
			character = fgetc(inputFile);

			if (x == n+1 && (character != EOF && character != '\n')) {
				f1 = 1;
				break;
			}
			if (y == n+1 && character != EOF) {
				f1 = 1;
				break;
			}

			if(character == '0' || character == '1') {
				if (cellInput(tempField, character, 1, x, y, 0)) {
					f2 = 1;
					break;
				}
			}

			if (character == '\n') {
				x = 1;
				++y;
			} else {
				++x;
			}
		}

		fclose(inputFile);

		if(!f1 && !f2) {
			free(*field);
			*field = tempField;
			return 0;
		}
		else if (f1) {
			free(tempField);
			clrscr();
			gotoxy(3,3);
			char temp[100];
			sprintf(temp, "Plik '%s' ma nieprawidlowa strukture!", filePath);
			gotoxy(3,4);
			cputs("Proba wczytywania pliku odrzucona!");
			getch();
			return 2;
		}
		else if (f2) {
			free(tempField);
			clrscr();
			char temp[100];
			gotoxy(3,3);
			sprintf(temp, "Dane w pliku '%s' sa niezgodne z zasadami gry!", filePath);
			gotoxy(3,6);
			cputs("Proba wczytywania pliku odrzucona!");
			getch();
			return 3;
		}
	}
	else {
		free(tempField);
		if (!hushPathError) {
			clrscr();
			gotoxy(3, 3);
			cputs("Nieprawidlowa sciezka do pliku lub plik uszkodzony");
			gotoxy(3,4);
			cputs("Proba wczytywania pliku odrzucona!");
			getch();
		}
		return 1;
	}

	return -1;
};


// FIXME: napraw to
// Funkcja losowo wype³niaj¹ca % pó³ z zakresu podanego w sta³ych
// losowymi wartoœciami, tylko wtedy, kiedy wype³nienia spe³niaj¹
// wszystkie trzy zasady
void randomInput (struct fieldInfo *field, struct rndata *rndData) {
	int n = field->nSize;
	int bot = rndData->rndBot;
	int top = rndData->rndTop;
	int tries = rndData->tries;
	int x, y, counter = 0;
	char input;

	int draws = rand()%(top - bot + 1) + bot;
	draws = (float)(n*n) * ((float)draws / 100);
	int succesfulDraws = 0;

	while (succesfulDraws < draws && counter < tries) {
		++counter;

		x = rand()%(n)+1;
		y = rand()%(n)+1;

		if ((rand()%2) == 1) input = '1';
		else input = '0';

		if (!cellInput(field, input, 1, x, y, 0)) {
			++succesfulDraws;
		}
	}
}


// Zapisuje stan gry do pliku
void saveGameWrite (struct fieldInfo *field, int x, int y, struct messeges *show) {
	char input[100] = {};
	int count = 0, i = 0;

	do {
		clrscr();
		gotoxy(3,3);
		cputs("Zapisywanie gry");
		gotoxy(3,4);
		cputs("Podaj sciezke:");
		gotoxy(3,7);
		cputs("Tylko litery, cyfry, kropki, dwukropki i ukosniki!");
		gotoxy(3,5);
		putch('>');
		for(i = 0; i < count; ++i) {
			putch(input[i]);
		}

		input[count] = getch();
		clrscr();

		// \b to klawisz Backspace
		if (input[count] == '\b') {
			input[count] = 0;
			if (count > 0) {
				--count;
				input[count] = 0;
			}
		}
		else if((input[count] >= 45 && input[count] <= 58) || (input[count] >= 65 && input[count] <= 90) || (input[count] >= 97 && input[count] <= 122)) {
			++count;
		}
	} while (input[count] != '\r'); // \r to klawisz Enter

	input[count] = '\0';

	FILE *outputFile = fopen(input, "w");

	if (outputFile) {
		int i, j, n = field->nSize;
		fputc(n+48, outputFile);
		fputc(x+48, outputFile);
		fputc(y+48, outputFile);
		fputc(show->help+48, outputFile);
		fputc(show->end+48, outputFile);
		fputc(show->one+48, outputFile);
		fputc(show->help2+48, outputFile);
		fputc(show->automat+48, outputFile);
		fputc('\n', outputFile);
		for (i = 0; i < n; ++i) {
			for (j = 0; j < n; ++j) {
				if (field->cell[i*n+j].data) {
					if(field->cell[i*n+j].data == '0') {
						if (field->cell[i*n+j].isDefault) fputc('2', outputFile);
						else fputc('0', outputFile);
					}
					else {
						if (field->cell[i*n+j].isDefault) fputc('3', outputFile);
						else fputc('1', outputFile);
					}
				}
				else fputc('.', outputFile);
			}
			fputc('\n', outputFile);
		}
		fclose(outputFile);
		clrscr();
		gotoxy(3,3);
		cputs("Zapis gry zakonczony powodzeniem!");
		getch();
	}
	else {
		clrscr();
		gotoxy(3,3);
		cputs("Zapis gry zakonczony niepowodzeniem!");
		gotoxy(3,4);
		cputs("Nie udalo sie utworzyc pliku.");
		getch();
	}
}


// Wczytuje stan gry z pliku
void saveGameRead (struct fieldInfo **field, int *x, int *y, struct messeges *show) {
	char input[100] = {};
	int count = 0, i = 0;
	struct fieldInfo *tempField;

	do {
		clrscr();
		gotoxy(3,3);
		cputs("Wczytywanie gry");
		gotoxy(3,4);
		cputs("Podaj sciezke:");
		gotoxy(3,7);
		cputs("Tylko litery, cyfry, kropki, dwukropki i ukosniki!");
		gotoxy(3,5);
		putch('>');
		for(i = 0; i < count; ++i) {
			putch(input[i]);
		}

		input[count] = getch();
		clrscr();

		// \b to klawisz Backspace
		if (input[count] == '\b') {
			input[count] = 0;
			if (count > 0) {
				--count;
				input[count] = 0;
			}
		}
		else if((input[count] >= 45 && input[count] <= 58) || (input[count] >= 65 && input[count] <= 90) || (input[count] >= 97 && input[count] <= 122)) {
			++count;
		}
	} while (input[count] != '\r'); // \r to klawisz Enter

	input[count] = '\0';

	FILE *inputFile = fopen(input, "r");

 	if (inputFile) {
		int i, j, n;
		char character = 0;
		char temp[40] = {};
		n = fgetc(inputFile) - 48;
		tempField = allocateMemory(n);
		clearCells(tempField);
		int x1 = fgetc(inputFile) - 48;
		int y1 = fgetc(inputFile) - 48;
		int t1 = fgetc(inputFile) - 48;
		int t2 = fgetc(inputFile) - 48;
		int t3 = fgetc(inputFile) - 48;
		int t4 = fgetc(inputFile) - 48;
		int t5 = fgetc(inputFile) - 48;
		character = fgetc(inputFile);

		if(character == '\n' && x1 <= n && y1 <= n) {
			for (i = 0; i < n; ++i) {
				fgets(temp, 40, inputFile);
				for (j = 0; j < n; ++j) {
					if (temp[j] == '1') {
						tempField->cell[i*n+j].data = '1';
					}
					else if (temp[j] == '3') {
						tempField->cell[i*n+j].data = '1';
						tempField->cell[i*n+j].isDefault = '1';
					}
					else if (temp[j] == '0') {
						tempField->cell[i*n+j].data = '0';
					}
					else if (temp[j] == '2') {
						tempField->cell[i*n+j].data = '0';
						tempField->cell[i*n+j].isDefault = '1';
					}
				}
			}
			fclose(inputFile);
			*x = x1;
			*y = y1;
			if (t1) show->help = 2;
			if (t2) show->end = 2;
			if (t3) show->one = 2;
			if (t4) show->help2 = 2;
			if (t5) show->automat = 1;

			free(*field);
			*field = tempField;

			clrscr();
			gotoxy(3,3);
			cputs("Wczytanie gry zakonczone powodzeniem!");
			getch();
		}
		else {
			clrscr();
			gotoxy(3,3);
			cputs("Wczytanie gry zakonczone niepowodzeniem!");
			gotoxy(3,4);
			cputs("Plik jest uszkodzony.");
			getch();
		}
	}
	else {
		clrscr();
		gotoxy(3,3);
		cputs("Wczytanie gry zakonczone niepowodzeniem!");
		gotoxy(3,4);
		cputs("Nie udalo sie otworzyc pliku lub plik nie istnieje.");
		getch();
	}
}
