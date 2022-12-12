#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

const int High=25,Width=30;
int level;
char key,keyb;
int block_x,block_y,radius;
int dude_x,dude_y;
int screen[High+2][Width];
int fall,life,pause;

void startup();
void HideCursor();
void gotoxy(int,int);
void show();
void updateWithInput();
void updateWithoutInput();
void death();
void wingame();

int main(int argc,char*argv[]){
	startup();
	while(1){
		HideCursor();
		if(!pause){
			show();
			updateWithoutInput();
		}
		updateWithInput();

	}
	return 0;
}

void startup(){
	level=0,life=5;
	fall=0,pause=0;
	dude_x=Width/2,dude_y=High/2;
	block_x=Width/2,block_y=High/2+1;
	radius=3;
	for(int i;i<High+2;i++){
		for(int j;j<Width;j++){
			screen[i][j]=0;
		}
	}
	for(int i=block_x-radius;i<=block_x+radius;i++){
		screen[block_y][i]=1;
	}
	
}

void show(){
	gotoxy(0,0);
	for(int i=0;i<High;i++){
		for(int j=0;j<Width;j++){
			if(j==0||j==Width-1) putchar('|');
			else if(i==0) putchar('T');
			else if(screen[i][j]==1) putchar('-');
			else if(j==dude_x&&i==dude_y-2) putchar('o');
			else if(j==dude_x-1&&i==dude_y-1){
				printf("/|\\");
				j+=2; }
			else if(j==dude_x-1&&i==dude_y){
				printf("/ \\");
				j+=2; }
			else putchar(' ');
		}
		putchar('\n');
	} 
	printf("当前层数：%d\n剩余生命值：%d",level,life-1);
}

void updateWithInput(){
		if(kbhit()){
			key=getch();
			switch(key){
				case 'a':if(dude_x==2) break;
						 keyb='a';
						 break;
				case 'd':if(dude_x==Width-3) break;
						 keyb='d';
						 break;
				case 'j':if(fall) break;
						 dude_y-=4;
						 break;
				case 'p':if(pause==1) pause=0;
						 else pause=1;
				default: break;
			}
		}
}

void updateWithoutInput(){
	static int speed=0,speed2=0,level1=0,highlevel=0;
	static float G=1,speed_fly=0;
	static int block_time=0,wudi=0;
	
	if(screen[dude_y+1][dude_x-3]==0&&screen[dude_y+1][dude_x-2]==0&&screen[dude_y+1][dude_x-1]==0&&screen[dude_y+1][dude_x]==0&&screen[dude_y+1][dude_x+1]==0&&screen[dude_y+1][dude_x+2]==0&&screen[dude_y+1][dude_x+3]==0) fall=1;
	else if(dude_y-2==0){
		fall=1;
		if(wudi==0){
			wudi=1;
			life--;
		}
		if(!life){
			death();
		}
	}else if(dude_y>=High+1){
		death();
	}else{
		fall=0;
		wudi=0;
		speed_fly=1;
	}
	
	if(speed<10) speed++;
	else {
		for(int i=0;i<High;i++){
		for(int j=0;j<Width;j++){
			if(screen[i][j]==1&&i!=0){
				screen[i][j]=0;
				screen[i-1][j]=1;
			}
			if(screen[i][j]==1&&i==0) screen[i][j]=0;
		}
	}
	if(!fall)dude_y--;
		
		switch(keyb){
				case 'a':if(dude_x==2) break;
						 dude_x--;
						 break;
				case 'd':if(dude_x==Width-3) break;
						 dude_x++;
						 break;
				default: break;
			}	
		speed=0;
	}
	if(speed2<2) speed2++;
	else{
		if(fall){
			if(speed_fly<1){
				speed_fly+=G;
				G+=1;
			}else{
			dude_y++;
			speed_fly=0;
			G=1;	
			} 
		}
		speed2=0;
	}		
	
	if(block_time<50){
		block_time++;
	}else{
		block_time=0;
		block_x=rand()%22+4;			
		for(int i=block_x-radius;i<=block_x+radius;i++)
		screen[High-1][i]=1;
	}
	
				
	for(int i=0;i<Width;i++){
		if(screen[dude_y][i]==1){
			level1++;
			break;
		}
	}if(level1==6){
		level++;
		level1=0;
	}
	if(level>=100&&!fall) wingame();
	
	life+=level/10-highlevel/10;
	highlevel=level;	
}

void death(){
	while(dude_y-2<High){
		dude_y++;
	}
	Sleep(2000);
	system("cls");
	puts("Game Over");
	puts("Too Weak!");
	exit(1);
}

void wingame(){
	Sleep(2000);
	system("cls");
	puts("Good Job, You Are A Ture Man !");
	exit(1);
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
