#include <stdio.h>
#include <windows.h>    
#include <conio.h> 		
#include <stdlib.h>  	

int ball_x,ball_y,ball_vx,ball_vy;
int board_x,board_y,block_l,block_r,radius,block_radius; 
int block_x,block_y;
int ball_num,score,Life,Alive;
const int high=20,width=25;

void startup();
void show();
void updataWithoutInput();
void updataWithInput();
void gotoxy(int x,int y);

int main(int argc,char*argv[]){
	startup();
	while(1){
		CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		show();
		updataWithoutInput();
		updataWithInput();
	}
	return 0;
}

void startup(){
	ball_x=width/2;
	ball_y=high-3;
	ball_vx=1;
	ball_vy=1;
	block_x=width/2;
	block_y=1;
	block_l=0;
	block_r=0;
	board_x=width/2;
	board_y=high;
	ball_num=0;
	score=0;
	radius=3;
	block_radius=2;
	Life=3;
	Alive=1;
}

void show(){
	int i,j;
	gotoxy(0,0);
	for(i=0;i<high;i++){
		for(j=0;j<width;j++){
			if(i==0) putchar('-');
			else if(j==0||j==width-1)  putchar('|');
			else if(i==block_y&&j>=block_x-block_radius&&j<=block_x+block_radius) putchar('B');
			else if(ball_x==j&&ball_y==i) putchar('o');
			else if(i==high-1&&j>=(board_x-radius)&&j<=(board_x+radius)) putchar('*');
			else putchar(' ');
		}
		putchar('\n');
	}
	printf("Score:%d\nBallBounceTime:%d\nLife:%d",score,ball_num,Life); 
} 

void updataWithoutInput(){
	static int speed=0,flag=0;
	if(speed<10) speed++;
	else {
	ball_x+=ball_vx;
	ball_y+=ball_vy;
	speed=0;	
	}
	
	if(ball_x>=width-1||ball_x<=0) ball_vx=-ball_vx;
	if(ball_y<=0) ball_vy=-ball_vy;
	if(ball_y==high-2&&ball_x>=board_x-radius&&ball_x<=board_x+radius){
		ball_vy=-ball_vy;
		flag=1;
		if(block_l){
		 ball_vx--;
		}else if(block_r){
		 ball_vx++;
		}
		}	
		
	if(flag==1&&ball_y<high-2){
		flag=0;
		ball_num++;
	}
		
	if(ball_x>=block_x-block_radius&&ball_x<=block_x+block_radius&&block_y==ball_y){
		ball_vy=-ball_vy;
		ball_vx=-ball_vx;
		block_x=rand()%23+1;
		block_y=rand()%8+2;
		putchar('\a');
		score++;
	}
	
	if(ball_y>high){
		if(--Life){
			ball_x=board_x;
			ball_y=high-2;
			Alive=0;
			ball_vx=0;
			ball_vy=0;
		}else{
			system("cls");
			puts("Game Over!");
			Sleep(3000);
			printf("Your Score:%d\n",score);
			exit(1);
		}
	}	
}

void updataWithInput(){
	char key;
	if(Alive){
		if(kbhit()){
		key=getch();
		switch(key){
			case 'a':if(board_x==radius+1) break;
					 board_x--;
					 block_l++;
					 block_r=0;
					 break;
			case 'd':if(board_x==width-radius-2)break;
					 board_x++;
					 block_l=0;
					 block_r++;
					 break;
			default: block_r=0;
					 block_l=0;
					 break;
		}
	}else{block_r=0;
		  block_l=0;		
	}
	}else{
				if(kbhit()){
		key=getch();
		switch(key){
			case 'a':if(board_x==radius+1) break;
					 board_x--;
					 ball_x--;				 
					 break;
			case 'd':if(board_x==width-radius-2)break;
					 board_x++;
					 ball_x++;
					 break;
			case ' ':Alive=1;
					 ball_vy=-1;
			default: break;
		}
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

/*#include <stdio.h>
#include <windows.h>    
#include <conio.h> 		
#include <stdlib.h>  	

int ball_x,ball_y,ball_vx,ball_vy;
int board_x,board_y,block_l,block_r,radius,block_radius; 
int block_x,block_y;
int ball_num,score,Life,Alive;
const int high=20,width=25;

void startup();
void show();
void updataWithoutInput();
void updataWithInput();
void gotoxy(int x,int y);

int main(int argc,char*argv[]){
	startup();
	while(1){
		CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		show();
		updataWithoutInput();
		updataWithInput();
	}
	return 0;
}

void startup(){
	ball_x=width/2;
	ball_y=high-3;
	ball_vx=1;
	ball_vy=1;
	block_x=width/2;
	block_y=1;
	block_l=0;
	block_r=0;
	board_x=width/2;
	board_y=high;
	ball_num=0;
	score=0;
	radius=3;
	block_radius=2;
	Life=3;
	Alive=1;
}

void show(){
	int i,j;
	gotoxy(0,0);
	for(i=0;i<high;i++){
		for(j=0;j<width;j++){
			if(i==0) putchar('-');
			else if(j==0||j==width-1)  putchar('|');
			else if(i==block_y&&j>=block_x-block_radius&&j<=block_x+block_radius) putchar('B');
			else if(ball_x==j&&ball_y==i) putchar('o');
			else if(i==high-1&&j>=(board_x-radius)&&j<=(board_x+radius)) putchar('*');
			else putchar(' ');
		}
		putchar('\n');
	}
	printf("Score:%d\nBallBounceTime:%d\nLife:%d",score,ball_num,Life); 
} 

void updataWithoutInput(){
	static int speed=0,flag=0;
	if(speed<10) speed++;
	else {
	ball_x+=ball_vx;
	ball_y+=ball_vy;
	speed=0;	
	}
	
	if(ball_x>=width-1||ball_x<=0) ball_vx=-ball_vx;
	if(ball_y<=0) ball_vy=-ball_vy;
	if(ball_y==high-2&&ball_x>=board_x-radius&&ball_x<=board_x+radius){
		ball_vy=-ball_vy;
		flag=1;
		if(block_l){
		 ball_vx--;
		}else if(block_r){
		 ball_vx++;
		}
		}	
		
	if(flag==1&&ball_y<high-2){
		flag=0;
		ball_num++;
	}
		
	if(ball_x>=block_x-block_radius&&ball_x<=block_x+block_radius&&block_y==ball_y){
		ball_vy=-ball_vy;
		ball_vx=-ball_vx;
		block_x=rand()%23+1;
		block_y=rand()%8+2;
		putchar('\a');
		score++;
	}
	
	if(ball_y>high){
		if(--Life){
			ball_x=board_x;
			ball_y=high-2;
			Alive=0;
			ball_vx=0;
			ball_vy=0;
		}else{
			system("cls");
			puts("Game Over!");
			Sleep(3000);
			printf("Your Score:%d\n",score);
			exit(1);
		}
	}	
}

void updataWithInput(){
	char key;
	if(Alive){
		if(kbhit()){
		key=getch();
		switch(key){
			case 'a':if(board_x==radius+1) break;
					 board_x--;
					 block_l++;
					 block_r=0;
					 break;
			case 'd':if(board_x==width-radius-2)break;
					 board_x++;
					 block_l=0;
					 block_r++;
					 break;
			default: block_r=0;
					 block_l=0;
					 break;
		}
	}else{block_r=0;
		  block_l=0;		
	}
	}else{
				if(kbhit()){
		key=getch();
		switch(key){
			case 'a':if(board_x==radius+1) break;
					 board_x--;
					 ball_x--;				 
					 break;
			case 'd':if(board_x==width-radius-2)break;
					 board_x++;
					 ball_x++;
					 break;
			case ' ':Alive=1;
					 ball_vy=-1;
			default: break;
		}
	}
	}
	
}

void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}*/

