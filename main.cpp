#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "conio2.h"

// Podzia³ na pliki
#include "structures.h"
#include "print.h"
#include "field-manip.h"

// Sta³e
#define N 8                 // Domyœlna d³ugoœæ boku pola gry

#define LEGEND_WIDTH 25     // Na podstawie tekstu w legendzie
#define HELP_WIDTH 37        // Na podstawie tekstu w oknie pomocy
#define FIELD_IS_ON_LEFT 0  // Ustawienie pola gry po lewej

#define TEXT_COLOR LIGHTGRAY        // Kolor normalnego tekstu
#define ALT_TEXT_COLOR LIGHTGREEN   // Kolor alternatywnego tekstu
#define POSITION_COLOR YELLOW       // Kolor zaznaczania pozycji
#define BACKGROUND BLACK            // Kolor normalnego t³a
#define ALT_BACKGROUND LIGHTGRAY    // Kolor alternatywnego t³a
#define ADD_1_COLOR LIGHTMAGENTA         // Kolor ilosci jedynek w dodatkowej pomocy

#define NEW_GAME "new-game.txt"  // Œcie¿ka do nowej gry
#define LEGEND "legend.txt"      // Œcie¿ka do legendy

// Pliki ³adowane przy rozpoczecniu gry o wskazanej wielkoœci pola
// powinny mieæ nazwê w konwencji: "field-nxn.txt"

#define RND_PERCENT_BOT 20  // Procentowa wartoœæ dolnej granicy iloœci losowanych pól
#define RND_PERCENT_TOP 50  // Procentowa wartoœæ górnej granicy iloœci losowanych pól
#define RND_TRIES N*N		// Maksymalna iloœæ prób losowania


//Potêgowanie
int power (int x, int y) {
	int z = 1, i;
	for (i = 0; i < y; ++i) {
		z *= x;
	}
	return z;
}

// Umo¿liwia zmianê wielkoœci planszy
// Pozwala na podanie liczby co njawy¿ej dwucyfrowej
// Zwraca -1 w przypadku podania nieprawid³owych danych
int inputSize(struct pos *positions, struct text_info *screenInfo) {
	int input[20] = {};
	int count = 0, i = 0;

	do {
		clrscr();
		gotoxy(3,3);
		cputs("Podaj N dla nowego rozmiaru planszy N*N: ");
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
		} else if (count > 2) {
			input[count] = 0;
		} else {
			if (input[count] >= 48 && input[count] <= 57 && count < 2) {
				++count;
			}
		}
	} while (input[count] != '\r'); // \r to klawisz Enter

	int newFieldSize = 0;
	for(i = 0; i < count; ++i) {
		newFieldSize += (input[i]-48) * power(10,count-i-1);
	}

	// Sprawdzamy czy pole o takim rozmiarze zmieœci siê w oknie
	int check1 = screenInfo->screenwidth - (positions->legendWidth + positions->helpWidth + newFieldSize*2 + 16); // 12 to odstepy w ramkach itp;
	int check2 = screenInfo->screenheight - (newFieldSize*2 + 6);

	if(check1 < 0 || check2 < 0) {
		clrscr();
		gotoxy(3,3);
		cputs("Podano zbyt duza wartosc wielkosci pola!");
		gotoxy(3,4);
		cputs("Pole nie zmiesci sie na ekranie!");
		gotoxy(3,5);
		cputs("Proba zmiany zostala odrzucona");
		getch();
		clrscr();
		return -1;
	}
	else if (newFieldSize % 2 || newFieldSize < 2) {
		clrscr();
		gotoxy(3,3);
		cputs("Podano niepoprawna wartosc wielkosci pola!");
		gotoxy(3,4);
		cputs("Proba zmiany zostala odrzucona");
		getch();
		clrscr();
		return -1;
	}
	else return newFieldSize;
}


void computePositions(struct pos *positions, int n, int fieldIsOnLeft) {

	if (fieldIsOnLeft) {
		positions->fieldPositionX = 2;
		positions->helpPositionX = n*2+9;
		positions->legendPositionX = n*2+7 + HELP_WIDTH + 7;

	} else {
		positions->fieldPositionX = LEGEND_WIDTH + HELP_WIDTH + 12;
		positions->helpPositionX = LEGEND_WIDTH + 9;
		positions->legendPositionX = 4;
	}
	positions->fieldPositionY = 2;
	positions->helpPositionY = 3;
	positions->legendPositionY = 3;
	positions->legendWidth = LEGEND_WIDTH;
	positions->helpWidth = HELP_WIDTH;
}

// G³ówna funkcja gry
void game (int n) {
	int input; // Przechowuje wejscie od u¿ytkwonika
	int positionX = 1; // Przechowuje wartoœæ X pozycji zaznaczenia
	int positionY = 1; // Przechowuje wartoœæ Y pozycji zaznaczenia
	char legendPath[40] = LEGEND; // Przechowuje œcie¿kê do legendy

	// Poni¿sze struktury przechowuj¹ dane na temat kolorów, pozycji legendy i pola gry
	// a tak¿e wartoœci dla "maszyny losuj¹cej"
	struct color colors = {TEXT_COLOR, ALT_TEXT_COLOR, POSITION_COLOR, BACKGROUND, ALT_BACKGROUND, ADD_1_COLOR};
	struct pos positions;
	struct rndata rndData = {RND_PERCENT_BOT, RND_PERCENT_TOP, RND_TRIES};
	struct messeges show = {0, 0, 0, 0, 0};
	
	// Sprawdzam rozmiar konsoli
	struct text_info screenInfo;
	gettextinfo(&screenInfo);

	computePositions(&positions, n, FIELD_IS_ON_LEFT);

	struct fieldInfo *fieldData;
	fieldData = allocateMemory(n);
	clearCells(fieldData);

	textbackground(colors.bgColor);
	textcolor(colors.textColor);

	do {
		clrscr();
		printLegend(legendPath, positionX, positionY, &positions, &colors);
		printBetterFrame(fieldData->nSize, fieldData->nSize, positions.fieldPositionX, positions.fieldPositionY, 1);
		printFieldData(fieldData, &positions, &colors);
		if (show.one == 1) --show.one;
		else if (show.one == 2){
			if (show.one == 2) oneShallStand(&fieldData, &positions, &colors, 0);
			show.one = 1;
		}
		if (show.automat) {
			show.end = 2;
			show.help2 = 2;
			gotoxy(positions.helpPositionX + positions.helpWidth/2 - 11, 16);
			textcolor(colors.positionColor);
			cputs("- Tryb automatyczny -");
			textcolor(colors.textColor);
		}
		if (show.end) {
			if (show.end == 2 && show.automat) {
				int test = checkGameEnd(fieldData, &positions, &colors, 0);
				if (test && show.help != 2) {
					gotoxy(positions.helpPositionX, positions.helpPositionY+2);
					cputs("Gry nie da sie ukonczyc");
					gotoxy(positions.helpPositionX, positions.helpPositionY+5);
					cputs("Smuteczek :(");
				}
			}
			else if (show.end == 2) checkGameEnd(fieldData, &positions, &colors, 1);
			--show.end;
		}
		if (show.help2) {
			if (show.help2 == 2) showAdditionalHelp(fieldData, &positions, &colors);
			--show.help2;
		}
		show.help = help(fieldData, positionX, positionY, &positions, &colors, show.help);
		showBetterPosition(positionX, positionY, fieldData->nSize, &positions, &colors);

		input = getch();

		switch (input) {

			//Poruszanie siê
			case 0: {
				input = getch();
				switch (input) {
				case 72: // strza³ka w górê
					if(positionY-1 >= 1)
						positionY-=1;
					break;

				case 80: // strza³ka w dó³
					if(positionY+1 <= fieldData->nSize)
						positionY+=1;
					break;

				case 75: // strza³ka w lewo
					if(positionX-1 >= 1)
						positionX-=1;
					break;

				case 77: // strza³ka w prawo
					if(positionX+1 <= fieldData->nSize)
						positionX+=1;
					break;
				}
				break;
			}

			case ' ': {
				int pos = (positionY-1)*fieldData->nSize + (positionX-1);
				if(!fieldData->cell[pos].isDefault) {
					fieldData->cell[pos].data = 0;
				}
				break;
			}

			case '0':
			case '1': {
				cellInput(fieldData, input, 0, positionX, positionY, 0);
				break;
			}

			case 'n':
			case 'N': {
				positionX = 1;
				positionY = 1;
				char path[] = NEW_GAME;
				readFileInput(&fieldData, path, 12, 0);
				computePositions(&positions, 12, FIELD_IS_ON_LEFT);
				break;
			}

			case 'o':
			case 'O': {
				clearCells(fieldData);
				randomInput(fieldData, &rndData);
				break;
			}

			case 'r':
			case 'R': {
				int temp = 0;
				temp = inputSize(&positions, &screenInfo);

				if (temp != -1) {
					free(fieldData);
					fieldData = allocateMemory(temp);
					clearCells(fieldData);
					computePositions(&positions, temp, FIELD_IS_ON_LEFT);
					positionX = 1;
					positionY = 1;

					char path[40] = {};
					sprintf(path, "field-%dx%d.txt", fieldData->nSize, fieldData->nSize);
					if (readFileInput(&fieldData, path, fieldData->nSize, 1)) randomInput(fieldData, &rndData);
				}
				break;
			}

			case 'p':
			case 'P': {
				show.help = 2;
				break;
			}

			case 'k':
			case 'K': {
				show.end = 2;
				break;
			}

			case 'd':
			case 'D': {
				show.help2 = 2;
				break;
			}

			case 'a':
			case 'A': {
				if (show.automat) {
					show.automat = 0;
					show.end = 0;
					show.help2 = 0;
				}
				else show.automat = 1;
				break;
			}

			case 'j':
			case 'J': {
				show.one = 2;
				break;
			}

			case 'w':
			case 'W': {
				if (show.one == 1) {
					oneShallStand(&fieldData, &positions, &colors, 1);
					show.one = 0;
				}
				break;
			}

			case 's':
			case 'S': {
				saveGameWrite(fieldData, positionX, positionY, &show);
				++show.help;
				++show.help2;
				++show.one;
				++show.end;
				break;
			}

			case 'l':
			case 'L': {
				saveGameRead(&fieldData, &positionX, &positionY, &show);
				computePositions(&positions, fieldData->nSize, FIELD_IS_ON_LEFT);
				break;
			}

			default: {
				clrscr();
			}
		}
	} while (input != 0x1b); // klawisz Esc
	free(fieldData);
}


// main
int main() {
    #ifndef __cplusplus
	Conio2_Init();
    #endif

	srand(time(NULL));

	settitle("Tomasz Pakula 172064");
	system("chcp 437"); // Zmiano kodowania konsoli na ANSI
						// Dziêki temu mo¿emy u¿ywaæ wszystkich typów ramek ASCII
	
	if(N % 2) {
		clrscr();
		cputs("Blad! Podany wymiar boku jest nieparzysty, gra nie rozpocznie siê");
		getch();
	} else {
		game(N);
		clrscr();
	}

	return 0;
}
