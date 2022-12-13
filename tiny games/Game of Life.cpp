#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define High 25
#define Width 50

int Cell[High][Width];
int pause,speed,times;

void gotoxy(int,int);
void startup();
void updateWithInput();
void updateWithoutInput();
void show();
void HideCursor();
void restartup();

int main(int argc,char*argv[]){
	startup();
	while(1){
		show();
		updateWithInput();
		updateWithoutInput();
	}
	return 0;
} 

void startup(){
	int i,j;
	for(i=0;i<High;i++){
		for(j=0;j<Width;j++)
		Cell[i][j]=rand()%2;
	}
	pause=0,speed=1000,times=0;
}

void restartup(){
	system("cls");
	int i,j;
	for(i=0;i<High;i++){
		for(j=0;j<Width;j++)
		Cell[i][j]=rand()%2;
	}
	times=0;
}

void show(){
	int i,j;
	gotoxy(0,0);
	HideCursor();
		
	if(!pause){
		for(i=0;i<High;i++){
		for(j=0;j<Width;j++)
		if(Cell[i][j]==1) putchar('*');
		else putchar(' ');
		
		putchar('\n');
	}
		times++;
		printf("ÖÜÆÚ%d",times);
	}	
	Sleep(speed);
}

void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void HideCursor(){
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void updateWithoutInput(){
	int NewCell[High][Width];
	int i,j,NeiborCellNumber;
	for(i=1;i<High-1;i++){
		for(j=1;j<Width-1;j++){
			NeiborCellNumber=Cell[i-1][j-1]+Cell[i-1][j]+Cell[i-1][j+1]+Cell[i][j-1]+Cell[i][j+1]+Cell[i+1][j-1]+Cell[i+1][j]+Cell[i+1][j+1];	
			
			if(NeiborCellNumber==3) NewCell[i][j]=1;
			else if(NeiborCellNumber==2) NewCell[i][j]=Cell[i][j];
			else NewCell[i][j]=0;
		}
	}
	
	for(i=1;i<High-1;i++){
		for(j=1;j<Width-1;j++){
			Cell[i][j]=NewCell[i][j];
		}
	}
}

void updateWithInput(){
	char key;
	if(kbhit()){
		key=getch();
		switch(key){
			case ' ':if(pause==0) pause=1;
					 else pause=0; 
					 break;
			case 'R':restartup();
					 break;
			case '+':if(speed>50)
					 speed-=100;
					 break;
			case '-':speed+=100;
					 break;
			default: break;
		}
	}
}

















