#include <graphics.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include<stdio.h>
#include<time.h>

#define Width 640
#define High 480
#define pie 3.1415926
#define ratio 4
#define player_num 20

typedef struct Clock
{
	clock_t start_t, end_t;
	int total_t, we_t, en_t;
};

typedef struct Player 
{
	int id;
	double x, y;
	double true_x, true_y;
	double begin_x, begin_y;
	double possiblity = 0.3;
	double distance = 999;
	int haveball = 0;
	int left = 0, right = 0, up = 0, down = 0, lastdirection = -1;
	int stage_idle = 0;
	int state = 0;
	int back = 0;
	double position_x[3];//开场对应state=0
	double position_y[3];//进攻对应state=1
						 //防守对应state=2
};

typedef struct Ball 
{
	double x, y;
	double true_x, true_y;
	double ballrun = 0;
	int left = 0, right = 0, up = 0, down = 0;
	int owner = -1;				//当前持球者
	int min_num = 0;
	int position = 0;        //-1界外，0界内，1进球
	int we_near;
	int score_we = 0, score_en = 0;
};
POINT board[60][4];
POINT Angle[2][3];
int x = -((float)ratio / 2 - 0.5) * Width, y = -((float)ratio / 2 - 0.5) * High;

void startup(Player *player,Ball* ball);
void prepare(Player* player, Ball* ball);
void position_set(Player* player);

void player_magicmove(Player* player, Ball* ball);
void player_AI(Player* player, Ball* ball);
void strugle_player(Ball* ball, Player* player, int i);
void strugle(Ball* ball, Player* player, int i);
void change_position(Player* player, Ball* ball);
void updateWithInput(Player*, Ball*);
void player_atk(Player* player, Ball* ball);
/*********出界********/

void warning_1(Player* player, Ball* ball);
void warning_2(Player* player, Ball* ball);
/**********************/
void show(Player*, Ball*,Clock);
void updateWithoutInput(Player*, Ball*);
void show_player(Player*);
void show_ball(Ball*, Player*);
void show_time(Clock);
void scoretable(Ball*);
void timechecker(Clock*, Ball*);
void Ed(Ball*, Clock*);


void newline(double x1, double y1, double x2, double y2, int x, int y);
void newarc(double x1, double y1, double x2, double y2, double a1, double a2, int x, int y);
void newsolidcircle(double x1, double y1, int radius, int x, int y);
void newbar(double x1, double y1, double x2, double y2, int x, int y);
void drawfield(Ball*);
float field_x(float x1, float y1, int x);
float field_y(float x1, float y1, int y);
void pass(Ball*, Player*);

IMAGE player_sprite[2], player_spritex, ball1, ball1x, ball2, ball2x, ball3, ball3x, ball4, ball4x, ball5, ball5x, ball6, ball6x;

int main() {
	srand((unsigned)time(NULL));
	Ball* ball;
	Player* player;
	ball = (Ball*)malloc(sizeof(Ball));
	player = (Player*)malloc(sizeof(Player) * player_num);
	ball->score_we = 0;
	ball->score_en = 0;

	position_set(player);
	ball->true_x = 48;
	ball->true_y = 34;
	ball->x = field_x(ball->true_x, ball->true_y, x);
	ball->y = field_y(ball->true_x, ball->true_y, y);
	for (int i = 0; i < player_num;i++)
		(player + i)->state = 0;
	
	for (int i = 0; i < player_num;i++)
	{
		(player + i)->true_x = (player + i)->position_x[(player + i)->state];
		(player + i)->true_y = (player + i)->position_y[(player + i)->state];
		(player + i)->id = i;
	}

	for (int i = 0;i < player_num;i++)
	{
		(player + i)->x = field_x((player + i)->true_x, (player + i)->true_y, x);
		(player + i)->y = field_y((player + i)->true_x, (player + i)->true_y, y);

		(player + i)->begin_x = (player + i)->position_x[(player + i)->state];
		(player + i)->begin_y = (player + i)->position_y[(player + i)->state];

		(player + i)->haveball = 0;
	}
	startup(player, ball);

	Clock clock1;
	clock1.total_t = 0;
	clock1.we_t = 0;
	clock1.en_t = 0;
	clock1.total_t = 0;
	clock1.start_t = clock();

	while (true)
	{
		show(player, ball, clock1);
		updateWithInput(player, ball);
		updateWithoutInput(player, ball);
		timechecker(&clock1, ball);
	}
	EndBatchDraw();
	return 0;
}
void startup(Player* player, Ball* ball) 
{
	loadimage(&player_sprite[0], _T("D:\\player1.jpg"), 140, 280);
	loadimage(&player_sprite[1], _T("D:\\player2.jpg"), 140, 280);
	loadimage(&player_spritex, _T("D:\\playerx.jpg"), 140, 280);
	loadimage(&ball1, _T("D:\\ball1.jpg"), 30, 30);
	loadimage(&ball1x, _T("D:\\ball1x.jpg"), 30, 30);
	loadimage(&ball2, _T("D:\\ball2.jpg"), 45, 45);
	loadimage(&ball2x, _T("D:\\ball2x.jpg"), 45, 45);
	loadimage(&ball3, _T("D:\\ball3.jpg"), 45, 45);
	loadimage(&ball3x, _T("D:\\ball3x.jpg"), 45, 45);
	loadimage(&ball4, _T("D:\\ball4.jpg"), 45, 45);
	loadimage(&ball4x, _T("D:\\ball4x.jpg"), 45, 45);
	loadimage(&ball5, _T("D:\\ball5.jpg"), 45, 45);
	loadimage(&ball5x, _T("D:\\ball5x.jpg"), 45, 45);
	loadimage(&ball6, _T("D:\\ball6.jpg"), 45, 45);
	loadimage(&ball6x, _T("D:\\ball6x.jpg"), 45, 45);
	initgraph(Width, High);
	setbkcolor(RGB(102, 205, 0));
	cleardevice();
	BeginBatchDraw();
}

void position_set(Player* player)
{
	(player + 0)->position_x[0] = 8;
	(player + 1)->position_x[0] = 8;
	(player + 2)->position_x[0] = 8;
	(player + 3)->position_x[0] = 30;
	(player + 4)->position_x[0] = 30;
	(player + 5)->position_x[0] = 30;
	(player + 6)->position_x[0] = 48;
	(player + 7)->position_x[0] = 48;
	(player + 8)->position_x[0] = 48;
	(player + 9)->position_x[0] = 44;

	(player + 0)->position_y[0] = 12;
	(player + 1)->position_y[0] = 34;
	(player + 2)->position_y[0] = 56;
	(player + 3)->position_y[0] = 12;
	(player + 4)->position_y[0] = 34;
	(player + 5)->position_y[0] = 56;
	(player + 6)->position_y[0] = 12;
	(player + 7)->position_y[0] = 34;
	(player + 8)->position_y[0] = 56;
	(player + 9)->position_y[0] = 34;

	(player + 10)->position_x[0] = 97;
	(player + 11)->position_x[0] = 97;
	(player + 12)->position_x[0] = 97;
	(player + 13)->position_x[0] = 75;
	(player + 14)->position_x[0] = 75;
	(player + 15)->position_x[0] = 75;
	(player + 16)->position_x[0] = 57;
	(player + 17)->position_x[0] = 57;
	(player + 18)->position_x[0] = 57;
	(player + 19)->position_x[0] = 61;

	(player + 10)->position_y[0] = 12;
	(player + 11)->position_y[0] = 34;
	(player + 12)->position_y[0] = 56;
	(player + 13)->position_y[0] = 12;
	(player + 14)->position_y[0] = 34;
	(player + 15)->position_y[0] = 56;
	(player + 16)->position_y[0] = 12;
	(player + 17)->position_y[0] = 34;
	(player + 18)->position_y[0] = 56;
	(player + 19)->position_y[0] = 34;

	(player + 0)->position_x[1] = 50;
	(player + 1)->position_x[1] = 50;
	(player + 2)->position_x[1] = 50;
	(player + 3)->position_x[1] = 77;
	(player + 4)->position_x[1] = 77;
	(player + 5)->position_x[1] = 77;
	(player + 6)->position_x[1] = 82.5;
	(player + 7)->position_x[1] = 82.5;
	(player + 8)->position_x[1] = 83.5;
	(player + 9)->position_x[1] = 89.5;

	(player + 0)->position_y[1] = 8.5;
	(player + 1)->position_y[1] = 31;
	(player + 2)->position_y[1] = 53.5;
	(player + 3)->position_y[1] = 8.5;
	(player + 4)->position_y[1] = 31;
	(player + 5)->position_y[1] = 53.5;
	(player + 6)->position_y[1] = 8.5;
	(player + 7)->position_y[1] = 31;
	(player + 8)->position_y[1] = 53.5;
	(player + 9)->position_y[1] = 29;

	(player + 10)->position_x[1] = 55;
	(player + 11)->position_x[1] = 55;
	(player + 12)->position_x[1] = 55;
	(player + 13)->position_x[1] = 38;
	(player + 14)->position_x[1] = 38;
	(player + 15)->position_x[1] = 38;
	(player + 16)->position_x[1] = 16.5;
	(player + 17)->position_x[1] = 16.5;
	(player + 18)->position_x[1] = 15.5;
	(player + 19)->position_x[1] = 20.5;

	(player + 10)->position_y[1] = 8.5;
	(player + 11)->position_y[1] = 31;
	(player + 12)->position_y[1] = 53.5;
	(player + 13)->position_y[1] = 8.5;
	(player + 14)->position_y[1] = 31;
	(player + 15)->position_y[1] = 53.5;
	(player + 16)->position_y[1] = 8.5;
	(player + 17)->position_y[1] = 31;
	(player + 18)->position_y[1] = 53.5;
	(player + 19)->position_y[1] = 29;


	(player + 0)->position_x[2] = 12;
	(player + 1)->position_x[2] = 12;
	(player + 2)->position_x[2] = 12;
	(player + 3)->position_x[2] = 34;
	(player + 4)->position_x[2] = 34;
	(player + 5)->position_x[2] = 34;
	(player + 6)->position_x[2] = 50.5;
	(player + 7)->position_x[2] = 54.5;
	(player + 8)->position_x[2] = 50.5;
	(player + 9)->position_x[2] = 45.5;

	(player + 0)->position_y[2] = 8.5;
	(player + 1)->position_y[2] = 31;
	(player + 2)->position_y[2] = 53.5;
	(player + 3)->position_y[2] = 8.5;
	(player + 4)->position_y[2] = 31;
	(player + 5)->position_y[2] = 53.5;
	(player + 6)->position_y[2] = 8.5;
	(player + 7)->position_y[2] = 31;
	(player + 8)->position_y[2] = 53.5;
	(player + 9)->position_y[2] = 31;

	(player + 10)->position_x[2] = 93;
	(player + 11)->position_x[2] = 93;
	(player + 12)->position_x[2] = 93;
	(player + 13)->position_x[2] = 71;
	(player + 14)->position_x[2] = 71;
	(player + 15)->position_x[2] = 71;
	(player + 16)->position_x[2] = 54.5;
	(player + 17)->position_x[2] = 50.5;
	(player + 18)->position_x[2] = 54.5;
	(player + 19)->position_x[2] = 59.5;

	(player + 10)->position_y[2] = 8.5;
	(player + 11)->position_y[2] = 31;
	(player + 12)->position_y[2] = 53.5;
	(player + 13)->position_y[2] = 8.5;
	(player + 14)->position_y[2] = 31;
	(player + 15)->position_y[2] = 53.5;
	(player + 16)->position_y[2] = 8.5;
	(player + 17)->position_y[2] = 31;
	(player + 18)->position_y[2] = 53.5;
	(player + 19)->position_y[2] = 31;
}

void show(Player* player, Ball* ball, Clock clockt) {
	cleardevice();
	drawfield(ball);
	scoretable(ball);
	show_time(clockt);

	int check = 0;
	for (int i = 0;i < player_num;i++)
	{
		if (ball->owner == i)
		{
			show_ball(ball, player);
			show_player(player + i);
			check = 1;
		}
		else
			show_player(player + i);
	}
	if (!check)
	{
		show_ball(ball, player);
	}
	if (ball->position == -1)
	{
		warning_1(player, ball);
		prepare(player, ball);
		x = -((float)ratio / 2 - 0.5) * Width - (ball->x - field_x(52.5, 34, x)) / 1.1;
		y = -((float)ratio / 2 - 0.5) * High;
		
	}
	else if (ball->position == 1)
	{
		if (ball->true_x < -1) {
			ball->score_en++;
		}else ball->score_we++;
		warning_2(player, ball);
		prepare(player, ball);
		x = -((float)ratio / 2 - 0.5) * Width - (ball->x - field_x(52.5, 34, x)) / 1.1;
		y = -((float)ratio / 2 - 0.5) * High;
	}

	FlushBatchDraw();
	Sleep(35);
}

void scoretable(Ball *ball) {
	char we_ge = '0', we_shi = '0', en_ge = '0', en_shi = '0';
	settextstyle(20, 10, _T(""));

	if (ball->score_we <= 99) {
		we_ge += ball->score_we % 10;
		we_shi += ball->score_we / 10;
	}
	else {
		we_ge += 9;
		we_shi += 9;
	}
	
	if (ball->score_en <= 99) {
		en_ge += ball->score_en % 10;
		en_shi += ball->score_en / 10;
	}
	else {
		en_ge += 9;
		en_shi += 9;
	}

	outtextxy(250, 0, _T("SCORE"));
	outtextxy(300, 0, we_shi);
	outtextxy(310, 0, we_ge);
	outtextxy(320, 0, _T(":"));
	outtextxy(330, 0, en_shi);
	outtextxy(340, 0, en_ge);
	
}

void timechecker(Clock *clockt, Ball* ball) {
	static int timebef = 0;

	clockt->end_t = clock();
	clockt->total_t = (int)(clockt->end_t - clockt->start_t) / CLOCKS_PER_SEC;

	if (clockt->total_t > timebef) {
		if (ball->owner > 0 && ball->owner <= 10) clockt->we_t++;
		else if (ball->owner > 10 && ball->owner <= 20) clockt->en_t++;
		timebef = clockt->total_t;
	}

	if (clockt->total_t>500) Ed(ball,clockt);				/*****************此处500为500秒，可更换********************/
}

void show_time(Clock clockt) {
	char t1 = '0', t2 = '0', t3 = '0';
	t1 += (int)clockt.total_t / 60;
	t2 += (int)clockt.total_t % 60 / 10 ;
	t3 += (int)clockt.total_t % 10 ;

	setcolor(WHITE);
	outtextxy(400, 0, _T("TIME"));
	outtextxy(440, 0, t1);
	outtextxy(450, 0, _T(":"));
	outtextxy(460, 0, t2);
	outtextxy(470, 0, t3);
}

void Ed(Ball*ball,Clock*clockt) {//结束界面
	
	settextstyle(20, 10, _T(""));
	char we_ge = '0', we_shi = '0', en_ge = '0', en_shi = '0';

	if (ball->score_we <= 99) {
		we_ge += ball->score_we % 10;
		we_shi += ball->score_we / 10;
	}
	else {
		we_ge += 9;
		we_shi += 9;
	}

	if (ball->score_en <= 99) {
		en_ge += ball->score_en % 10;
		en_shi += ball->score_en / 10;
	}
	else {
		en_ge += 9;
		en_shi += 9;
	}

	char rate1 = '0', rate2 = '0', rate3 = '0';		//敌方控球率
	char Rate1 = '0', Rate2 = '0', Rate3 = '0';		//我方控球率
	double rate, Rate;
	rate = clockt->en_t*1.0 / clockt->total_t;
	Rate = clockt->we_t*1.0 / clockt->total_t;
	rate1 += (int)(rate * 10)%10;
	rate2 += (int)(rate * 100) % 10;
	rate3 += (int)(rate * 1000) % 10;
	Rate1 += (int)(Rate * 10)%10;
	Rate2 += (int)(Rate * 100) % 10;
	Rate3 += (int)(Rate * 1000) % 10;

	cleardevice();
	setcolor(RED);
	outtextxy(150, 100, _T("双方最终比分："));
	outtextxy(300, 100, we_shi);
	outtextxy(310, 100, we_ge);
	outtextxy(320, 100, _T(":"));
	outtextxy(330, 100, en_shi);
	outtextxy(340, 100, en_ge);

	outtextxy(150, 200, _T("全场我方控球率："));
	outtextxy(300, 200, Rate1);
	outtextxy(310, 200, Rate2);
	outtextxy(320, 200, _T("."));
	outtextxy(330, 200, Rate3);
	outtextxy(340, 200, _T("%"));

	outtextxy(150, 220, _T("全场敌方控球率："));
	outtextxy(300, 220, rate1);
	outtextxy(310, 220, rate2);
	outtextxy(320, 220, _T("."));
	outtextxy(330, 220, rate3);
	outtextxy(340, 220, _T("%"));
	FlushBatchDraw();
	while (1) {
		//需要一个退出接口
		
	}		
}

float field_x(float x1, float y1, int x)
{
	float r = (0.12 * y1 + 0.21 * (68 - y1)) / 68;
	return (((1 - 2 * r) * Width * x1 / 105 + r * Width) * ratio + x);
}

float field_y(float x1, float y1, int y)
{
	float r = y1 / 68;
	return ((0.388 + 0.224 * r) * High * ratio + y);
}

void updateWithInput(Player* player, Ball* ball) {
	/****************
	if haveball or neararound
	每轮进行遍历 22次 直到该球员是我们的操作球员
	****************/
	//瓒崇悆浣嶇Щ鎺у埗
	double mindistance = 2000;
	int min_num = 0;
	int we_near = 0;
	for (int i = 0;i < player_num;i++) {
		(player + i)->distance = sqrt(((player + i)->x - ball->x) * ((player + i)->x - ball->x) + ((player + i)->y - ball->y) * ((player + i)->y - ball->y));
		if ((player + i)->distance < mindistance) {
			mindistance = (player + i)->distance;
			min_num = i;
		}
		if ((player + i)->haveball) {
			min_num = i;
			break;
		}
		
	}
	ball->min_num = min_num;

	mindistance = 2000;

	for (int i = 0;i < 10;i++)
	{
		//(player + i)->distance = sqrt(((player + i)->x - ball->x) * ((player + i)->x - ball->x) + ((player + i)->y - ball->y) * ((player + i)->y - ball->y));
		if ((player + i)->distance < mindistance)
		{
			mindistance = (player + i)->distance;
			we_near = i;
		}
		if ((player + i)->haveball)
		{
			we_near = i;
			break;
		}
	}
	ball->we_near = we_near;

	if ((GetAsyncKeyState(0x41) & 0x8000))
	{
		if ((player + we_near)->haveball == 1)
		{
			(player + we_near)->haveball = 0;
			ball->ballrun = 8;
		}
	}
	if ((GetAsyncKeyState(0x44) & 0x8000))
	{
		strugle_player(ball, player, we_near);
	}
	//閻炲啫鎲虫担宥囆╅幒褍鍩?
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000)) {
		(player + we_near)->true_x -= 0.6;
		(player + we_near)->left = 1;
		(player + we_near)->lastdirection = 1;
		x += 9.5;
	}
	else {
		(player + we_near)->left = 0;
	}

	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
		(player + we_near)->true_x += 0.6;
		(player + we_near)->right = 1;
		(player + we_near)->lastdirection = 2;
		x -= 9.5;
	}
	else {
		(player + we_near)->right = 0;
	}

	if ((GetAsyncKeyState(VK_UP) & 0x8000)) {
		if (!((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_RIGHT) & 0x8000))) {
			(player + we_near)->true_y--;
			(player + we_near)->up = 1;
			(player + we_near)->lastdirection = 3;
		}
	}
	else {
		(player + we_near)->up = 0;
	}

	if ((GetAsyncKeyState(VK_DOWN) & 0x8000)) {
		if (!((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_RIGHT) & 0x8000))) {
			(player + we_near)->true_y++;
			(player + we_near)->down = 1;
			(player + we_near)->lastdirection = 4;
		}
	}
	else {
		(player + we_near)->down = 0;
	}

	if (GetAsyncKeyState(32) & 0x8000)
	{
		x = -((float)ratio / 2 - 0.5) * Width - (ball->x - field_x(52.5, 34, x)) / 1.1;
		y = -((float)ratio / 2 - 0.5) * High;
		ball->x = field_x(ball->true_x, ball->true_y, x);
		ball->y = field_y(ball->true_x, ball->true_y, y);
	}
	for (int i = 0;i < player_num;i++)
	{
		(player + i)->x = field_x((player + i)->true_x, (player + i)->true_y, x) ;
		(player + i)->y = field_y((player + i)->true_x, (player + i)->true_y, y) ;
	}
	
}

void updateWithoutInput(Player* player, Ball* ball) {
	/**********
	当遍历球员是npc，进行算法操作
	*************/
	static int shoottime = 0;

	if (ball->ballrun > 0) 
	{
		/*if (ball->ballrun == 30) 
		{
			ball->x += ball->right * 100;
			ball->x -= ball->left * 100;
			ball->y += ball->down * 100;
			ball->y -= ball->up * 100;
		}*/
		//if(player->haveball)
		switch ((player+ball->owner)->lastdirection)
		{
		case 1:ball->left = 1;
			break;
		case 2:ball->right = 1;
			break;
		case 3:ball->up = 1;
			break;
		case 4:ball->down = 1;
			break;
		default:
			break;
		}

		ball->true_x += ball->right * ball->ballrun * 0.8;
		ball->true_x -= ball->left * ball->ballrun * 0.8;
		ball->true_y += ball->down * ball->ballrun;
		ball->true_y -= ball->up * ball->ballrun;
		ball->ballrun -= 2;
		
		(player + ball->owner)->haveball = 0;
		if (!ball->ballrun)
			ball->owner = -1;
		/***********************************************************************
		杩欓噷鐨勬祦绋嬪拰涓婇潰涓€鏍凤紝鍏堟敼ture
		************************************************************************/
		ball->x = field_x(ball->true_x, ball->true_y, x);
		ball->y = field_y(ball->true_x, ball->true_y, y);
	}
	else 
	{
		ball->left = 0;
		ball->right = 0;
		ball->up = 0;
		ball->down = 0;

	}


	if (((player + ball->min_num)->x + 21 - ball->x - 15) * ((player + ball->min_num)->x + 21 - ball->x - 22) + ((player + ball->min_num)->y + 60 - ball->y - 30) * ((player + ball->min_num)->y + 60 - ball->y - 30) < 1400) {
		
		
		ball->owner = ball->min_num;

		ball->ballrun = 0;
		(player + ball->owner)->haveball = 1;
		ball->left = (player + ball->owner)->left;
		ball->right = (player + ball->owner)->right;
		ball->up = (player + ball->owner)->up;
		ball->down = (player + ball->owner)->down;
		ball->true_x = (player + ball->owner)->true_x + 1;
		ball->true_y = (player + ball->owner)->true_y + 5;


		
	}
	else {
		//(player + ball->min_num)->possiblity = 0.5;
		(player + ball->min_num)->haveball = 0;
	}

	if (ball->true_x < -1 || ball->true_x>105.6 || ball->true_y < 0 || ball->true_y>63)
		ball->position = -1;
	if ((ball->true_x < -1 && ball->true_y>26 && ball->true_y < 36) || (ball->true_x > 105.6 && ball->true_y > 26 && ball->true_y < 36))
		ball->position = 1;

		

	player_AI(player, ball);
	//field_xy_ball(ball);
	//field_xy_player(player);
}

void show_ball(Ball* ball, Player* player) 
{
	static int pre_x = x;
	if (ball->owner >= 0)
		if ((player + ball->owner)->haveball == 1)
		{
			ball->x = (player + ball->owner)->x + 6;
			ball->y = (player + ball->owner)->y + 33;
			if ((player + ball->owner)->left == 1) ball->x -= 13;
			if ((player + ball->owner)->right == 1) ball->x += 13;
			if ((player + ball->owner)->up == 1) ball->y -= 13;
			if ((player + ball->owner)->down == 1) ball->y += 13;
			if ((player + ball->owner)->left + (player + ball->owner)->right + (player + ball->owner)->up + (player + ball->owner)->down == 0) {
				if ((player + ball->owner)->lastdirection == 1) ball->x -= 13;
				else if ((player + ball->owner)->lastdirection == 2) ball->x += 13;
				else if ((player + ball->owner)->lastdirection == 3) ball->y -= 13;
				else if ((player + ball->owner)->lastdirection == 4) ball->y += 13;

			}
			pre_x = x;
		}
		//else
		{
			ball->x += (x - pre_x);
			pre_x = x;
		}
		setcolor(WHITE);
	settextstyle(15, 8, _T(""));
	if (ball->we_near >= 0)
		outtextxy((player + ball->we_near)->x, (player + ball->we_near)->y - 15, _T("Player"));

	putimage(ball->x , ball->y, &ball1x, NOTSRCERASE);
	putimage(ball->x , ball->y, &ball1, SRCINVERT);
}

void show_player(Player* player) 
{
	int t;
	static int num = 0;
	static int stage[player_num] = { 0 }, i[player_num] = { 0 };
	//static int i = 0, stage = 0;
	if (i[num] < 12) i[num]++;
	else i[num] = 0;
	if (i[num] < 3) stage[num] = 0;
	else if (i[num] < 6) stage[num] = 1;
	else if (i[num] < 9) stage[num] = 2;
	else if (i[num] < 12) stage[num] = 1;

	if (player->id < 10 && player->id >= 0)
		t = 0;
	else
		t = 1;
	//脠脣脦茂露炉脳梅
	if (player->left == 1) {
		if (player->right == 1) {
			putimage(player->x, player->y, 43, 60, &player_spritex, 46, 140, NOTSRCERASE);
			putimage(player->x, player->y, 43, 60, &player_sprite[t], 46, 140, SRCINVERT);
		}
		else {
			putimage(player->x, player->y, 43, 60, &player_spritex, stage[num] * 46, 205, NOTSRCERASE);
			putimage(player->x, player->y, 43, 60, &player_sprite[t], stage[num] * 46, 205, SRCINVERT);
		}
	}
	else if (player->right == 1) {
		putimage(player->x, player->y, 43, 60, &player_spritex, stage[num] * 51, 77, NOTSRCERASE);
		putimage(player->x, player->y, 43, 60, &player_sprite[t], stage[num] * 51, 77, SRCINVERT);
	}
	else if (player->up == 1) {
		if (player->down == 1) {
			putimage(player->x, player->y, 43, 60, &player_spritex, 55, 140, NOTSRCERASE);
			putimage(player->x, player->y, 43, 60, &player_sprite[t], 55, 140, SRCINVERT);
		}
		else {
			putimage(player->x, player->y, 42, 60, &player_spritex, stage[num] * 50, 10, NOTSRCERASE);
			putimage(player->x, player->y, 42, 60, &player_sprite[t], stage[num] * 50, 10, SRCINVERT);
		}
	}
	else if (player->down == 1) {
		putimage(player->x, player->y, 43, 60, &player_spritex, stage[num] * 50, 140, NOTSRCERASE);
		putimage(player->x, player->y, 43, 60, &player_sprite[t], stage[num] * 50, 140, SRCINVERT);
	}
	else {
		if (player->lastdirection == 1) {
			putimage(player->x, player->y, 43, 60, &player_spritex, 46, 205, NOTSRCERASE);
			putimage(player->x, player->y, 43, 60, &player_sprite[t], 46, 205, SRCINVERT);
		}
		else if (player->lastdirection == 2) {
			putimage(player->x, player->y, 43, 60, &player_spritex, 51, 77, NOTSRCERASE);
			putimage(player->x, player->y, 43, 60, &player_sprite[t], 51, 77, SRCINVERT);
		}
		else if (player->lastdirection == 3) {
			putimage(player->x, player->y, 43, 60, &player_spritex, 50, 10, NOTSRCERASE);
			putimage(player->x, player->y, 43, 60, &player_sprite[t], 50, 10, SRCINVERT);
		}
		else {
			putimage(player->x, player->y, 43, 60, &player_spritex, 50, 140, NOTSRCERASE);
			putimage(player->x, player->y, 43, 60, &player_sprite[t], 50, 140, SRCINVERT);
		}

	}

	

	num++;
	num %= player_num;
}

void player_AI(Player* player, Ball* ball)
{
	static int pre_owner = -1;
	if (pre_owner != ball->owner)
		change_position(player, ball);
	player_magicmove(player, ball);
	if (ball->owner >= 10 && ball->owner <= 19)
		player_atk(player, ball);
	//player_defend();
	pre_owner = ball->owner;

}

void player_magicmove(Player* player, Ball* ball) 
{
	int if_sameteam = 0;
	static long int k = 0;
	k++;
	for (int i = 0;i < player_num;i++) 
	{
		if (i == ball->we_near || i == ball->owner)continue;

		if (sqrt(((player + i)->true_x - (player + i)->begin_x) * ((player + i)->true_x - (player + i)->begin_x) + ((player + i)->true_y - (player + i)->begin_y) * ((player + i)->true_y - (player + i)->begin_y)) >= 14 ) {
			(player + i)->back = 1;
		}

		if ((player + i)->back == 1) {
			if ((player + i)->true_x > (player + i)->begin_x)
			{
				(player + i)->right = 0;
				(player + i)->left = 1;
				(player + i)->true_x -= 0.3;
			}
			else
			{
				(player + i)->left = 0;
			}
			if ((player + i)->true_x < (player + i)->begin_x)
			{
				(player + i)->left = 0;
				(player + i)->right = 1;
				(player + i)->true_x += 0.3;
			}
			else
			{
				(player + i)->right = 0;
			}
			if ((player + i)->true_y > (player + i)->begin_y)
			{
				(player + i)->down = 0;
				(player + i)->up = 1;
				(player + i)->true_y -= 0.5;
			}
			else
			{
				(player + i)->up = 0;
			}
			if ((player + i)->true_y < (player + i)->begin_y)
			{
				(player + i)->up = 0;
				(player + i)->down = 1;
				(player + i)->true_y += 0.5;
			}
			else
			{
				(player + i)->down = 0;
			}

			if (sqrt(((player + i)->true_x - (player + i)->begin_x) * ((player + i)->true_x - (player + i)->begin_x) + ((player + i)->true_y - (player + i)->begin_y) * ((player + i)->true_y - (player + i)->begin_y)) <= 0.5) (player + i)->back = 0;
			continue;
		}

		if (ball->owner >= 0 && ball->owner < 10)
		{
			if (i >= 0 && i < 10)
				if_sameteam = 1;
		}
		else if(ball->owner >= 10 && ball->owner < 20)
		{
			if (i >= 10 && i < 20)
				if_sameteam = 1;
		}
			
		if (((player + i)->x + 21 - ball->x - 15) * ((player + i)->x + 21 - ball->x - 22) + ((player + i)->y + 60 - ball->y - 30) * ((player + i)->y + 60 - ball->y - 30) < 40000 && if_sameteam == 0&& sqrt(((player + i)->true_x - (player + i)->begin_x) * ((player + i)->true_x - (player + i)->begin_x) + ((player + i)->true_y - (player + i)->begin_y) * ((player + i)->true_y - (player + i)->begin_y)) < 14)
		{
			if (((player + i)->x + 21 - ball->x - 15) * ((player + i)->x + 21 - ball->x - 22) + ((player + i)->y + 60 - ball->y - 30) * ((player + i)->y + 60 - ball->y - 30) > 1000)
			{
				if ((player + i)->x > ball->x + 20)
				{

					(player + i)->left = 1;
					(player + i)->true_x -= 0.5;
				}
				else
				{
					(player + i)->left = 0;
				}
				if ((player + i)->x < ball->x - 20)
				{
					(player + i)->right = 1;
					(player + i)->true_x += 0.5;
				}
				else
				{
					(player + i)->right = 0;
				}
				if ((player + i)->y > ball->y + 24)
				{
					(player + i)->up = 1;
					(player + i)->true_y -= 1;
				}
				else
				{
					(player + i)->up = 0;
				}
				if ((player + i)->y < ball->y - 24)
				{
					(player + i)->down = 1;
					(player + i)->true_y += 1;
				}
				else
				{
					(player + i)->down = 0;
				}
			}
		}
		else 
		{
			if ((player + i)->true_x > (player + i)->begin_x)
			{
				(player + i)->left = 1;
				(player + i)->true_x -= 0.3;
			}
			else
			{
				(player + i)->left = 0;
			}
			if ((player + i)->true_x < (player + i)->begin_x)
			{
				(player + i)->right = 1;
				(player + i)->true_x += 0.3;
			}
			else 
			{
				(player + i)->right = 0;
			}
			if ((player + i)->true_y > (player + i)->begin_y) 
			{
				(player + i)->up = 1;
				(player + i)->true_y -= 0.5;
			}
			else 
			{
				(player + i)->up = 0;
			}
			if ((player + i)->true_y < (player + i)->begin_y) 
			{
				(player + i)->down = 1;
				(player + i)->true_y += 0.5;
			}
			else 
			{
				(player + i)->down = 0;
			}
			if (sqrt(((player + i)->true_x - (player + i)->begin_x) * ((player + i)->true_x - (player + i)->begin_x) + ((player + i)->true_y - (player + i)->begin_y) * ((player + i)->true_y - (player + i)->begin_y)) < 4)
			{
				if ((player + i)->begin_x != (player + i)->position_x[(player + i)->state] || (player + i)->begin_y != (player + i)->position_y[(player + i)->state])
				{
					(player + i)->begin_x = (player + i)->position_x[(player + i)->state];
					(player + i)->begin_y = (player + i)->position_y[(player + i)->state];
				}
				else
				{
					(player + i)->begin_x += rand() % 15 - 7;
					(player + i)->begin_y -= rand() % 11 - 5;
				}
			}
		}

			/*static clock_t start, again;
			static double time_1, time_2;
			double timing;
			static int m = 1;
			if (m)
			{
				start = clock();
				time_1 = (double)start / CLK_TCK;
				m--;
			}
			again = clock();
			time_2 = (double)again / CLK_TCK;
			timing = time_1 - time_2;
			if (timing >= 0.5)
			{*/
			if (if_sameteam == 0)
				strugle(ball, player, i);
		

		if_sameteam = 0;
	}
}
void player_atk(Player* player, Ball* ball)
{


	(player + ball->owner)->up = 0;
	(player + ball->owner)->down = 0;
	(player + ball->owner)->left = 0;
	(player + ball->owner)->right = 0;


	if ((player + ball->owner)->true_x > 30)
	{
		(player + ball->owner)->left = 1;
		(player + ball->owner)->true_x -= 0.5;
	}
	else
	{
		//向前加速
		if ((player + ball->owner)->true_x > -2)
		{
			(player + ball->owner)->left = 1;
			(player + ball->owner)->true_x -= 0.5;
		}
		else
		{
			(player + ball->owner)->left = 0;
		}
		if ((player + ball->owner)->true_x < 15)
		{
			(player + ball->owner)->left = 1;
			(player + ball->owner)->true_x -= 0.2;
		}
		
		//向球门靠拢		
		if ((player + ball->owner)->true_y < 22)
		{
			(player + ball->owner)->down = 1;
			(player + ball->owner)->true_y += 1;
		}
		else
		{
			(player + ball->owner)->down = 0;
		}
		if ((player + ball->owner)->true_y > 28)
		{
			(player + ball->owner)->up = 1;
			//(player + ball->owner)->down = 0;
			(player + ball->owner)->true_y -= 1;
		}
		else
		{
			(player + ball->owner)->up = 0;
		}
		/*if (((player + ball->owner)->true_y > 22) && ((player + ball->owner)->true_y < 28))
		{
			(player + ball->owner)->up = 0;
			(player + ball->owner)->down = 0;
		}*/

	}
}


void warning_1(Player* player, Ball* ball)
{
	MOUSEMSG m1;
	setbkmode(OPAQUE);
	while (1)
	{
		while (MouseHit())
		{
			m1 = GetMouseMsg();
			if (m1.uMsg == WM_LBUTTONDOWN || m1.uMsg == WM_RBUTTONDOWN)
			{
				ball->position = 0;
				return;
			}
		}
		setcolor(BLACK);
		setfillcolor(WHITE);
		rectangle(220, 190, 420, 290);

		settextstyle(30, 0, 0);
		outtextxy(275, 225, _T("球出界"));

		settextstyle(10, 0, 0);
		outtextxy(260, 270, _T("鼠标点击继续比赛"));
		FlushBatchDraw();
	}
}
void warning_2(Player* player, Ball* ball)
{
	MOUSEMSG m1;
	while (1)
	{
		while (MouseHit())
		{
			m1 = GetMouseMsg();
			if (m1.uMsg == WM_LBUTTONDOWN || m1.uMsg == WM_RBUTTONDOWN)
			{
				ball->position = 0;
				return;
			}
		}
		setcolor(BLACK);
		setfillcolor(WHITE);
		rectangle(220, 190, 420, 290);

		settextstyle(30, 0, 0);
		outtextxy(275, 225, _T("球进了"));

		settextstyle(10, 0, 0);
		outtextxy(260, 270, _T("鼠标点击继续比赛"));
		FlushBatchDraw();
	}
}
void prepare(Player* player, Ball* ball)
{

	ball->true_x = 48;
	ball->true_y = 34;
	ball->x = field_x(ball->true_x, ball->true_y, x);
	ball->y = field_y(ball->true_x, ball->true_y, y);

	for (int i = 0; i < player_num;i++)
	{
		(player + i)->true_x = (player + i)->position_x[(player + i)->state];
		(player + i)->true_y = (player + i)->position_y[(player + i)->state];
	}
	for (int i = 0;i < player_num;i++)
	{
		(player + i)->x = field_x((player + i)->true_x, (player + i)->true_y, x);
		(player + i)->y = field_y((player + i)->true_x, (player + i)->true_y, y);
		(player + i)->haveball = 0;
	}
}

void change_position(Player* player, Ball* ball)
{
	int i;
	if (ball->owner >= 0 && ball->owner < 10)
	{
		for (i = 0;i < 10;i++)
			(player + i)->state = 1;
		for (i = 10;i < 20;i++)
			(player + i)->state = 2;
	}
	else if (ball->owner >= 10 && ball->owner < 20)
	{
		for (i = 0;i < 10;i++)
			(player + i)->state = 2;
		for (i = 10;i < 20;i++)
			(player + i)->state = 1;
	}
	else if (ball->position == 1)
	{
		for (i = 0;i < 20;i++)
			(player + i)->state = 0;
	}
	else
	{
		for (i = 0;i < 20;i++)
			(player + i)->state = 0;
	}
	for (i = 0;i < 20;i++)
	{
		(player + i)->begin_x = (player + i)->position_x[(player + i)->state];
		(player + i)->begin_y = (player + i)->position_y[(player + i)->state];
	}
}
void strugle(Ball* ball, Player* player, int i) 
{
	static int times[20] = { 0 };
	times[i]++;
	if (times[i] != 5)
		return;
	else
		times[i] = 0;
	if (((player + i)->x + 21 - ball->x - 15) * ((player + i)->x + 21 - ball->x - 22) + ((player + i)->y + 60 - ball->y - 30) * ((player + i)->y + 60 - ball->y - 30) < 1600 && (player + i)->haveball == 0 && (player + i)->possiblity != 0) 
	{
		if (rand() % 10 > player->possiblity * 10) 
		{
			(player + ball->owner)->haveball = 0;
			(player + i)->haveball = 1;
			ball->min_num = i;
		}
	}
}

void strugle_player(Ball* ball, Player* player, int i)
{
	static int times[20] = { 0 };
	times[i]++;
	if (times[i] != 4)
		return;
	else
		times[i] = 0;
	if (((player + i)->x + 21 - ball->x - 15) * ((player + i)->x + 21 - ball->x - 22) + ((player + i)->y + 60 - ball->y - 30) * ((player + i)->y + 60 - ball->y - 30) < 1600 && (player + i)->haveball == 0 && (player + i)->possiblity != 0)
	{
		if (rand() % 10 > 2) //概率%70
		{
			(player + ball->owner)->haveball = 0;
			(player + i)->haveball = 1;
			ball->min_num = i;
		}
	}
	
}

void drawfield(Ball* ball)
{

	int i;
	cleardevice();
	setcolor(RGB(45, 95, 10));
	setfillcolor(RGB(75, 125, 40));
	newbar(0.1 * Width, 0.36 * High, 0.9 * Width, 0.63 * High, x, y);

	setcolor(RGB(144, 28, 28));//跑道左
	setfillcolor(RGB(144, 28, 28));
	Angle[0][0].x = 0 + x;
	Angle[0][0].y = 0 + y;
	Angle[0][1].x = Width * 0.176 * ratio + x;
	Angle[0][1].y = High * 0.338 * ratio + y;
	Angle[0][2].x = Width * 0.06 * ratio + x;
	Angle[0][2].y = High * 0.562 * ratio + y;
	solidpolygon(Angle[0], 3);
	setcolor(WHITE);
	setfillcolor(WHITE);
	newline(Width * 0.176, High * 0.338, Width * 0.06, High * 0.562, x, y);
	newline(Width * 0.161, High * 0.338, Width * 0.04, High * 0.562, x, y);
	newline(Width * 0.148, High * 0.338, Width * 0.02, High * 0.562, x, y);
	newline(Width * 0.136, High * 0.338, Width * 0.00, High * 0.562, x, y);
	newline(Width * 0.125, High * 0.338, -Width * 0.02, High * 0.562, x, y);

	setcolor(RGB(144, 28, 28));//跑道右
	setfillcolor(RGB(144, 28, 28));
	Angle[1][0].x = Width * ratio + x;
	Angle[1][0].y = 0 + y;
	Angle[1][1].x = Width * 0.824 * ratio + x;
	Angle[1][1].y = High * 0.338 * ratio + y;
	Angle[1][2].x = Width * 0.94 * ratio + x;
	Angle[1][2].y = High * 0.562 * ratio + y;
	solidpolygon(Angle[1], 3);
	setcolor(WHITE);
	setfillcolor(WHITE);
	newline(Width * 0.824, High * 0.338, Width * 0.94, High * 0.562, x, y);
	newline(Width * 0.839, High * 0.338, Width * 0.96, High * 0.562, x, y);
	newline(Width * 0.852, High * 0.338, Width * 0.98, High * 0.562, x, y);
	newline(Width * 0.864, High * 0.338, Width * 1, High * 0.562, x, y);
	newline(Width * 0.875, High * 0.338, Width * 1.02, High * 0.562, x, y);

	for (i = 0;i < 60;i++)
	{
		board[i][0].x = Width * (0.21 + i * 0.0097) * ratio + x;
		board[i][0].y = High * 0.388 * ratio + y;
		board[i][1].x = Width * (0.21 + (i + 1) * 0.0097) * ratio + x;
		board[i][1].y = High * 0.388 * ratio + y;
		board[i][2].x = Width * (0.12 + (i + 1) * 0.0127) * ratio + x;
		board[i][2].y = High * 0.612 * ratio + y;
		board[i][3].x = Width * (0.12 + i * 0.0127) * ratio + x;
		board[i][3].y = High * 0.612 * ratio + y;
		setfillcolor(RGB(45 + i % 2 * 30, 95 + i % 2 * 30, 10 + i % 2 * 30));
		solidpolygon(board[i], 4);
	}


	setcolor(WHITE);
	setfillcolor(WHITE);
	newline(Width * 0.21, High * 0.388, Width * 0.79, High * 0.388, x, y);//球场框
	newline(Width * 0.21, High * 0.388, Width * 0.12, High * 0.612, x, y);
	newline(Width * 0.12, High * 0.612, Width * 0.88, High * 0.612, x, y);
	newline(Width * 0.88, High * 0.612, Width * 0.79, High * 0.388, x, y);

	newline(Width * 0.5, High * 0.39, Width * 0.5, High * 0.61, x, y);//中线

	newarc(Width * 0.44, High * 0.47, Width * 0.56, High * 0.53, 0, 0, x, y);//中圆
	newsolidcircle(Width * 0.5, High * 0.5, 1, x, y);//中点

	newline(Width * 0.1771, High * 0.470, Width * 0.210, High * 0.470, x, y);//球门线左
	newline(Width * 0.1528, High * 0.530, Width * 0.189, High * 0.530, x, y);
	newline(Width * 0.210, High * 0.470, Width * 0.189, High * 0.530, x, y);

	newsolidcircle(Width * 0.235, High * 0.5, 1, x, y);//左点
	newarc(Width * 0.165, High * 0.530, Width * 0.301, High * 0.470, -0.37 * pie, 0.29 * pie, x, y);//左半圆

	newline(Width * 0.8229, High * 0.470, Width * 0.790, High * 0.470, x, y);//球门线右
	newline(Width * 0.8472, High * 0.530, Width * 0.811, High * 0.530, x, y);
	newline(Width * 0.790, High * 0.470, Width * 0.811, High * 0.530, x, y);
	newsolidcircle(Width * 0.765, High * 0.5, 1, x, y);//右点
	newarc(Width * 0.835, High * 0.530, Width * 0.699, High * 0.470, 0.71 * pie, -0.63 * pie, x, y);//右半圆

	newline(Width * 0.1916, High * 0.434, Width * 0.286, High * 0.434, x, y);//禁区线左
	newline(Width * 0.1384, High * 0.566, Width * 0.249, High * 0.566, x, y);
	newline(Width * 0.286, High * 0.434, Width * 0.249, High * 0.566, x, y);

	newline(Width * 0.8084, High * 0.434, Width * 0.714, High * 0.434, x, y);//禁区线右
	newline(Width * 0.8616, High * 0.566, Width * 0.751, High * 0.566, x, y);
	newline(Width * 0.714, High * 0.434, Width * 0.751, High * 0.566, x, y);

	newline(Width * 0.160, High * 0.512, Width * 0.134, High * 0.512, x, y);//左球门底线
	newline(Width * 0.170, High * 0.488, Width * 0.145, High * 0.488, x, y);
	newline(Width * 0.134, High * 0.512, Width * 0.145, High * 0.488, x, y);

	newline(Width * 0.160, High * 0.512, Width * 0.160, High * 0.476, x, y);//门柱
	newline(Width * 0.134, High * 0.512, Width * 0.134, High * 0.476, x, y);
	newline(Width * 0.170, High * 0.488, Width * 0.170, High * 0.456, x, y);
	newline(Width * 0.145, High * 0.488, Width * 0.145, High * 0.456, x, y);

	newline(Width * 0.160, High * 0.476, Width * 0.134, High * 0.476, x, y);//上门线
	newline(Width * 0.160, High * 0.476, Width * 0.170, High * 0.456, x, y);
	newline(Width * 0.145, High * 0.456, Width * 0.170, High * 0.456, x, y);
	newline(Width * 0.145, High * 0.456, Width * 0.134, High * 0.476, x, y);

	for (i = 1;i <= 25;i++)
		newline(Width * (0.160 - i * 0.001), High * 0.512, Width * (0.160 - i * 0.001), High * 0.476, x, y);//网
	for (i = 1;i <= 12;i++)
		newline(Width * 0.160, High * (0.512 - i * 0.003), Width * 0.134, High * (0.512 - i * 0.003), x, y);
	for (i = 1;i <= 25;i++)
		newline(Width * (0.145 + i * 0.001), High * 0.456, Width * (0.134 + i * 0.001), High * 0.476, x, y);
	for (i = 1;i <= 25;i++)
		newline(Width * (0.170 - i * 0.001), High * 0.488, Width * (0.170 - i * 0.001), High * 0.456, x, y);
	for (i = 1;i <= 10;i++)
		newline(Width * 0.170, High * (0.488 - i * 0.003), Width * 0.145, High * (0.488 - i * 0.003), x, y);
	for (i = 1;i <= 10;i++)
		newline(Width * (0.134 + 0.001 * i), High * 0.476, Width * (0.134 + 0.001 * i), High * (0.476 - 0.002 * i), x, y);

	newline(Width * 0.8084, High * 0.434, Width * 0.714, High * 0.434, x, y);//禁区线右
	newline(Width * 0.8616, High * 0.566, Width * 0.751, High * 0.566, x, y);
	newline(Width * 0.714, High * 0.434, Width * 0.751, High * 0.566, x, y);

	newline(Width * 0.160, High * 0.512, Width * 0.134, High * 0.512, x, y);//左球门底线
	newline(Width * 0.170, High * 0.488, Width * 0.145, High * 0.488, x, y);
	newline(Width * 0.134, High * 0.512, Width * 0.145, High * 0.488, x, y);

	newline(Width * 0.160, High * 0.512, Width * 0.160, High * 0.476, x, y);//门柱
	newline(Width * 0.134, High * 0.512, Width * 0.134, High * 0.476, x, y);
	newline(Width * 0.170, High * 0.488, Width * 0.170, High * 0.456, x, y);
	newline(Width * 0.145, High * 0.488, Width * 0.145, High * 0.456, x, y);

	newline(Width * 0.160, High * 0.476, Width * 0.134, High * 0.476, x, y);//上门线
	newline(Width * 0.160, High * 0.476, Width * 0.170, High * 0.456, x, y);
	newline(Width * 0.145, High * 0.456, Width * 0.170, High * 0.456, x, y);
	newline(Width * 0.145, High * 0.456, Width * 0.134, High * 0.476, x, y);

	for (i = 1;i <= 25;i++)
		newline(Width * (0.160 - i * 0.001), High * 0.512, Width * (0.160 - i * 0.001), High * 0.476, x, y);//网
	for (i = 1;i <= 12;i++)
		newline(Width * 0.160, High * (0.512 - i * 0.003), Width * 0.134, High * (0.512 - i * 0.003), x, y);
	for (i = 1;i <= 25;i++)
		newline(Width * (0.145 + i * 0.001), High * 0.456, Width * (0.134 + i * 0.001), High * 0.476, x, y);
	for (i = 1;i <= 25;i++)
		newline(Width * (0.170 - i * 0.001), High * 0.488, Width * (0.170 - i * 0.001), High * 0.456, x, y);
	for (i = 1;i <= 10;i++)
		newline(Width * 0.170, High * (0.488 - i * 0.003), Width * 0.145, High * (0.488 - i * 0.003), x, y);
	for (i = 1;i <= 10;i++)
		newline(Width * (0.134 + 0.001 * i), High * 0.476, Width * (0.134 + 0.001 * i), High * (0.476 - 0.002 * i), x, y);

	newline(Width * 0.840, High * 0.512, Width * 0.866, High * 0.512, x, y);//右球门底线
	newline(Width * 0.830, High * 0.488, Width * 0.855, High * 0.488, x, y);
	newline(Width * 0.866, High * 0.512, Width * 0.855, High * 0.488, x, y);

	newline(Width * 0.840, High * 0.512, Width * 0.840, High * 0.476, x, y);//门柱
	newline(Width * 0.866, High * 0.512, Width * 0.866, High * 0.476, x, y);
	newline(Width * 0.830, High * 0.488, Width * 0.830, High * 0.456, x, y);
	newline(Width * 0.855, High * 0.488, Width * 0.855, High * 0.456, x, y);

	newline(Width * 0.840, High * 0.476, Width * 0.866, High * 0.476, x, y);//上门线
	newline(Width * 0.840, High * 0.476, Width * 0.830, High * 0.456, x, y);
	newline(Width * 0.855, High * 0.456, Width * 0.830, High * 0.456, x, y);
	newline(Width * 0.855, High * 0.456, Width * 0.866, High * 0.476, x, y);

	for (i = 1;i <= 25;i++)
		newline(Width * (0.840 + i * 0.001), High * 0.512, Width * (0.840 + i * 0.001), High * 0.476, x, y);//网
	for (i = 1;i <= 12;i++)
		newline(Width * 0.840, High * (0.512 - i * 0.003), Width * 0.866, High * (0.512 - i * 0.003), x, y);
	for (i = 1;i <= 25;i++)
		newline(Width * (0.855 - i * 0.001), High * 0.456, Width * (0.866 - i * 0.001), High * 0.476, x, y);
	for (i = 1;i <= 25;i++)
		newline(Width * (0.830 + i * 0.001), High * 0.488, Width * (0.830 + i * 0.001), High * 0.456, x, y);
	for (i = 1;i <= 10;i++)
		newline(Width * 0.830, High * (0.488 - i * 0.003), Width * 0.855, High * (0.488 - i * 0.003), x, y);
	for (i = 1;i <= 10;i++)
		newline(Width * (0.866 - 0.001 * i), High * 0.476, Width * (0.866 - 0.001 * i), High * (0.476 - 0.002 * i), x, y);

	newarc(Width * 0.216, High * 0.391, Width * 0.194, High * 0.384, -0.40 * pie, -0.1 * pie, x, y);
	newarc(Width * 0.784, High * 0.391, Width * 0.806, High * 0.384, -0.9 * pie, -0.60 * pie, x, y);
	newarc(Width * 0.127, High * 0.608, Width * 0.113, High * 0.616, 0, 0.435 * pie, x, y);
	newarc(Width * 0.873, High * 0.608, Width * 0.887, High * 0.616, 0.565 * pie, pie, x, y);

	//if (GetAsyncKeyState(0x41) & 0x8000)
	//if (ball->left == 1)
		//x += 9.5;
	//if (GetAsyncKeyState(0x44) & 0x8000)
	//if (ball->right == 1)
		//x -= 9.5;
	if (x >= -0.4 * Width)
		x = -0.4 * Width;
	if (x <= -2.6 * Width)
		x = -2.6 * Width;

}

void newline(double x1, double y1, double x2, double y2, int x, int y)
{
	line(x1 * ratio + x, y1 * ratio + y, x2 * ratio + x, y2 * ratio + y);
}
void newarc(double x1, double y1, double x2, double y2, double a1, double a2, int x, int y)
{
	arc(x1 * ratio + x, y1 * ratio + y, x2 * ratio + x, y2 * ratio + y, a1, a2);
}
void newsolidcircle(double x1, double y1, int radius, int x, int y)
{
	solidcircle(x1 * ratio + x, y1 * ratio + y, radius * ratio);
}
void newbar(double x1, double y1, double x2, double y2, int x, int y)
{
	bar(x1 * ratio + x, y1 * ratio + y, x2 * ratio + x, y2 * ratio + y);
}



/**************************
1.把11个人弄出来.
2.int if_atk = 0, 1;
int position = 1 - 12 void change_position(Player player, Ball ball);
3.void player_AI(***)->1.player_atk()2.player_defend();
1-10  334 3个后卫3个中锋4个前锋
后卫

操控球员头上有个箭头
*************************/
						