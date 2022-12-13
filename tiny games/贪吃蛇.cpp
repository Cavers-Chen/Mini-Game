#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

const int High=40,Width=40;
int body_len,head_x,head_y,food_x,food_y,food_reside;
int screen[High][Width]; 
int score,pause;
char key,keyb;

void startup();
void HideCursor();
void gotoxy(int,int);
void show();
void updateWithInput();
void updateWithoutInput();
void death();

int main(int argc,char*argv[]){
	startup();
	while(1){
		HideCursor();
		show();
		updateWithInput();
		updateWithoutInput();
	}
	return 0;
} 

void startup(){
	body_len=4;
	head_x=Width/2,head_y=High/2;
	score=0,pause=0;
	food_x=rand()%(Width-2)+1;
	food_y=rand()%(High-2)-1;
	food_reside=1;
	
	for(int i=0;i<High;i++){
		for(int j=0;j<Width;j++){
			screen[i][j]=0; 
		}
	}
	
	screen[food_y][food_x]=-1;
	
	for(int i=head_x,j=1;i>=head_x-body_len+1;i--,j++) screen[head_y][i]=j;
	show();
	while(1){
		if(kbhit()){
		keyb=getch();
		if(keyb=='w'||keyb=='s'||keyb=='d')
		break;	
		}	
	}
	
}

void HideCursor(){
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void show(){
	gotoxy(0,0);
	for(int i=0;i<High;i++){
		for(int j=0;j<Width;j++){
			if(i==0||i==High-1||j==0||j==Width-1) putchar('*');
			else if(screen[i][j]==0) putchar(' ');
			else if(screen[i][j]==1) putchar('@');
			else if(screen[i][j]>1)  putchar('+');
			else if(screen[i][j]==-1)putchar('F');
		}
		putchar('\n');
	}
	printf("Score:%d",score);
}

void updateWithInput(){
	static int food_time=0;
		if(food_time<0) food_time++;
		else if(!food_reside){
		food_x=rand()%(Width-2)+1;
		food_y=rand()%(High-2)+1;
		screen[food_y][food_x]=-1;
		food_time=0;
		food_reside=1;
	}
	
	if(kbhit()){
		key=getch();
		switch(key){
			case 'a':if(keyb=='d'){
					 head_x++;
					 break;
					}else{
					 head_x--;
					 keyb='a';
					 break;
					}	 
			case 'd':if(keyb=='a'){
					 head_x--;
					 break;
					}else{
					 head_x++;
					 keyb='d';
					 break;
					}
			case 'w':if(keyb=='s'){
					 head_y++;
					 break;
					}else{
					 head_y--;
					 keyb='w';
					 break;
					}
			case 's':if(keyb=='w'){
					 head_y--;
					 break;
					}else{
					 head_y++;
					 keyb='s';
					 break;
					}
			case 'p':if(pause==1) pause=0;
					 else pause=1;
			defalut: break;
		}	
	}else{
		if(!pause)
		switch(keyb){
			case 'a':head_x--;
					 break;
			case 'd':head_x++;
					 break;
			case 'w':head_y--;
					 break;
			case 's':head_y++;
					 break;
			default: break;
		}
	}
	
	if(screen[head_y][head_x]>1||head_x==0||head_x==Width-1||head_y==0||head_y==High-1)	death();	
	else if(screen[head_y][head_x]==-1){
		score++;
		body_len++;
		screen[head_y][head_x]=1;
		food_reside=0;
		}
		
}


void updateWithoutInput(){
	//static int speed=0;
	
	//if(speed<0) speed++;
	//else{
		if(!pause)
		for(int i=0;i<High;i++){
			for(int j=0;j<Width;j++){
				if(screen[i][j]>=1){
				screen[i][j]++;
				if(screen[i][j]>body_len) 
				screen[i][j]=0;
				}else if(i==head_y&&j==head_x&&screen[i][j]==0){
					screen[i][j]=1;
				}
			}
		}
	//	speed=0;
	//}
	
}

void death(){
	Sleep(1000);
	system("cls");
	puts("Game Over");
	Sleep(3000);
	system("cls");
	printf("Your Score:%d",score);
	exit(1);
}
