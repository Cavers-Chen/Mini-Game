#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

const int High=20,Width=20;
int screen[High][Width];                //0�հ� 1�߽� 2�ɻ� 3�ӵ� 4-15�л� 
int score,life,highscore,pause,level;
int fl_x,fl_y;
int enemy_x;

void Firstlook(){
	printf("�ɻ���ս\n ���򣺻��ел�����+1����ײ�л�����-1���л����±߽�����-1�����⣬ÿ��10�֣�����ֵ+1\n��a��w��d��sΪ�������ң���j�������ӵ�����p��Ϊ��ͣ\nף����ˣ�\n");
	printf("____���س���ʼ��Ϸ____\n");
	char ch;
	ch=getchar();
	system("cls");
}

void startup(){
	fl_x=Width/2;
	fl_y=High*2/3;
	score=0,highscore=0;
	life=3;
	enemy_x=rand()%(Width-2)+1;
	pause=0,level=1;
	
	for(int i=0;i<High;i++){
		for(int j=0;j<Width;j++){
			if(j==0||j==Width-1) screen[i][j]=1;
			else if(i==fl_y&&j==fl_x) screen[i][j]=2;
			else if(i==0&&j==enemy_x) screen[i][j]=4;
			else screen[i][j]=0;
		}
	}
}

void death(){
	Sleep(3000);
	system("cls");
	puts("Game Over");
	Sleep(3000);
	system("cls");
	printf("Your Score:%d\n",score);
	exit(1);
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
			if(screen[i][j]==0) putchar(' ');
			else if(screen[i][j]==1) putchar('|');
			else if(screen[i][j]==2) putchar('*');
			else if(screen[i][j]==3) putchar('^');
			else if(screen[i][j]>=4) putchar('+');
		}
		putchar('\n');
	} 
	printf("Score:%d\nLife:%d\nLevel:%d",score,life,level);
	Sleep(30);
}

void updateWithInput(){
	char key;
	if(kbhit()){
		key=getch();
		switch(key){
			case 'a':if(fl_x==1) break;
					 else fl_x--;
					 break;
			case 'd':if(fl_x==Width-2) break;
				  	 else fl_x++;
				  	 break;
			case 'w':if(fl_y==High/4) break;
					 else fl_y--;
					 break;
			case 's':if(fl_y==High-2) break;
					 else fl_y++;
					 break;
			case 'j':screen[fl_y-1][fl_x]=3;
					 break;
			case 'p':if(pause==0) pause=1;
					 else pause=0;
					 break;
			default: break; 
		}
	}
}

void updateWithoutInput(){
	static int time_en=0,flag=0;
	
	if(!pause){
		if(time_en<20){
			time_en++;
		}else{
			time_en=0;
			flag=1;
			enemy_x=rand()%(Width-2)+1;
		}
		
		for(int i=0;i<High;i++){
			for(int j=0;j<Width;j++){
				if(j==0||j==Width-1){									//���ɱ߽� 
					 screen[i][j]=1;
				}else if(screen[i][j]>=4&&fl_x==j&&fl_y==i){			//������ 
					screen[i][j]=2;
					if(!--life){
						death();
					}
				}else if(screen[i][j]>=4&&i==High-1){					//�л�����ͷ� 
					screen[i][j]=0;
					if(!--life) death();
				}else if(i>0&&screen[i][j]==3&&screen[i-1][j]>=4){ 		//���ел� 
					screen[i][j]=0;
					screen[i-1][j]=0;
					score++;
					if(level<5&&score/10>highscore/10){					//�Ѷȵ��� 
						level++;
					}
					life+=score/10-highscore/10;
					highscore=score;
				}else if(i==fl_y&&j==fl_x){								//���ɻ��� 
					screen[i][j]=2;
				}else if(flag==1&&i==0&&j==enemy_x){					//���ɵл� 
					screen[i][j]=4;
				}else if(screen[i][j]==3&&i>0){          				//�ӵ��ƶ� 				ע��������һ��д�����ˣ���Ϸ����Ե�޹����� 
					screen[i-1][j]=3;
					screen[i][j]=0;
				}else if(screen[i][j]>=4){								//�л��ƶ� 
					if(screen[i][j]>=30-(level-1)*5){
						screen[i+1][j]=4;
						screen[i][j]=0;
					}else screen[i][j]++;
				}else screen[i][j]=0;
			}
		}
	}	
}

void HideCursor(){
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int main(int argc,char*argv[]){
	Firstlook();
	startup();
	while(1){
		HideCursor();
		show();
		updateWithInput();
		updateWithoutInput();
	}
	return 0;
} 
