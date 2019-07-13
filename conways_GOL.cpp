#include <windows.h> // xu ly nhung thu lien quan den Win32
#include <dirent.h> // xu ly cac ve PATH cac folder va file (directory)
#include <iostream>	// thu vien INPUT OUTPUT
#include <stdlib.h> // thu vien chuan C
#include <fstream>	// thu vien xu ly FILE
#include <conio.h> 	// xu ly nhung thu lien quan den Console cmd
#include <vector>	// cau truc du lieu VECTOR
#include <set>
#include <string>
#include <vector>
#include <time.h>

#define _black 	0
#define _blue 	1
#define _green 	2
#define _aqua 	3
#define	_red	4
#define _purple	5
#define	_yellow	6
#define _white	7
//
#define	_Black	8
#define	_Blue 	9
#define _Green 	10
#define	_Aqua 	11
#define _Red 	12
#define _Purple 13
#define _Yellow 14
#define _White 	15

// -- Screen constants
const unsigned W = 59, H = 35;

// -- Borders
unsigned startX = 3, startY = 2;
unsigned X = 3, Y = 2;

unsigned endX = startX + H - 1;
unsigned endY = startY + W - 1;

// -- Live/dead matrix
bool cell[H + 100][W + 100];

// -- Alive cells
unsigned alive;

// -- Detect game state
long long Hash;
std::set<long long> Hashes;
bool repFlag = 0;

// -- Misc.
std::vector< std::vector< std::string > > quotesAboutImmortality {
	{"\"That man has reached immortality", "who is disturbed by nothing material.\""},
	{"\"Man that never dies doesn't truely live.\""},
	{"\"We turn not older with years, but newer every day.\""},
	{"\"Is a repeated life worth living?\""},
	{"\"Oh how wrong we were to think immortality meant never dying.\""},
	{"\"I want to be remembered. Not to live forever.\""},
	{"\"i want die\""}
};
int quoteID_immrt = -1;

std::vector< std::vector< std::string > > quotesAboutDeath {
	{"\"Death is the end and the beginning.\""},
	{"\"God is dead.\""},
	{"\"people said the world ending in 2012 was fake", "but has anyone felt alive since then\""},
	{"\"There are three deaths.", "The first is when the body ceases to function.", "The second is when the body is consigned to the grave.", "The third is that moment, sometime in the future,", "when your name is spoken for the last time.\""},
	{"\"Three can keep a secret, if two of them are dead.\""},
	{"\"i might look fly but inside i just want to die\""},
	{"\"Death is not the opposite of life, but part of it.\"",},
	{"\"I like sleeping. It's the closest to death that I can ever be.\""},
	{"\"People like us aren't qualified to be involved", "in a dramatic incident such as a suicide.", "No matter how depressed you are or how much pain you're in,", "you have to return to your routine, daily life.", "Even if you don't come back, you'll just end up dying in vain.", "A dramatic death isn't befitting of us.\""}
};
int quoteID_death = -1;

//-------------------------------------------------------------------//

// -- Fundamental functions
int checkBound(unsigned X, unsigned Y);
int moveCursor(unsigned *X, unsigned *Y);

// -- Visual functions
void gotoXY(short x, short y);
void SetColor(int cl){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cl);
}

void display(int tickcount){
	alive = 0;
	//
	SetColor(_Green);
	for(unsigned ix = startX; ix <= endX; ix++)
		for(unsigned iy = startY; iy <= endY; iy++)
		{
			gotoXY(ix, iy);
			if(cell[ix][iy]) {printf("@"); alive++;}
			else printf(" ");
		}
	SetColor(_White);
	//
	gotoXY(10, 65); printf("Tick: %d          ", tickcount);
	gotoXY(11, 65); printf("Alive cells: %d          ", alive);
	SetColor(_white);
}

//-- Game canvas functions
void initCanvas();
void initGameState();
void announceGameStart();
void announceGameOver();
void announceGameEverlasting();
void sayQuoteImmortal();
void sayQuoteDeath();

//-- Game's Under-the-system functions
int state(int x, int y){
	int neighbours = 0;
	for(int i = -1; i < 2; i++)
	  for(int j = -1; j < 2; j++)
		if(!!i || !!j) neighbours += cell[x + i][y + j];
	//--
	if(cell[x][y]) return ((neighbours == 2) || (neighbours == 3));
	else return (neighbours == 3);
}

void tick(){
	bool cellAfter[H + 100][W + 100];
	//
	for(unsigned ix = startX; ix <= endX; ix++)
		for(unsigned iy = startY; iy <= endY; iy++)
			cellAfter[ix][iy] = state(ix, iy);
	//
	for(unsigned ix = startX; ix <= endX; ix++)
		for(unsigned iy = startY; iy <= endY; iy++)
			cell[ix][iy] = cellAfter[ix][iy];
}

void checkCurrentHash(){
	Hash = 1;
	for(unsigned ix = startX; ix <= endX; ix++)
		for(unsigned iy = startY; iy <= endY; iy++){
			Hash <<= 1;
			Hash += cell[ix][iy];
			Hash %= 1011001110001111; //prime
		}
	//
	if(Hashes.find(Hash) == Hashes.end()) Hashes.insert(Hash);
	else repFlag = 1;
}

//-------------------------------------------------------------------//

int main(){
	//
	initCanvas();
	initGameState();
	announceGameStart();
	//
	int tickcount = 0;
	do {
		Sleep(100);
		tickcount++;
		//
		if(!repFlag) checkCurrentHash();
		else{
			if(quoteID_immrt == -1) announceGameEverlasting();
			else sayQuoteImmortal();
		}
		tick();
		//
		display(tickcount);
		//
		gotoXY(1, 0);
	} while(alive);
	//
	announceGameOver();
	sayQuoteDeath();
	return 0;
}

//-------------------------------------------------------------------//

int checkBound(unsigned X, unsigned Y){
	return ((startX <= X) && (X <= endX)) && ((startY <= Y) && (Y <= endY));
}

int moveCursor(unsigned *X, unsigned *Y)
{
	char c;
	//do
	{
		c = getch();
		//std::cout << getch() << " " << getch() << " ";
		//std::cout << int(c) << " ";
		if (int(c) == -32) { // if the first value is esc
		    //getch(); // skip the [
		    switch( int(getch()) ) { // the real value
		        case 72:
		            if (checkBound( (*X)-1, (*Y) )) (*X)--;
		            break;
		        case 80:
		            if (checkBound( (*X)+1, (*Y) )) (*X)++;
		            break;
		        case 75:
		            if (checkBound( (*X), (*Y)-1 )) (*Y)--;
		            break;
		        case 77:
		            if (checkBound( (*X), (*Y)+1 )) (*Y)++;
		            break;
		    }
		    gotoXY((*X),(*Y));
		}
		else{
			
			if(int(c) == 0) {
				char cc = getch();
				if(int(cc) == 107) return -1;
				if(int(cc) == 63)  return -2;
			}
			//
			switch(int(c)){
				case 32:	// SPACE
					return 1;
				case 13:	// ENTER
					return 2;
				default:
					return 3;
			}
		}
	}
	return 0;
}

void initCanvas(){
	SetColor(_White);
	gotoXY(0, 1); 	printf("Conway's Game of Life");
	SetColor(_white);
	gotoXY(1, 2);	printf("0     1     2     3     4     5     6     7     8     9 ");
	gotoXY(2, 2);	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -+");
	gotoXY(2, 1);	printf("+");
	gotoXY(3, 0);	for(int i = 0; i < 7; i++)
		printf("%d|                                                           |\n |                                                           |\n                                                              \n |                                                           |\n                                                              \n", i);
	printf(" +- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -+");
}

void initGameState(){
	SetColor(_White);
	gotoXY(2, 65);	printf("INPUT initial game state..");
	gotoXY(3, 65);	printf("Please place your initial cells.");
	gotoXY(4, 65);	printf("================================");
	gotoXY(5, 65);	printf("> Move with arrows key.");
	gotoXY(6, 65);	printf("> Place/unplace with SPACE");
	gotoXY(7, 65);	printf("> Finish with ENTER.");
	SetColor(_Green);
	// 59 x 35
	gotoXY(startX, startY);
	//
	while(1){
		int signal = moveCursor(&X, &Y);
		if(signal == 1){
			if(cell[X][Y] ^= 1) printf("@");
			else printf(" "); 
		}
		if(signal == 2) return;
		gotoXY(X, Y);
	}
	SetColor(_white);
}

void announceGameStart(){
	SetColor(_White);
	gotoXY(8, 65);	printf("================================");
	gotoXY(9, 65);	printf("Game is running...");
	SetColor(_white);
}

void announceGameOver(){
	SetColor(_White);
	gotoXY(13, 65);	printf("================================");
	gotoXY(14, 65); printf("Your \"EMPIRE\" has gone extinct!");
	//
	srand(time(NULL));
	quoteID_death = (rand() % quotesAboutDeath.size());
}

void announceGameEverlasting(){
	SetColor(_White);
	gotoXY(13, 65);	printf("================================");
	gotoXY(14, 65); printf("Your \"EMPIRE\" has entered a loop!");
	//
	srand(time(NULL));
	quoteID_immrt = (rand() % quotesAboutImmortality.size());
}

void sayQuoteImmortal(){
	//
	SetColor(_Yellow);
	X = 16, Y = 65;
	for(std::string st : quotesAboutImmortality[quoteID_immrt]){
		gotoXY(X, Y);
		std::cout << "| " << st;
		X++;
	}
	//
	SetColor(_white);
}

void sayQuoteDeath(){
	//
	SetColor(_Yellow);
	X = 16, Y = 65;
	for(std::string st : quotesAboutDeath[quoteID_death]){
		gotoXY(X, Y);
		std::cout << "| " << st;
		X++;
	}
}

void gotoXY(short x, short y)
{
    static HANDLE h = NULL;
    if(!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = {y,x};
    SetConsoleCursorPosition(h,c);
}
