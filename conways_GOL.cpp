#include <windows.h> // xu ly nhung thu lien quan den Win32
#include <dirent.h> // xu ly cac ve PATH cac folder va file (directory)
#include <iostream>	// thu vien INPUT OUTPUT
#include <stdlib.h> // thu vien chuan C
#include <fstream>	// thu vien xu ly FILE
#include <conio.h> 	// xu ly nhung thu lien quan den Console cmd
#include <vector>	// cau truc du lieu VECTOR
#include <set>

const unsigned W = 59, H = 35;

unsigned startX = 3, startY = 2;
unsigned X = 3, Y = 2;

unsigned endX = startX + H - 1;
unsigned endY = startY + W - 1;

bool cell[H + 100][W + 100];

//-------------------------------------------------------------------//

// -- Fundamental functions
int checkBound(unsigned X, unsigned Y);
int moveCursor(unsigned *X, unsigned *Y);

// -- Visual functions
void gotoXY(short x, short y);
void clrscr(int sh, int sv, int h, int v);

void display(int tickcount){
	int alive = 0;
	//
	for(unsigned ix = startX; ix <= endX; ix++)
		for(unsigned iy = startY; iy <= endY; iy++)
		{
			gotoXY(ix, iy);
			if(cell[ix][iy]) {printf("#"); alive++;}
			else printf(" ");
		}
	//
	gotoXY(10, 65); printf("Tick: %d          ", tickcount);
	gotoXY(11, 65); printf("Alive cells: %d          ", alive);
}

//-- Game functions
void initCanvas();
void initGameState();
void announceGameStart();

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

//-------------------------------------------------------------------//

int main(){
	initCanvas();
	initGameState();
	announceGameStart();
	//
	int tickcount = 0;
	while(1){
		tickcount++;
		Sleep(100);
		tick();
		display(tickcount);
		gotoXY(0, 0);
	}
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
	gotoXY(0, 1); 	printf("Conway's Game of Life");
	gotoXY(1, 2);	printf("0     1     2     3     4     5     6     7     8     9 ");
	gotoXY(2, 2);	printf("-----------------------------------------------------------+");
	gotoXY(2, 1);	printf("+");
	gotoXY(3, 0);	for(int i = 0; i < 7; i++)
		printf("%d|                                                           |\n |                                                           |\n |                                                           |\n |                                                           |\n |                                                           |\n", i);
	printf(" +-----------------------------------------------------------+");
}

void initGameState(){
	gotoXY(2, 65);	printf("INPUT initial game state..");
	gotoXY(3, 65);	printf("Please place your initial cells.");
	gotoXY(4, 65);	printf("================================");
	gotoXY(5, 65);	printf("> Move with arrows key.");
	gotoXY(6, 65);	printf("> Place/unplace with SPACE");
	gotoXY(7, 65);	printf("> Finish with ENTER.");
	// 59 x 35
	gotoXY(startX, startY);
	//
	while(1){
		int signal = moveCursor(&X, &Y);
		if(signal == 1){
			if(cell[X][Y] ^= 1) printf("#");
			else printf(" "); 
		}
		if(signal == 2) return;
		gotoXY(X, Y);
	}
}

void announceGameStart(){
	gotoXY(8, 65);	printf("================================");
	gotoXY(9, 65);	printf("Game is running...");
}

void gotoXY(short x, short y)
{
    static HANDLE h = NULL;
    if(!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = {y,x};
    SetConsoleCursorPosition(h,c);
}

void clrscr(int sh, int sv, int h, int v)
{
	char emptyline[v+2];
	memset(emptyline, ' ', v);
	emptyline[v] = '\n';
	emptyline[v+1] = '\0';

	gotoXY(sh, sv);
	for(int i = 0; i < h; i++) {
		//for(int j = 0; j < v; j++) std::cout << ' ';
		//std::cout << "\n";
		printf("%s", emptyline);
	}
	gotoXY(2,0);
}
