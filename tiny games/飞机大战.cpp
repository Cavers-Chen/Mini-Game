#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <conio.h>

int position_x,position_y;
int bullet_x,bullet_y;
int enemy_x,enemy_y;
int score;
const int high=30,width=20; 

void startup();
void show();
void updataWithoutInput();
void updataWithInput();
void gotoxy(int,int);
void HideCursor();

int main(int argc,char*argv[]){
	
	startup();
	while(1){
		HideCursor();
		show();
		updataWithoutInput();
		updataWithInput();
	}
	return 0;
}

void startup(){
	position_x=width/2;
	position_y=high/2;
	bullet_x=position_x;
	bullet_y=-1;
	enemy_x=position_x;
	enemy_y=0;
	score=0;
}

void show(){
	gotoxy(0,0);
	system("cls");
	int i,j;
	for(i=0;i<high;i++){
		for(j=0;j<width;j++){
		if(i==position_y&&j==position_x) putchar('*');
		else if(i==enemy_y&&j==enemy_x) putchar('@');
		else if(i==bullet_y&&j==bullet_x) putchar('!');	
		else if(j==0||j==width-1) putchar('|');
		else putchar(' ');
		}
		putchar('\n');
	}
	printf("Score:%d",score);
}

void updataWithoutInput(){
	static int speed=0;
	if(speed<10) speed++;
	else{speed=0;
		enemy_y++;}
	
	if(bullet_y>-1) 
		bullet_y--;
	
	if(bullet_y==enemy_y&&bullet_x==enemy_x){
		score++;
		bullet_y=-1;
		enemy_x=rand()%10+5;
		enemy_y=0;
	}
}

void updataWithInput(){
	char key;
	if(kbhit()){
		key=getch();
		switch(key){
		case 'a':if(position_x==0) break;
					position_x--;
					break;
		case 'd':if(position_x==width-1) break;
					position_x++;
					break;
		case 'w':if(position_y==5) break;
					position_y--;
					break;
		case 's':if(position_y==high-1) break;
					position_y++;
					break;
		case 'j':bullet_x=position_x;
				 bullet_y=position_y-1;	
				 break;
		default:break;	
		}
	}
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


