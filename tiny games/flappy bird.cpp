#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

const int high=15,width=20;
int bird_x,bird_y;
int bar_bottom,bar_top,bar_x; 
int score,length,velocity,G;

void updateWithInput();
void updateWithoutInput();
void show();
void gotoxy(int,int);
void startup();
void death();

int main(int argc,char*argv[]){
	startup();
	while(1){
		CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		show();
		updateWithoutInput();
		updateWithInput();
	}
	return 0;
} 

void startup(){
	bird_x=width/3;
	bird_y=1;
	bar_x=width-1;
	bar_bottom=high*2/3;
	bar_top=high/3;
	score=0,length=0,velocity=15,G=80;
}

void show(){
	int i,j;
	gotoxy(0,0);
	for(i=0;i<high;i++){
		for(j=0;j<width;j++){
			if(i==0||i==high-1) putchar('+');
			else if(j==bar_x&&i<=bar_top||i>=bar_bottom&&j==bar_x) putchar('*');
			else if(j==bird_x&&i==bird_y) putchar('@');
			else putchar(' ');
		}
		putchar('\n');
	}
	printf("Score:%d\nLength:%dm",score,length);
}

void updateWithoutInput(){
	int static speed_bird=0,speed_bar=0,flag=0;
	if(speed_bird<G) speed_bird++;
	else{
		bird_y++;
		speed_bird=0;
	}	
	if(speed_bar<velocity) speed_bar++;
	else{
		bar_x--;
		length++;
		speed_bar=0;
	}
	
	if(flag==0&&bird_x>bar_x){
	score++;
	flag=1;	
	if(velocity>3) velocity-=3; 
	} 
	
	if(bar_x<0){
	bar_x=width;
	int temp=rand()%int(high*0.8);
	bar_bottom=temp+2;
	bar_top=temp-2;
	flag=0;	
	} 
	
	if(bird_x==bar_x&&bird_y<=bar_top||bird_x==bar_x&&bird_y>=bar_bottom||bird_y>=high||bird_y<=0) death();
}

void updateWithInput(){
	char key;
	
	if(kbhit()){
		key=getch();
		if(key==' '){
			bird_y-=2;
			G=80;
		}
	}else if(G>5)G--;
}

void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void death(){
	Sleep(1000);
	system("cls");
	puts("Game Over");
	Sleep(1000);
	system("cls");
	printf("Score:%d\nLength:%d",score,length);
	exit(1);
}





















