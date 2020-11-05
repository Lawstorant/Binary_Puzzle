#include <stdio.h>
#include <stdlib.h>
#include "conio2.h"
#include "structures.h"
#include "rules.h"
#include "field-manip.h"


// Wyœwietlanie ³adniejszej ramki
void printBetterFrame (int x, int y, int fpx, int fpy, int isField) {
	int i, j;

	if(isField){
		x = x * 2 - 1;
		y = y * 2;

		for (i = 0; i <= y; ++i) {
			gotoxy(fpx, fpy+i);

			if(i == 0) {
				putch(201);
				for (j = 1; j <= x; ++j) {
					if(j%2 == 1)
						putch(205);
					else
						putch(209);
				}
				putch(187);
			}
			else if(i == y) {
				putch(200);
				for (j = 1; j <= x; ++j) {
					if(j%2 == 1)
						putch(205);
					else
						putch(207);
				}
				putch(188);
			}
			else {
				if (i%2 == 1) {
					putch(186);
					for(j = 1; j < x; j+=2) {
						putch(' ');
						putch(179);
					}
					putch(' ');
					putch(186);
				}
				else {
					putch(199);
					for(j = 0; j < x; ++j) {
						if(j%2 == 1)
							putch(197);
						else
							putch(196);
					}
					putch(182);
				}
			}
		}
	}
	else {
		gotoxy(fpx, fpy++);
		putch(201);
		for(i = 1; i < x; ++i) {
			putch(205);
		}
		putch(187);

		for(i = 0; i < y-2; ++i) {
			gotoxy(fpx, fpy);
			putch(186);
			gotoxy(fpx+x, fpy++);
			putch(186);
		}

		gotoxy(fpx, fpy++);
		putch(200);
		for(i = 1; i < x; ++i) {
			putch(205);
		}
		putch(188);
	}
}


// Wyœwietlanie legendy
void printLegend (char *path, int x, int y, struct pos *positions, struct color *colors) {
	//int nl = 1;
	char txt[50];


	int LPX = positions->legendPositionX;
	int LPY = positions->legendPositionY;

	gotoxy(LPX + positions->legendWidth/2 - 5, LPY++);
	cputs("- Legenda -");
	++LPY;
	gotoxy(LPX, LPY++);
	cputs("Tomasz Pakula 172064");
	++LPY;
	gotoxy(LPX, LPY++);
	cputs("Pozycja na planszy:");
	textcolor(colors->positionColor);
	gotoxy(LPX, LPY++);
	sprintf(txt, "X:%d Y:%d", x, y);
	cputs(txt);
	textcolor(colors->textColor);
	++LPY;

	FILE *inputFile = fopen(path, "r");

	if(inputFile) {
		while (fgets(txt, 50, inputFile) != NULL) {
			gotoxy(LPX, LPY++);
			cputs(txt);
		}
		fclose(inputFile);
	}

	int lines = LPY - positions->legendPositionY;
	printBetterFrame(positions->legendWidth+3, lines+2, positions->legendPositionX-2, positions->legendPositionY-1, 0);
}


// Wype³nianie planszy danymi
void printFieldData(struct fieldInfo *field, struct pos *positions, struct color *colors) {
	int i, j, obl;
	int n = field->nSize;

	int x = positions->fieldPositionX + 1;
	int y = positions->fieldPositionY + 1;

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			obl = j + i*n;
			if (field->cell[obl].data) {
				if (!field->cell[obl].isDefault) textcolor(colors->altTextColor);
				gotoxy(2*j +x,2*i +y);
				putch(field->cell[obl].data);
				textcolor(colors->textColor);
			}
		}
	}
}


// Podœwietlenie aktualnie zaznaczonej komórki na planszy
// Funkcja dla ³adniejszej ramki
void showBetterPosition(int x, int y, int n, struct pos *positions, struct color *colors) {
	int xf = positions->fieldPositionX + (x * 2) - 1;
	int yf = positions->fieldPositionY + (y * 2) - 1;

	int t1 = 197;
	int t2 = 196;
	int t3 = 197;
	int b1 = 197;
	int b2 = 196;
	int b3 = 197;
	int l = 179;
	int r = 179;

	if(x == 1) {
		t1 = 199;
		l = 186;
		b1 = 199;
	}
	if(x == n) {
		t3 = 182;
		r = 186;
		b3 = 182;
	}
	if(y == 1) {
		t1 = 209;
		t2 = 205;
		t3 = 209;
	}
	if(y == n) {
		b1 = 207;
		b2 = 205;
		b3 = 207;
	}
	if(x == 1 && y == 1) t1 = 201;
	if(x == 1 && y == n) b1 = 200;
	if(x == n && y == 1) t3 = 187;
	if(x == n && y == n) b3 = 188;

	textcolor(colors->positionColor);
	gotoxy(xf-1, yf-1);
	putch(t1); putch(t2); putch(t3);
	gotoxy(xf-1, yf);
	putch(l);
	//textbackground(colors->altBgColor);
	gotoxy(xf+1, yf);
	//putch(' ');
	//textbackground(colors->bgColor);
	putch(r);
	gotoxy(xf-1, yf+1);
	putch(b1); putch(b2); putch(b3);
	gotoxy(xf,yf);

	textcolor(colors->textColor);
}


// Wyœwietlanie pomocy dostêpnych znaków
void actuallyShowHelp (struct fieldInfo *field, int x, int y, struct pos *positions, struct color *colors) {
	int HPX = positions->helpPositionX;
	int HPY = positions->helpPositionY;
	int pos = x-1 + ((y-1) * field->nSize);
	char bufor[50];

	HPY+=2;

	if(field->cell[pos].isDefault == 0) {
		int zero1 = rule1(field, '0', x, y);
		int zero2 = rule2(field, '0', x, y);
		int zero3 = rule3(field, '0', x, y);

		int one1 = rule1(field, '1', x, y);
		int one2 = rule2(field, '1', x, y);
		int one3 = rule3(field, '1', x, y);

		gotoxy(HPX, HPY++);
		cputs("0: ");

		if(zero1 == 0 && zero2 == 0 && zero3 == 0) {
			textcolor(10);
			cputs("dostepne");
			textcolor(colors->textColor);
		} else {
			textcolor(12);
			cputs("niedostepne");
			textcolor(colors->textColor);
			if(zero1 == 1) {
				sprintf(bufor, "(1) W kolumnie %d beda wiecej", x);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz dwa zera w linii.");
			}
			else if(zero1 == 2) {
				sprintf(bufor, "(1) W wierszu %d beda wiecej", y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz dwa zera w linii.");
			}
			else if(zero1 == 3) {
				sprintf(bufor, "(1) W kolumnie %d i wierszu %d", x, y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("beda wiecej niz dwa zera w linii.");
			}

			if(zero2 == 1) {
				sprintf(bufor, "(2) W kolumnie %d bedzie wiecej", x);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz N/2 zer.");
			}
			else if(zero2 == 2) {
				sprintf(bufor, "(2) W wierszu %d bedzie wiecej", y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz N/2 zer.");
			}
			else if(zero2 == 3) {
				sprintf(bufor, "(2) W kolumnie %d i wierszu %d", x, y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("bedzie wiecej niz N/2 zer.");
			}

			if(zero3 < 100 && zero3 != 0) {
				sprintf(bufor, "(3) Kolumna %d bedzie taka sama", x);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				sprintf(bufor, "jak kolumna %d.", zero3);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
			}
			else if((zero3 & 127) == 0 && zero3 != 0) {
				sprintf(bufor, "(3) Wiersz %d bedzie taki sam", y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				sprintf(bufor, "jak wiersz %d.", zero3 >> 8);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
			}
			else if (zero3 != 0) {
				sprintf(bufor, "(3) Kolumna %d i wiersz %d beda takie", x, y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				sprintf(bufor, "same jak kolumna %d i wiersz %d.", zero3 & 127, zero3 >> 8);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
			}
		}

		gotoxy(HPX, HPY++);
		cputs("1: ");
		if(one1 == 0 && one2 == 0 && one3 == 0) {
			textcolor(10);
			cputs("dostepne");
			textcolor(colors->textColor);
		} else {
			textcolor(12);
			cputs("niedostepne");
			textcolor(colors->textColor);

			if(one1 == 1) {
				sprintf(bufor, "(1) W kolumnie %d beda wiecej", x);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz dwie jedynki w linii.");
			}
			else if(one1 == 2) {
				sprintf(bufor, "(1) W wierszu %d beda wiecej", y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz dwie jedynki w linii.");
			}
			else if(one1 == 3) {
				sprintf(bufor, "(1) W kolumnie %d i wierszu %d beda", x, y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("wiecej niz dwie jedynki w linii.");
			}

			if(one2 == 1) {
				sprintf(bufor, "(2) W kolumnie %d bedzie wiecej", x);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz N/2 jedynek.");
			}
			else if(one2 == 2) {
				sprintf(bufor, "(2) W wierszu %d bedzie wiecej", y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("niz N/2 jedynek.");
			}
			else if(one2 == 3) {
				sprintf(bufor, "(2) W kolumnie %d i wierszu %d", x, y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				gotoxy(HPX+3, HPY++);
				cputs("bedzie wiecej niz N/2 jedynek.");
			}

			if(one3 < 100 && one3 != 0) {
				sprintf(bufor, "(3) Kolumna %d bedzie taka sama", x);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				sprintf(bufor, "jak kolumna %d.", one3);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
			}
			else if((one3 & 127) == 0 && one3 != 0) {
				sprintf(bufor, "(3) Wiersz %d bedzie taki sam", y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				sprintf(bufor, "jak wiersz %d.", one3 >> 8);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
			}
			else if (one3 != 0) {
				sprintf(bufor, "(3) Kolumna %d i wiersz %d beda takie", x, y);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
				sprintf(bufor, "same jak kolumna %d i wiersz %d.", one3 & 127, one3 >> 8);
				gotoxy(HPX+3, HPY++);
				cputs(bufor);
			}
		}
	}
	else {
		gotoxy(HPX, HPY++);
		cputs("To pole jest");
		gotoxy(HPX, HPY++);
		textcolor(12);
		cputs("Niemodyfikowalne");
		textcolor(colors->textColor);
	}
}


// Funkcja przejœciowa dla pomocy
int help (struct fieldInfo *field, int x, int y, struct pos *positions, struct color *colors, int actuallyDoIt) {
	gotoxy(positions->helpPositionX + positions->helpWidth/2-4, positions->helpPositionY);
	cputs("- Pomoc -");

	if (actuallyDoIt == 2) actuallyShowHelp(field, x, y, positions, colors);

	printBetterFrame(positions->helpWidth+3, 16, positions->helpPositionX-2, positions->legendPositionY-1, 0);

	if (actuallyDoIt) return actuallyDoIt -1;
	else return 0;

}

// Zaznacza pola do których nie mo¿na wpisaæ ¿adnej wartoœci
int checkGameEnd (struct fieldInfo *field, struct pos *positions, struct color *colors, int helpMessage) {
	int n = field->nSize;
	int i, j, count = 0;

	if (helpMessage) {
		gotoxy(positions->helpPositionX, positions->helpPositionY+2);
		cputs("Wyswietlam pola, ktorych nie da");
		gotoxy(positions->helpPositionX, positions->helpPositionY+3);
		cputs("sie wypelnic.");
	}

	textbackground(LIGHTRED);

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if(!field->cell[i*n + j].data) {
				int zero = rule1(field, '0', j+1, i+1) + rule2(field, '0', j+1, i+1) + rule3(field, '0', j+1, i+1);
				int one = rule1(field, '1', j+1, i+1) + rule2(field, '1', j+1, i+1) + rule3(field, '1', j+1, i+1);

				if(zero && one) {
					gotoxy(positions->fieldPositionX + j*2 + 1, positions->fieldPositionY + i*2 + 1);
					putch(' ');
					++count;
				}
			}
		}
	}
	textbackground(colors->bgColor);
	return count;
};


// Wyœwietla iloœæ jedynek i zer przy kolumnach/wierszach
void showAdditionalHelp (struct fieldInfo *field, struct pos *positions, struct color *colors) {
	int i, j, n = field->nSize;
	int fpx = positions->fieldPositionX;
	int fpy = positions->fieldPositionY;
	int count = 0;

	textcolor(colors->altTextColor);
	gotoxy(fpx+ n*2 +1, fpy+ 2*n);
	putch(179);
	gotoxy(fpx+ n*2 +3, fpy+ 2*n);
	putch(179);
	gotoxy(fpx+ n*2, fpy+ 2*n +1);
	putch(196);
	putch('0');
	gotoxy(fpx+ n*2+3, fpy+ 2*n +1);
	putch(179);
	gotoxy(fpx+ n*2+3, fpy+ 2*n +2);
	putch(179);
	gotoxy(fpx+ n*2, fpy+ 2*n +3);
	putch(196);
	putch(196);
	putch(196);
	putch('1');
	textcolor(colors->textColor);

	// kolumny
	for (i = 0; i < n; ++i) {
		count = 0;
		for (j = 0; j < n; ++j) {
			if (field->cell[j*n+i].data == '0') ++count;
		}
		gotoxy(fpx+ i*2 +1, fpy+ 2*n +1);
		if(count < 10) putch(count+48);
		else {
			putch((count/10) + 48);
			gotoxy(fpx+ i*2 +1, fpy+ 2*n +2);
			putch(count % 10 + 48);
		}
		

		textcolor(colors->add1color);
		count = 0;
		for (j = 0; j < n; ++j) {
			if (field->cell[j*n+i].data == '1') ++count;
		}
		gotoxy(fpx+ i*2 +1, fpy+ 2*n +3);
		if(count < 10) putch(count+48);
		else {
			putch((count/10) + 48);
			gotoxy(fpx+ i*2 +1, fpy+ 2*n +4);
			putch(count % 10 + 48);
		}
		textcolor(colors->textColor);
	}

	// wiersze
	for (i = 0; i < n; ++i) {
		count = 0;
		for (j = 0; j < n; ++j) {
			if (field->cell[j+i*n].data == '0') ++count;
		}
		gotoxy(fpx+ 2*n +1, fpy+ 2*i +1);
		if(count < 10) putch(count+48);
		else {
			putch((count/10) + 48);
			putch(count % 10 + 48);
		}

		textcolor(colors->add1color);
		count = 0;
		for (j = 0; j < n; ++j) {
			if (field->cell[j+i*n].data == '1') ++count;
		}
		gotoxy(fpx+ 2*n +3, fpy+ 2*i +1);
		if(count < 10) putch(count+48);
		else {
			putch((count/10) + 48);
			putch(count % 10 + 48);
		}
		textcolor(colors->textColor);
	}
}

// Podœwietlenie jednoznacznych pól
int oneShallStand (struct fieldInfo **field, struct pos *positions, struct color *colors, int printOrWrite) {
	struct fieldInfo *a = *field;
	int n = a->nSize;
	int i = 0, j = 0;

	if(!printOrWrite){
		gotoxy(positions->helpPositionX, positions->helpPositionY+2);
		cputs("Wyswietlam pola, ktore da sie     ");
		gotoxy(positions->helpPositionX, positions->helpPositionY+3);
		cputs("wypelnic tylko na jeden sposob.");

		textcolor(LIGHTRED);

		for (i = 0; i < n; ++i) {
			for (j = 0; j < n; ++j) {
				if(!a->cell[i*n + j].data) {
					int zero = rule1(a, '0', j+1, i+1) + rule2(a, '0', j+1, i+1) + rule3(a, '0', j+1, i+1);
					int one = rule1(a, '1', j+1, i+1) + rule2(a, '1', j+1, i+1) + rule3(a, '1', j+1, i+1);

					if (zero) zero = 1;
					if (one) one = 1;

					if(!zero != !one) {
						gotoxy(positions->fieldPositionX + j*2 + 1, positions->fieldPositionY + i*2 + 1);
						if (zero) putch('1');
						else putch('0');
					}
				}
			}
		}
		textcolor(colors->textColor);
	}
	else{
		struct fieldInfo *tempField;
		tempField = allocateMemory(n);
		for (i = 0; i < n; ++i) {
			for (j = 0; j < n; ++j) {
				tempField->cell[i*n+j].data = a->cell[i*n+j].data;
				tempField->cell[i*n+j].isDefault = a->cell[i*n+j].isDefault;
				if(a->cell[i*n + j].data == 0) {
					int zero = rule1(a, '0', j+1, i+1) + rule2(a, '0', j+1, i+1) + rule3(a, '0', j+1, i+1);
					int one = rule1(a, '1', j+1, i+1) + rule2(a, '1', j+1, i+1) + rule3(a, '1', j+1, i+1);

					if (zero) zero = 1;
					if (one) one = 1;

					if(!zero != !one) {
						if (zero) cellInput(tempField, '1', 0, j+1, i+1, 1);
						else cellInput(tempField, '0', 0, j+1, i+1, 1);
					}
				}
			}
		}
		free(*field);
		*field = tempField;
	}
	return 0;
}
