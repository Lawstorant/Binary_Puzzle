#ifndef STRUCTURES_H
#define STRUCTURES_H

struct rndata {
	int rndBot;
	int rndTop;
	int tries;
};

struct color {
	int textColor;
	int altTextColor;
	int positionColor;
	int bgColor;
	int altBgColor;
	int add1color;
};

struct pos {
	int fieldPositionX;
	int fieldPositionY;
	int legendPositionX;
	int legendPositionY;
	int helpPositionX;
	int helpPositionY;
	int legendWidth;
	int helpWidth;
};

struct messeges {
	int help;
	int help2;
	int end;
	int one;
	int automat;
};

// Zawartosc komórki na planszy
struct cells {
	char data;
	int isDefault;
};

// Struktura planszy
struct fieldInfo {
	int nSize;
	struct cells cell[];
};

#endif
