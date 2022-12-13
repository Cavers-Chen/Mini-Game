#include<graphics.h>
#include<conio.h>
#include<math.h>
#include <tchar.h>
#include <stdio.h>

#define Width 640
#define High 480

typedef struct					//球员数据的结构体
{
	int player_num;				//球员编号十位代表国家1*中国2*Russia3*Korea4*German5*UK，1.2.3.4.5为每个国家的自创球员，60-64为公共替补
	TCHAR name[20];				//TCHAR和char的差别是在于ANSI和Unicode的变更，这方面你看到得问我，函数用法上有些许差别
	float speed_ac;				//之后得对应为球员能力在word里面有
	float speed_max;
	float control_radius;
	float control_skill;
	float shoot_1;
	float shoot_2;
	float shoot_3;
	float enduce;
	float pass_short;
	float pass_long;
	float defend;
	float power;
	float if_door;
}Player;

typedef struct
{
	TCHAR account[20];
	TCHAR code[20];
}Users;

int table = 0;
//用于在多个界面之间转化的变量
//0为登录界面signin()，1为注册界面register(),2选择游戏界面一次游戏界面:单人游戏，双人游戏，帮助，关于 choose_main()
//21单人游戏选择界面choose_main_solo()，22双人游戏球队选择choose_main_double，23帮助
//choose_main_help()，24关于 choose_main_relation()
//211快速界面choose_main_solo_quickrun()，212球员创立choose_main_solo_playerstdio()
//221游戏里面game2()
//2111游戏game1()
int mouse_x = 0, mouse_y = 0;
IMAGE bk0_img, bk10_img, bk1h0_img, fifa0_img; //0   1
IMAGE img_backgound2, img_background21;        //2   21
IMAGE russia, china, german, korea, britain, choosebk, formation433, formation343, formation334;    //国家和阵型图片
IMAGE bk_img, c_img[5];		//背景图这个可以放在startup里面，不过这样的话putimage也要放在里面

TCHAR put_1[20], put_2[20];			//输入的账号密码显示，不过现在是空文，因为没有操作文件
int put_position1 = 0, put_position2 = 0;//纪录上面位置
int name_position = 0;		//同上纪录name的最终输入位置

TCHAR name[20];				//这个是在名字操作函数name_put里，可以传参操作
TCHAR account[20];
TCHAR code[20];

int record = -1;																			//记录队员列表中上一次点击的队员的编号
int list_block[4][21] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, { 0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0 }, { 0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0 }, { 0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0} };//记录LIST表格中头名状况，方便跳过该单位格的输出
TCHAR player_code[65][20] = { _T("大傻"),_T("二愣12子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大62傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九") };				//测试的取名***这部分应该在initplayerlist函数部分被初始化***
int player_num[66] = { 0 };																	//0-65代表队员编号把每个数字当作两位数，十位当作国家名，各位是球员名，十位为6的是替补			

void signin();
int show_input_0();
void name_put1(int isput);//账号输入函数
void name_put2(int isput);//密码输入函数    值得注意的是我这里注册和登录用的同一个

void registerup();
int show_input_1();
void file_do(int isput);

void choose_main();
void show_2();
int updateWithInput_2();

void choose_main_solo();
void show_21();
int updateWithInput_21();

void choose_main_double();
void show_22();
int updateWithinput_22();
void newdraw_double(int flagnum1, int formnum1, int flagnum2, int formnum2);

void choose_main_solo_quickrun();
void newdraw_playerlist(int flagnum, int formnum);											//两个参数，分别更改队形和人员数据
void initplayerlist();																		//读文件（这部分函数内容我先注释掉了）****这里vs写法下和c写法不是很兼容，所以我写出来要么是编译过不了，要么是运行时崩溃，老哥可以稍微研究一下，解决不了问题应该也不是很大\（0.0）/****
void remakeplayerlist(int flagnum, int formnum, int num);									//更改队员编号顺序和姓名
void firststep();																			//初始单人界面
void drawonetab(int flagnum, int formnum);													//绘制国家队员名字
void drawtwotab();																			//绘制公用替补队员名字
void drawthrtab(int flagnum);																//绘制自创国家替补队员名字
void newdraw_solo(int flagnum, int formnum);												//单人界面刷新绘制
void show_211();
int updateWithInput_211();

void choose_main_solo_playerstdio();
void ability_1(int able_x, char able, int x1, int y1, int x2, int y2);//左边三个数依据位置显示数字字符
void ability_2(int able_x, char able, int x1, int y1, int x2, int y2);//右边三个
void name_put(int isput);//名字输入纪录函数
float ability_num(int able_x, int x1);//计算speed_ac之类的计算函数，具体怎么计算还会改下，现在粗暴直接按照输入值
int tips_ifreplace();//判断是否txt文件中以前已经创立了这个国家的球员

void startup_all();
int main()
{
	startup_all();
	while (1)
	{
		switch (table)
		{
		case 0:signin();
			break;
		case 1:registerup();
			break;
		case 2:choose_main();
			break;
		case 21:choose_main_solo();
			break;
		case 22:choose_main_double();
			break;
		case 211:choose_main_solo_quickrun();
			break;
		case 212: choose_main_solo_playerstdio();
			break;
		default:
			break;
		}
	}
	_getch();
	closegraph();
	return 0;
}

void startup_all()
{
	initgraph(Width, High);
	//界面0、1
	loadimage(&bk0_img, _T("D:\\bk2.jpg"), Width, High);
	loadimage(&bk10_img, _T("D:\\bk1.png"), 300, 300);
	loadimage(&bk1h0_img, _T("D:\\bk1hide.png"), 300, 300);
	loadimage(&fifa0_img, _T("D:\\fifa.png"), 198, 60);
	//界面2
	loadimage(&img_backgound2, _T("D:\\足球场2.jpg"));
	//界面21
	loadimage(&img_background21, _T("D:\\足球场21.jpg"));
	//界面 22 211
	loadimage(&china, _T("D:\\china.jpg"));
	loadimage(&korea, _T("D:\\korea.jpg"));
	loadimage(&britain, _T("D:\\britain.jpg"));
	loadimage(&russia, _T("D:\\russia.jpg"));
	loadimage(&german, _T("D:\\german.jpg"));
	loadimage(&choosebk, _T("D:\\choosebk.gif"));
	loadimage(&formation433, _T("D:\\433.jpg"), 200, 150);
	loadimage(&formation343, _T("D:\\343.jpg"), 200, 150);
	loadimage(&formation334, _T("D:\\334.jpg"), 200, 150);
	//界面212
	loadimage(&bk_img, _T("D:\\背景.jpg"), Width, High);
	loadimage(&c_img[0], _T("D:\\c1.jpg"), 120, 83);
	loadimage(&c_img[1], _T("D:\\c2.jpg"), 120, 83);
	loadimage(&c_img[2], _T("D:\\c3.jpg"), 120, 83);
	loadimage(&c_img[3], _T("D:\\c4.jpg"), 120, 83);
	loadimage(&c_img[4], _T("D:\\c5.jpg"), 120, 83);
	//setbkmode()TRANSPARENT);
	BeginBatchDraw();
}

void signin()
{
	int condition=1;
	while (condition)
	{
		condition = show_input_0();
	}
}

void registerup()
{
	int condition = 1;
	while (condition)
	{
		condition = show_input_1();
	}
}

void choose_main()
{
	int condition = 1;
	while (condition)
	{
		show_2();
		condition = updateWithInput_2();
	}
}

void choose_main_solo()
{
	int condition = 1;
	while (condition)
	{
		show_21();
		condition = updateWithInput_21();
	}
}

void choose_main_double()
{
	int condition = 1;
	
	while (condition)
	{
		show_22();
		condition = updateWithinput_22();
	}
}

void choose_main_solo_quickrun()
{
	int condition = 1;
	while (condition)
	{
		show_211();
		condition = updateWithInput_211();
	}
}

void choose_main_solo_playerstdio() 
{
	putimage(0, 0, &bk_img);
	static int speed_x = 105, skill_x = 105, shoot_x = 105, pass_x = 405, defend_x = 405, power_x = 405, all_x = 539, country, isput = 0;//能力条的位置x，国家值，是否输入名字
	char speed = 0, skill = 0, shoot = 0, pass = 0, defend = 0, power = 0;//能力的数字字符
	TCHAR all[3];//总能力的输出字符
	int i, mark = 0, if_fp = 0;//mark判断点击的哪个能力，if_fp是否确定之后操作文件
	MOUSEMSG m;		//鼠标
	FILE* fp, * fp_temp;//文件
	Player stdioplayer;//结构体
	while (MouseHit())
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 80 && mouse_x <= 220 && mouse_y >= 60 && mouse_y <= 80)//鼠标是不是在名字处
			isput = 1;
		else if (m.uMsg == WM_LBUTTONDOWN)
			isput = 0;

		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 260 && mouse_x <= 380 && mouse_y >= 20 && mouse_y <= 100)//国旗数值
			country = (country + 1) % 5;
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 120 && mouse_x <= 250 && mouse_y >= 410 && mouse_y <= 460 && all_x >= 410 && all_x <= 412)//确认并且操作文件
		{
			table = 21;
			if_fp = 1;
		}

		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 390 && mouse_x <= 520 && mouse_y >= 410 && mouse_y <= 460)//取消退出
			table = 21;
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 105 && mouse_x < 235 && mouse_y >= 155 && mouse_y <= 175)//能力框移动
		{
			speed_x = m.x;
			mark = 1;
		}
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 105 && mouse_x < 235 && mouse_y >= 235 && mouse_y <= 255)
		{
			skill_x = m.x;
			mark = 2;
		}
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 105 && mouse_x < 235 && mouse_y >= 315 && mouse_y <= 335)
		{
			shoot_x = m.x;
			mark = 3;
		}
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 405 && mouse_x < 535 && mouse_y >= 155 && mouse_y <= 175)
		{
			pass_x = m.x;
			mark = 4;
		}
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 405 && mouse_x < 535 && mouse_y >= 235 && mouse_y <= 255)
		{
			defend_x = m.x;
			mark = 5;
		}
		if (m.uMsg == WM_LBUTTONDOWN && mouse_x >= 405 && mouse_x < 535 && mouse_y >= 315 && mouse_y <= 335)
		{
			power_x = m.x;
			mark = 6;
		}
	}
	all_x = (42 - (speed_x - 105) / 13 - (skill_x - 105) / 13 - (shoot_x - 105) / 13 - (pass_x - 405) / 13 - (defend_x - 405) / 13 - (power_x - 405) / 13) * 3 + 410;//总能力随分配减少
	if (all_x < 410)//阻止总能力值为负数
	{
		while (all_x < 410)
		{
			switch (mark)
			{
			case 1:speed_x -= 1;
				break;
			case 2:skill_x -= 1;
				break;
			case 3:shoot_x -= 1;
				break;
			case 4:pass_x -= 1;
				break;
			case 5:defend_x -= 1;
				break;
			case 6:power_x -= 1;
				break;
			default:
				break;
			}
			all_x = (42 - (speed_x - 105) / 13 - (skill_x - 105) / 13 - (shoot_x - 105) / 13 - (pass_x - 405) / 13 - (defend_x - 405) / 13 - (power_x - 405) / 13) * 3 + 410;
		}
	}
	setcolor(RGB(255, 236, 139));
	setfillcolor(RGB(255, 236, 139));//暗边框
	bar(57, 17, 583, 383);
	bar(145, 415, 235, 465);
	bar(415, 415, 505, 465);

	setcolor(WHITE);
	setfillcolor(WHITE);
	bar(60, 20, 580, 380);//大框架


	setcolor(RGB(211, 211, 211));
	setfillcolor(RGB(211, 211, 211));
	if (mouse_x >= 120 && mouse_x <= 250 && mouse_y >= 410 && mouse_y <= 460)
	{
		setcolor(WHITE);
		setfillcolor(WHITE);
		bar(140, 410, 230, 460);//确认键亮
	}
	else
	{
		setcolor(RGB(211, 211, 211));
		setfillcolor(RGB(211, 211, 211));
		bar(140, 410, 230, 460);//确认键暗
	}
	if (mouse_x >= 390 && mouse_x <= 520 && mouse_y >= 410 && mouse_y <= 460)
	{
		setcolor(WHITE);
		setfillcolor(WHITE);
		bar(410, 410, 500, 460);//取消键位亮
	}
	else
	{
		setcolor(RGB(211, 211, 211));
		setfillcolor(RGB(211, 211, 211));
		bar(410, 410, 500, 460);//取消键位暗
	}
	settextcolor(BLACK);
	settextstyle(40, 0, _T("黑体"));
	outtextxy(145, 415, _T("确认"));
	outtextxy(415, 415, _T("取消"));

	setcolor(RED);
	setfillcolor(RED);
	bar(60, 97, 260, 100);//中上线
	bar(380, 97, 580, 100);


	putimage(260, 17, &c_img[country]);//国家旗帜
	if (mouse_x >= 260 && mouse_x <= 380 && mouse_y >= 20 && mouse_y <= 100)//国家点击亮色暗色效果
	{
		setcolor(WHITE);
		rectangle(260, 17, 380, 100);
	}
	else
	{
		setcolor(BLACK);
		rectangle(260, 17, 380, 100);
	}


	setcolor(RGB(255, 215, 0));//总能力条后景
	setfillcolor(RGB(255, 215, 0));
	bar(77, 57, 243, 83);
	bar(407, 57, 543, 83);

	setcolor(RED);
	setfillcolor(RED);
	bar(410, 60, all_x, 80);//红色能力条

	setcolor(RGB(190, 190, 190));
	setfillcolor(RGB(190, 190, 190));
	if (isput == 0)
		bar(80, 60, 240, 80);//灰色名字框
	else if (isput == 1)
	{
		setcolor(WHITE);
		setfillcolor(WHITE);
		bar(80, 60, 240, 80);//白色名字框
	}
	for (i = 0;i <= 42;i++)
	{
		if (all_x >= 410 + i * 3 && all_x < 413 + i * 3)//总能力数值字符串
		{
			all[0] = '0' + i / 10;
			all[1] = '0' + i % 10;
			all[2] = '\0';
		}
	}
	setcolor(RGB(190, 190, 190));
	setfillcolor(RGB(190, 190, 190));//小能力条

	bar(100, 150, 240, 180);
	bar(100, 230, 240, 260);
	bar(100, 310, 240, 340);

	bar(400, 150, 540, 180);
	bar(400, 230, 540, 260);
	bar(400, 310, 540, 340);

	settextcolor(BLACK);
	settextstyle(25, 0, _T("黑体"));
	outtextxy(65, 25, _T("name:"));
	outtextxy(390, 25, _T("能力点:"));
	outtextxy(550, 58, all);

	settextcolor(BLACK);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(150, 120, _T("速度"));
	outtextxy(150, 200, _T("控球"));
	outtextxy(150, 280, _T("射门"));

	outtextxy(450, 120, _T("传球"));
	outtextxy(450, 200, _T("防守"));
	outtextxy(450, 280, _T("体力"));

	ability_1(speed_x, speed, 105, 155, 235, 175);//能力值字符输出
	ability_1(skill_x, skill, 105, 235, 235, 255);
	ability_1(shoot_x, shoot, 105, 315, 235, 335);
	ability_2(pass_x, pass, 405, 155, 535, 175);
	ability_2(defend_x, defend, 405, 235, 535, 255);
	ability_2(power_x, power, 405, 315, 535, 335);

	name_put(isput);//名字输入isput为1可以输入，0只显示

	FlushBatchDraw();

	if (if_fp == 1)//文件操作
	{
		int if_re = 0;
		char name1[20];
		fopen_s(&fp, "D:\\data.txt", "rt+");//rt+和wt+分别是打开读写和建立或者打开读写
		fopen_s(&fp_temp, "D:\\temp.txt", "wt+");

		while (fscanf_s(fp, "%d %s %f %f %f %f %f %f %f %f %f %f %f %f", &stdioplayer.player_num, name1, &stdioplayer.speed_ac, &stdioplayer.speed_max,
			&stdioplayer.control_radius, &stdioplayer.control_skill, &stdioplayer.shoot_1, &stdioplayer.shoot_2, &stdioplayer.shoot_3, &stdioplayer.enduce,
			&stdioplayer.pass_short, &stdioplayer.pass_long, &stdioplayer.defend, &stdioplayer.power) != NULL && !feof(fp))
		{
			if (stdioplayer.player_num == country + 1)
			{
				if_re = 1;
				if (tips_ifreplace())
				{
					stdioplayer.player_num = country + 1;
					stdioplayer.speed_ac = ability_num(speed_x, 105);//能力值具现
					stdioplayer.speed_max = ability_num(speed_x, 105);
					stdioplayer.control_radius = ability_num(skill_x, 105);
					stdioplayer.control_skill = ability_num(skill_x, 105);
					stdioplayer.shoot_1 = ability_num(shoot_x, 105);
					stdioplayer.shoot_2 = ability_num(shoot_x, 105);
					stdioplayer.shoot_3 = ability_num(shoot_x, 105);
					stdioplayer.enduce = ability_num(shoot_x, 105);
					stdioplayer.pass_short = ability_num(pass_x, 405);
					stdioplayer.pass_long = ability_num(pass_x, 405);
					stdioplayer.defend = ability_num(defend_x, 405);
					stdioplayer.power = ability_num(power_x, 405);

					_tcscpy_s(stdioplayer.name, name);
					fprintf(fp_temp, "%d ", stdioplayer.player_num);
					for (i = 0;i < name_position;i++)
						fprintf_s(fp_temp, "%c", stdioplayer.name[i]);
					fprintf(fp_temp, " %f %f %f %f %f %f %f %f %f %f %f %f\n", stdioplayer.speed_ac, stdioplayer.speed_max,
						stdioplayer.control_radius, stdioplayer.control_skill, stdioplayer.shoot_1, stdioplayer.shoot_2, stdioplayer.shoot_3, stdioplayer.enduce,
						stdioplayer.pass_short, stdioplayer.pass_long, stdioplayer.defend, stdioplayer.power);
				}
				else
				{
					fprintf(fp_temp, "%d ", stdioplayer.player_num);
					fprintf(fp_temp, "%s", name1);
					fprintf(fp_temp, " %f %f %f %f %f %f %f %f %f %f %f %f\n", stdioplayer.speed_ac, stdioplayer.speed_max,
						stdioplayer.control_radius, stdioplayer.control_skill, stdioplayer.shoot_1, stdioplayer.shoot_2, stdioplayer.shoot_3, stdioplayer.enduce,
						stdioplayer.pass_short, stdioplayer.pass_long, stdioplayer.defend, stdioplayer.power);
					table = 212;	//如果取消任然在212并且不写入自此数据
				}
			}
			else
			{
				fprintf(fp_temp, "%d ", stdioplayer.player_num);
				fprintf(fp_temp, "%s", name1);
				fprintf(fp_temp, " %f %f %f %f %f %f %f %f %f %f %f %f\n", stdioplayer.speed_ac, stdioplayer.speed_max,
					stdioplayer.control_radius, stdioplayer.control_skill, stdioplayer.shoot_1, stdioplayer.shoot_2, stdioplayer.shoot_3, stdioplayer.enduce,
					stdioplayer.pass_short, stdioplayer.pass_long, stdioplayer.defend, stdioplayer.power);
			}
		}

		if (if_re == 0)
		{
			stdioplayer.player_num = country + 1;
			stdioplayer.speed_ac = ability_num(speed_x, 105);//能力值具现
			stdioplayer.speed_max = ability_num(speed_x, 105);
			stdioplayer.control_radius = ability_num(skill_x, 105);
			stdioplayer.control_skill = ability_num(skill_x, 105);
			stdioplayer.shoot_1 = ability_num(shoot_x, 105);
			stdioplayer.shoot_2 = ability_num(shoot_x, 105);
			stdioplayer.shoot_3 = ability_num(shoot_x, 105);
			stdioplayer.enduce = ability_num(shoot_x, 105);
			stdioplayer.pass_short = ability_num(pass_x, 405);
			stdioplayer.pass_long = ability_num(pass_x, 405);
			stdioplayer.defend = ability_num(defend_x, 405);
			stdioplayer.power = ability_num(power_x, 405);

			_tcscpy_s(stdioplayer.name, name);
			fprintf(fp_temp, "%d ", stdioplayer.player_num);
			for (i = 0;i < name_position;i++)
				fprintf_s(fp_temp, "%c", stdioplayer.name[i]);
			fprintf(fp_temp, " %f %f %f %f %f %f %f %f %f %f %f %f\n", stdioplayer.speed_ac, stdioplayer.speed_max,
				stdioplayer.control_radius, stdioplayer.control_skill, stdioplayer.shoot_1, stdioplayer.shoot_2, stdioplayer.shoot_3, stdioplayer.enduce,
				stdioplayer.pass_short, stdioplayer.pass_long, stdioplayer.defend, stdioplayer.power);
		}
		fclose(fp);
		fclose(fp_temp);
		remove("D:\\data.txt");//删除原先文档
		rename("D:\\temp.txt", "D:\\data.txt");//改名现在文档
	}
}

int show_input_0()
{
	MOUSEMSG m;//鼠标
	static int isput_1 = 0, isput_2 = 0;//判断是否在输入

	while (MouseHit())
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 215 && m.x <= 285 && m.y >= 325 && m.y <= 365)//界面转换
		{
			table = 1;
			FlushBatchDraw();
			return 0;
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 85 && m.x <= 155 && m.y >= 325 && m.y <= 365)
		{
			table = 2;
			return 0;
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 120 && m.x <= 300 && m.y >= 180 && m.y <= 200)//输入账号
		{
			isput_1 = 1;
		}
		else if (m.uMsg == WM_LBUTTONDOWN)
		{
			isput_1 = 0;
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 120 && m.x <= 300 && m.y >= 240 && m.y <= 260)//输入密码
		{
			isput_2 = 1;
		}
		else if (m.uMsg == WM_LBUTTONDOWN)
		{
			isput_2 = 0;
		}
	}


	putimage(0, 0, &bk0_img);
	putimage(85, 0, &fifa0_img);
	putimage(330, 70, &bk1h0_img, NOTSRCERASE);//遮蔽图
	putimage(330, 70, &bk10_img, SRCINVERT);

	setbkmode(TRANSPARENT);
	setcolor(GREEN);
	setfillcolor(WHITE);
	bar(50, 80, 320, 400);//大框

	settextcolor(BLACK);
	settextstyle(50, 0, _T("黑体"));//字
	outtextxy(135, 90, _T("登录"));

	settextstyle(20, 0, _T("黑体"));
	outtextxy(65, 180, _T("账号"));
	outtextxy(65, 240, _T("密码"));


	if (isput_1 == 0)
	{
		setcolor(RGB(220, 220, 220));//账号密码位置
		setfillcolor(RGB(220, 220, 220));
		bar(120, 180, 300, 200);
	}

	else
	{
		setcolor(RGB(255, 250, 240));//账号密码位置
		setfillcolor(RGB(255, 250, 240));
		bar(120, 180, 300, 200);
	}
	if (isput_2 == 0)
	{
		setcolor(RGB(220, 220, 220));//账号密码位置
		setfillcolor(RGB(220, 220, 220));
		bar(120, 240, 300, 260);
	}

	else
	{
		setcolor(RGB(255, 250, 240));//账号密码位置
		setfillcolor(RGB(255, 250, 240));
		bar(120, 240, 300, 260);
	}

	setcolor(BLACK);//账号密码黑外框
	rectangle(120, 180, 300, 200);
	rectangle(120, 240, 300, 260);

	setcolor(RGB(83, 134, 139));
	setfillcolor(RGB(83, 134, 139));
	bar(95, 335, 155, 365);//登录键斜底色

	if (mouse_x >= 85 && mouse_x <= 155 && mouse_y >= 325 && mouse_y <= 365)//登录键
	{
		setcolor(RGB(152, 245, 255));
		setfillcolor(RGB(152, 245, 255));
		bar(90, 330, 150, 360);
	}
	else
	{
		setcolor(RGB(211, 211, 211));
		setfillcolor(RGB(211, 211, 211));
		bar(90, 330, 150, 360);
	}

	setcolor(RGB(83, 134, 139));
	setfillcolor(RGB(83, 134, 139));
	bar(225, 335, 285, 365);//注册键斜底色

	if (mouse_x >= 215 && mouse_x <= 285 && mouse_y >= 325 && mouse_y <= 365)//注册键
	{
		setcolor(RGB(152, 245, 255));
		setfillcolor(RGB(152, 245, 255));
		bar(220, 330, 280, 360);
	}
	else
	{
		setcolor(RGB(211, 211, 211));
		setfillcolor(RGB(211, 211, 211));
		bar(220, 330, 280, 360);
	}

	setcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(90, 330, _T("登录"));
	outtextxy(220, 330, _T("注册"));

	name_put1(isput_1);//账号输入显示
	name_put2(isput_2);//密码输入显示
	if (isput_1 == 0 && isput_2 == 0)//都不在位置上灰框，清除输入流
		if (_kbhit())
			_getch();
	FlushBatchDraw();
	return 1;
}

void name_put1(int isput)
{
	int i;
	char input;

	if (isput == 1)
	{
		if (_kbhit())//账号输入
		{
			input = _getch();
			if (input != 8)
				put_1[put_position1++] = input;
			else
				if (put_position1 > 0)
					put_position1--;
		}
	}
	for (i = 0;i < put_position1;i++)//账号显示
	{
		settextcolor(BLACK);
		settextstyle(18, 0, _T("黑体"));
		outtextxy(123 + i * 10, 180, put_1[i]);
	}

}
void name_put2(int isput)
{
	int i;
	char input;

	if (isput == 1)//密码
	{
		if (_kbhit())
		{
			input = _getch();
			if (input != 8)
				put_2[put_position2++] = input;
			else
				if (put_position2 > 0)
					put_position2--;
		}
	}
	for (i = 0;i < put_position2;i++)
	{
		settextcolor(BLACK);
		settextstyle(18, 0, _T("黑体"));
		outtextxy(123 + i * 10, 240, put_2[i]);
	}
}

int show_input_1()
{
	MOUSEMSG m;
	static int risput_1 = 0, risput_2 = 0;//注册键位界面的判断
	int if_fp;
	char tmep[20];
	while (MouseHit())
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
		if (m.uMsg == WM_LBUTTONUP && m.x >= 160 && m.x <= 230 && m.y >= 325 && m.y <= 365)
		{
			table = 0;
			if_fp = 1;
			file_do(if_fp);
			return 0;
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 120 && m.x <= 300 && m.y >= 180 && m.y <= 200)
			risput_1 = 1;
		else if (m.uMsg == WM_LBUTTONDOWN)
			risput_1 = 0;
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 120 && m.x <= 300 && m.y >= 240 && m.y <= 260)
			risput_2 = 1;
		else if (m.uMsg == WM_LBUTTONDOWN)
			risput_2 = 0;
	}

	putimage(0, 0, &bk0_img);
	putimage(85, 0, &fifa0_img);
	putimage(330, 70, &bk1h0_img, NOTSRCERASE);
	putimage(330, 70, &bk10_img, SRCINVERT);
	setbkmode(TRANSPARENT);
	setcolor(GREEN);
	setfillcolor(WHITE);
	bar(50, 80, 320, 400);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("黑体"));
	outtextxy(135, 90, _T("注册"));

	settextstyle(20, 0, _T("黑体"));
	outtextxy(65, 180, _T("账号"));
	outtextxy(65, 240, _T("密码"));

	if (risput_1 == 0)
	{
		setcolor(RGB(220, 220, 220));//账号密码位置
		setfillcolor(RGB(220, 220, 220));
		bar(120, 180, 300, 200);
	}
	else
	{
		setcolor(RGB(255, 250, 240));//账号密码位置
		setfillcolor(RGB(255, 250, 240));
		bar(120, 180, 300, 200);		
		//scanf_s("%s", pe.account);
		//fopen_s(&fp, pe.account, "r");
		//fopen_s(&fp, pe.account, "w");
	}

	if (risput_2 == 0)
	{
		setcolor(RGB(220, 220, 220));//账号密码位置
		setfillcolor(RGB(220, 220, 220));
		bar(120, 240, 300, 260);
		
	}
	else
	{
		setcolor(RGB(255, 250, 240));//账号密码位置
		setfillcolor(RGB(255, 250, 240));
		bar(120, 240, 300, 260);
		//scanf_s("%s", pe.code);
	}

	setcolor(BLACK);
	rectangle(120, 180, 300, 200);
	rectangle(120, 240, 300, 260);

	setcolor(RGB(83, 134, 139));
	setfillcolor(RGB(83, 134, 139));
	bar(170, 335, 230, 365);

	if (mouse_x >= 160 && mouse_x <= 230 && mouse_y >= 325 && mouse_y <= 365)
	{
		setcolor(RGB(152, 245, 255));
		setfillcolor(RGB(152, 245, 255));
		bar(165, 330, 225, 360);
	}
	else
	{
		setcolor(RGB(211, 211, 211));
		setfillcolor(RGB(211, 211, 211));
		bar(165, 330, 225, 360);
	}

	setcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(165, 330, _T("注册"));

	name_put1(risput_1);
	name_put2(risput_2);

	FlushBatchDraw();
	//文件操作
	//account_put();

	return 1;
}

void show_2()
{

	//显示背景图
	putimage(0, 0, &img_backgound2);
	//putimage(0, 0, &img_ball1, NOTSRCERASE);
	//putimage(0, 0, &img_ball2, SRCINVERT);
	setcolor(BLACK);
	setfillcolor(RGB(211, 211, 211));
	setlinestyle(PS_SOLID, 4);
	fillrectangle(80, 60, 560, 420);//展示边框
	line(80, 240, 560, 240);
	line(320, 60, 320, 420);
	fillcircle(640, 480, 60);
	TCHAR n[] = _T("x");
	TCHAR s1[] = _T("单人游戏");
	TCHAR s2[] = _T("双人游戏");
	TCHAR s3[] = _T("帮助");
	TCHAR s4[] = _T("关于");
	TCHAR s5[] = _T("返回目录");

	//移动鼠标选项颜色变化
	MOUSEMSG m;
	while (MouseHit())
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
	}
	settextstyle(40, 0, _T("宋体"));
	setcolor(BLACK);
	if ((mouse_x >= 80) && (mouse_x <= 320) && (mouse_y >= 60) && (mouse_y <= 240))
	{
		setfillcolor(RGB(46, 139, 87));;
		setcolor(BLACK);
		fillrectangle(80, 60, 320, 240);
		outtextxy(120, 120, s1);
	}
	else
	{
		setfillcolor(WHITE);
		setcolor(BLACK);
		fillrectangle(80, 60, 320, 240);
		setcolor(GREEN);
		outtextxy(120, 120, s1);
	}
	if ((mouse_x >= 320) && (mouse_x <= 560) && (mouse_y >= 60) && (mouse_y <= 240))
	{
		setfillcolor(RGB(46, 139, 87));
		setcolor(BLACK);
		fillrectangle(320, 60, 560, 240);
		outtextxy(360, 120, s2);
	}
	else
	{
		setfillcolor(WHITE);
		setcolor(BLACK);
		fillrectangle(320, 60, 560, 240);
		setcolor(GREEN);
		outtextxy(360, 120, s2);
	}
	if ((mouse_x >= 80) && (mouse_x <= 320) && (mouse_y >= 240) && (mouse_y <= 420))
	{
		setfillcolor(RGB(46, 139, 87));
		setcolor(BLACK);
		fillrectangle(80, 240, 320, 420);
		outtextxy(150, 300, s3);
	}
	else
	{
		setfillcolor(WHITE);
		setcolor(BLACK);
		fillrectangle(80, 240, 320, 420);
		setcolor(GREEN);
		outtextxy(150, 300, s3);
	}
	if ((mouse_x >= 320) && (mouse_x <= 560) && (mouse_y >= 240) && (mouse_y <= 420))
	{
		setfillcolor(RGB(46, 139, 87));
		setcolor(BLACK);
		fillrectangle(320, 240, 560, 420);
		outtextxy(390, 300, s4);
	}
	else
	{
		setfillcolor(WHITE);
		setcolor(BLACK);
		fillrectangle(320, 240, 560, 420);
		setcolor(GREEN);
		outtextxy(390, 300, s4);
	}
	if (((mouse_x - 640) * (mouse_x - 640) + (mouse_y - 480) * (mouse_y - 480)) <= 3600)
	{
		setcolor(RED);
		outtextxy(600, 440, n);
		setcolor(WHITE);
		settextstyle(25, 0, _T("宋体"));
		outtextxy(480, 440, s5);
	}
	else
	{
		settextstyle(40, 0, _T("宋体"));
		setcolor(BLACK);
		outtextxy(600, 440, n);
	}
	FlushBatchDraw();
}

int updateWithInput_2()
{
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		//根据鼠标位置改变cable，进入下一个界面
		if ((m.uMsg == WM_LBUTTONDOWN) && (mouse_x >= 80) && (mouse_x <= 320) && (mouse_y >= 60) && (mouse_y <= 240))
		{
			table = 21;
			return 0;
		}
		if ((m.uMsg == WM_LBUTTONDOWN) && (mouse_x >= 320) && (mouse_x <= 560) && (mouse_y >= 60) && (mouse_y <= 240))
		{
			table = 22;
			return 0;
		}
		if ((m.uMsg == WM_LBUTTONDOWN) && (mouse_x >= 80) && (mouse_x <= 320) && (mouse_y >= 240) && (mouse_y <= 420) )
		{
			table = 23;
			return 0;
		}
		if ((m.uMsg == WM_LBUTTONDOWN) && (mouse_x >= 320) && (mouse_x <= 560) && (mouse_y >= 240) && (mouse_y <= 420))
		{
			table = 24;
			return 0;
		}
		if ((m.uMsg == WM_LBUTTONDOWN) && (((mouse_x - 640)* (mouse_x - 640) + (mouse_y - 480)* (mouse_y - 480)) <= 3600 ))
		{
			table = 0;
			return 0;
		}
	}
	return 1;
}

void show_21()
{
	putimage(0, 0, &img_background21);
	setcolor(BLACK);
	setfillcolor(YELLOW);
	setlinestyle(PS_SOLID, 4);
	fillrectangle(0, 30, 300, 450);
	fillcircle(640, 480, 60);
	setfillcolor(WHITE);
	setcolor(WHITE);
	fillrectangle(40, 65, 260, 205);
	fillrectangle(40, 275, 260, 415);
	//返回选项
	TCHAR n[] = _T("X");
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("宋体"));
	//显示选项

	TCHAR s1[] = _T("快速游戏");
	TCHAR s2[] = _T("球员创立");
	TCHAR s3[] = _T("返回上一界面");
	setcolor(GREEN);
	settextstyle(40, 0, _T("宋体"));
	//鼠标移动改变颜色；
	MOUSEMSG m;

	if (MouseHit())
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
	}

	if ((mouse_x >= 40) && (mouse_x <= 260) && (mouse_y >= 65) && (mouse_y <= 205))
	{
		setcolor(BLACK);
		fillrectangle(40, 65, 260, 205);
		outtextxy(70, 115, s1);
	}
	else
	{
		setcolor(GREEN);
		outtextxy(70, 115, s1);
	}
	if ((mouse_x >= 40) && (mouse_x <= 260) && (mouse_y >= 275) && (mouse_y <= 415))
	{
		setcolor(BLACK);
		fillrectangle(40, 275, 260, 415);
		outtextxy(70, 325, s2);
	}
	else
	{
		setcolor(GREEN);
		outtextxy(70, 325, s2);
	}
	if (((mouse_x - 640) * (mouse_x - 640) + (mouse_y - 480) * (mouse_y - 480)) <= 3600)
	{
		setcolor(RED);
		outtextxy(600, 440, n);
		setcolor(WHITE);
		settextstyle(25, 0, _T("宋体"));
		outtextxy(410, 440, s3);
	}
	else
	{
		setcolor(BLACK);
		outtextxy(600, 440, n);
	}
	FlushBatchDraw();
}

int updateWithInput_21()
{
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
		//{	//根据鼠标位置改变cable，进入下一个界面
		if ((m.uMsg == WM_LBUTTONDOWN) && (mouse_x >= 40) && (mouse_x <= 260) && (mouse_y >= 65) && (mouse_y <= 205))
		{
			table = 211;
			return 0;
		}
		if ((m.uMsg == WM_LBUTTONDOWN) && (mouse_x >= 40) && (mouse_x <= 260) && (mouse_y >= 275) && (mouse_y <= 415))
		{
			table = 212;
			return 0;
		}
		if ((m.uMsg == WM_LBUTTONDOWN) && (((mouse_x - 640) * (mouse_x - 640) + (mouse_y - 480) * (mouse_y - 480)) <= 3600))
		{
			table = 2;
			return 0;
		}
	}
	return 1;
}

void show_22()
{
	settextstyle(20,0,_T("gbcbig.shx"));
	putimage(0, 0, &choosebk);
	putimage(60, 40, &china);
	putimage(376, 40, &china);
	putimage(60, 250, &formation433);
	putimage(376, 250, &formation334);
	setbkmode(TRANSPARENT);
	setcolor(WHITE);
	outtextxy(60, 20, _T("COUNTRY"));
	outtextxy(60, 195, _T("CHINA"));
	outtextxy(60, 230, _T("FORMATION"));
	outtextxy(60, 405, _T("4-3-3"));
	outtextxy(376, 20, _T("COUNTRY"));
	outtextxy(376, 195, _T("CHINA"));
	outtextxy(376, 230, _T("FORMATION"));
	outtextxy(376, 405, _T("4-3-3"));
	outtextxy(0, 315, _T(" P1"));
	outtextxy(0, 340, _T(" SIDE"));
	outtextxy(570, 315, _T(" P2"));
	outtextxy(570, 340, _T(" SIDE"));
	outtextxy(280, 270, _T("START"));
	outtextxy(290, 340, _T("BACK"));
	setcolor(BLACK);
	rectangle(59, 39, 257, 190);
	rectangle(59, 249, 260, 400);
	rectangle(375, 39, 573, 190);
	rectangle(376, 250, 576, 399);
	FlushBatchDraw();
	//防止开始时黑屏
}

int updateWithinput_22()
{
	int flagnum1 = 1, formnum1 = 1;
	int flagnum2 = 1, formnum2 = 1;
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
		if (mouse_x >= 60 && mouse_y >= 40 && mouse_x <= 260 && mouse_y <= 190)
		{
			setcolor(WHITE);
			rectangle(59, 39, 257, 190);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				if (flagnum1 < 5) flagnum1++;
				else flagnum1 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (mouse_x >= 52 && mouse_y >= 243 && mouse_x <= 264 && mouse_y <= 402) {
			setcolor(WHITE);
			rectangle(59, 249, 260, 400);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				if (formnum1 < 3) formnum1++;
				else formnum1 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (mouse_x >= 376 && mouse_y >= 40 && mouse_x <= 588 && mouse_y <= 190)
		{
			setcolor(WHITE);
			rectangle(375, 39, 573, 190);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				if (flagnum2 < 5) flagnum2++;
				else flagnum2 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (mouse_x >= 52 && mouse_y >= 243 && mouse_x <= 264 && mouse_y <= 402)
		{
			setcolor(WHITE);
			rectangle(59, 249, 260, 400);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				if (formnum1 < 3) formnum1++;
				else formnum1 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (mouse_x >= 375 && mouse_y >= 247 && mouse_x <= 580 && mouse_y <= 400)
		{
			setcolor(WHITE);
			rectangle(376, 250, 576, 399);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				if (formnum2 < 3) formnum2++;
				else formnum2 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (mouse_x >= 280 && mouse_y >= 270 && mouse_x <= 360 && mouse_y <= 290)
		{
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(280, 270, _T("START"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				table = 221;
				return 0;												//双人游戏开始接口 调用flagnum12 formnum12即可（如果P1 P2国家一样，continue即可）
			}
		}
		else if (mouse_x >= 290 && mouse_y >= 340 && mouse_x <= 330 && mouse_y <= 360)
		{
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(290, 340, _T("BACK"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				table = 2;		//返回菜单接口
				return 0;
			}
		}
		else
		{
			setcolor(BLACK);
			rectangle(59, 39, 257, 190);
			rectangle(59, 249, 260, 400);
			rectangle(375, 39, 573, 190);
			rectangle(376, 250, 576, 399);
			//rectangle(298, 270, 343, 285);
			setcolor(WHITE);
			outtextxy(280, 270, _T("START"));
			outtextxy(290, 340, _T("BACK"));
			FlushBatchDraw();
		}
	}
}


void newdraw_double(int flagnum1, int formnum1, int flagnum2, int formnum2)
{
	MOUSEMSG m;
	m = GetMouseMsg();
	putimage(0, 0, &choosebk);

	if (flagnum1 == 1)
	{																//P1选择栏目
		putimage(60, 40, &china);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("CHINA"));
	}
	else if (flagnum1 == 2) {
		putimage(60, 40, &russia);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("RUSSIA"));
	}
	else if (flagnum1 == 3) {
		putimage(60, 40, &korea);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("KOREA"));
	}
	else if (flagnum1 == 4) {
		putimage(60, 40, &german);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("GERMAN"));
	}
	else if (flagnum1 == 5) {
		putimage(60, 40, &britain);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("BRITAIN"));
	}

	if (formnum1 == 1) {
		putimage(60, 250, &formation433);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("4-3-3"));
		outtextxy(0, 315, _T(" P1"));
		outtextxy(0, 340, _T(" SIDE"));
	}
	else if (formnum1 == 2) {
		putimage(60, 250, &formation343);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-4-3"));
		outtextxy(0, 315, _T(" P1"));
		outtextxy(0, 340, _T(" SIDE"));
	}
	else if (formnum1 == 3) {
		putimage(60, 250, &formation334);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-3-4"));
		outtextxy(0, 315, _T(" P1"));
		outtextxy(0, 340, _T(" SIDE"));
	}

	if (flagnum2 == 1) {																//P2选择栏目
		putimage(376, 40, &china);
		setcolor(WHITE);
		outtextxy(376, 20, _T("COUNTRY"));
		outtextxy(376, 195, _T("CHINA"));
	}
	else if (flagnum2 == 2) {
		putimage(376, 40, &russia);
		setcolor(WHITE);
		outtextxy(376, 20, _T("COUNTRY"));
		outtextxy(376, 195, _T("RUSSIA"));
	}
	else if (flagnum2 == 3) {
		putimage(376, 40, &korea);
		setcolor(WHITE);
		outtextxy(376, 20, _T("COUNTRY"));
		outtextxy(376, 195, _T("KOREA"));
	}
	else if (flagnum2 == 4) {
		putimage(376, 40, &german);
		setcolor(WHITE);
		outtextxy(376, 20, _T("COUNTRY"));
		outtextxy(376, 195, _T("GERMAN"));
	}
	else if (flagnum2 == 5) {
		putimage(376, 40, &britain);
		setcolor(WHITE);
		outtextxy(376, 20, _T("COUNTRY"));
		outtextxy(376, 195, _T("BRITAIN"));
	}

	if (formnum2 == 1) {
		putimage(376, 250, &formation334);
		outtextxy(376, 230, _T("FORMATION"));
		outtextxy(376, 405, _T("4-3-3"));
		outtextxy(570, 315, _T(" P2"));
		outtextxy(570, 340, _T(" SIDE"));
	}
	else if (formnum2 == 2) {
		putimage(376, 250, &formation343);
		outtextxy(376, 230, _T("FORMATION"));
		outtextxy(376, 405, _T("3-4-3"));
		outtextxy(570, 315, _T(" P2"));
		outtextxy(570, 340, _T(" SIDE"));
	}
	else if (formnum2 == 3) {
		putimage(376, 250, &formation433);
		outtextxy(376, 230, _T("FORMATION"));
		outtextxy(376, 405, _T("3-3-4"));
		outtextxy(570, 315, _T(" P2"));
		outtextxy(570, 340, _T(" SIDE"));
	}
	setcolor(BLACK);
	rectangle(59, 39, 257, 190);
	rectangle(59, 249, 260, 400);
	rectangle(375, 39, 573, 190);
	rectangle(376, 250, 576, 399);
	setcolor(WHITE);
	outtextxy(280, 270, _T("START"));
	outtextxy(290, 340, _T("BACK"));

	FlushBatchDraw();
}

void firststep() {																		//初始单人界面
	int flagnum = 1, formnum = 1;
	putimage(0, 0, &choosebk);
	putimage(60, 40, &china);
	putimage(60, 250, &formation433);
	setbkmode(TRANSPARENT);
	setcolor(WHITE);
	outtextxy(60, 20, _T("COUNTRY"));
	outtextxy(60, 195, _T("CHINA"));
	outtextxy(60, 230, _T("FORMATION"));
	outtextxy(60, 405, _T("4-3-3"));
	outtextxy(0, 315, _T(" OUR"));
	outtextxy(0, 340, _T(" SIDE"));
	outtextxy(300, 270, _T("START"));
	outtextxy(305, 340, _T("BACK"));
	setcolor(RGB(0, 205, 0));
	setfillcolor(RGB(0, 139, 0));
	fillrectangle(370, 0, 640, 480);
	for (int i = 2;i * 20 < 480;i++) {
		rectangle(370, i * 20, 640, i * 20 + 20);
	}
	setcolor(WHITE);
	outtextxy(373, 15, _T("PLAYER LIST"));
	outtextxy(373, 40, _T("Striker Soccer"));
	outtextxy(373, 120, _T("Center Forward"));
	outtextxy(373, 200, _T("Football Back"));
	outtextxy(373, 300, _T("Reserve Player"));
	outtextxy(373, 420, _T("Special Player"));

	setcolor(BLACK);
	rectangle(59, 39, 257, 190);
	rectangle(59, 249, 260, 400);

	drawonetab(flagnum, formnum);
	drawtwotab();
	drawthrtab(flagnum);
	FlushBatchDraw();																		//防止开始时黑屏
}

void drawonetab(int flagnum, int formnum) {
	setcolor(WHITE);
	for (int num = flagnum * 10, i = 1;i < 13;i++)
	{
		if (list_block[formnum][i] == 1) continue;
		else if (num == record)
		{
			setcolor(BLACK);
			outtextxy(440, i * 20 + 40, player_code[num]);
			setcolor(WHITE);
		}
		else outtextxy(440, i * 20 + 40, player_code[num]);
		num++;
	}
}

void drawtwotab() {
	setcolor(WHITE);
	for (int num = 60, i = 0;i < 5;i++, num++) {
		outtextxy(440, i * 20 + 320, player_code[num]);
		if (num == record) {
			setcolor(BLACK);
			outtextxy(440, i * 20 + 320, player_code[num]);
			setcolor(WHITE);
		}
	}
}

void drawthrtab(int flagnum)
{
	setcolor(WHITE);
	int num = flagnum;
	if (strcmp((const char *)player_code[num], "0") != 0 && num != record)		//这里有个强制转换
		outtextxy(440, 440, player_code[num]);
}

void initplayerlist()
{
	FILE* fp;
	int n, i = 0;
	TCHAR name[20];
	fopen_s(&fp, "D://player.txt", "r");										//读的是副本文档,拷贝编号和名字			******可能会出问题，需要测试一下******
	/*while (!feof(fp))
	{															//因为没有做结构体，所以读写格式需要更改一下
																				//这个demo所需要的是队员编号和名字，其他参数不重要
		fscanf_s(fp, _T("%d %s"), &n,name);										//这个地方会一直报错，网上查了一下，vs上的c写法有些不兼容，老哥可以想想办法试试看能不能改，不能问题也不是特别大
		player_num[i] = n;														//可以先把此部分while部分先注释掉，看效果
		strcpy_s(player_code[i], name);
		i++;
	}*/
	fclose(fp);
}

void remakeplayerlist(int flagnum, int formnum, int num) {						//交换队员名字的时候，对队员编号数组进行更改调换操作的函数
	TCHAR temps[20];
	int temp;
	_tcscpy_s(temps, player_code[num]);
	_tcscpy_s(player_code[num], player_code[record]);
	_tcscpy_s(player_code[record], temps);
	temp = player_num[num];
	player_num[num] = player_num[record];
	player_num[record] = temp;

	record = -1;

	drawonetab(flagnum, formnum);
	drawtwotab();
	drawthrtab(flagnum);
}

void newdraw_playerlist(int flagnum, int formnum) 
{
	setcolor(RGB(0, 205, 0));
	setfillcolor(RGB(0, 139, 0));
	fillrectangle(370, 0, 640, 480);
	for (int i = 2;i * 20 < 480;i++) 
	{
		rectangle(370, i * 20, 640, i * 20 + 20);
	}
	setcolor(WHITE);
	if (formnum == 1) 
	{
		outtextxy(373, 15, _T("PLAYER LIST"));
		outtextxy(373, 40, _T("Striker Soccer"));
		outtextxy(373, 120, _T("Center Forward"));
		outtextxy(373, 200, _T("Football Back"));
		outtextxy(373, 300, _T("Reserve Player"));
		outtextxy(373, 420, _T("Special Player"));
	}
	else if (formnum == 2) 
	{
		outtextxy(373, 15, _T("PLAYER LIST"));
		outtextxy(373, 40, _T("Striker Soccer"));
		outtextxy(373, 120, _T("Center Forward"));
		outtextxy(373, 220, _T("Football Back"));
		outtextxy(373, 300, _T("Reserve Player"));
		outtextxy(373, 420, _T("Special Player"));
	}
	else if (formnum == 3) 
	{
		outtextxy(373, 15, _T("PLAYER LIST"));
		outtextxy(373, 40, _T("Striker Soccer"));
		outtextxy(373, 140, _T("Center Forward"));
		outtextxy(373, 220, _T("Football Back"));
		outtextxy(373, 300, _T("Reserve Player"));
		outtextxy(373, 420, _T("Special Player"));
	}
	drawonetab(flagnum, formnum);
	drawtwotab();
	drawthrtab(flagnum);
}

void newdraw_solo(int flagnum, int formnum)
{
	MOUSEMSG m;
	m = GetMouseMsg();
	putimage(0, 0, &choosebk);

	if (flagnum == 1) {
		putimage(60, 40, &china);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("CHINA"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (flagnum == 2) {
		putimage(60, 40, &russia);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("RUSSIA"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (flagnum == 3) {
		putimage(60, 40, &korea);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("KOREA"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (flagnum == 4) {
		putimage(60, 40, &german);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("GERMAN"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (flagnum == 5) {
		putimage(0, 0, &choosebk);
		putimage(60, 40, &britain);
		setcolor(WHITE);
		outtextxy(60, 20, _T("COUNTRY"));
		outtextxy(60, 195, _T("BRITAIN"));
		newdraw_playerlist(flagnum, formnum);
	}

	if (formnum == 1) {
		putimage(60, 250, &formation433);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("4-3-3"));
		outtextxy(0, 315, _T(" OUR"));
		outtextxy(0, 340, _T(" SIDE"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (formnum == 2) {
		putimage(60, 250, &formation343);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-4-3"));
		outtextxy(0, 315, _T(" OUR"));
		outtextxy(0, 340, _T(" SIDE"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (formnum == 3) {
		putimage(60, 250, &formation334);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-3-4"));
		outtextxy(0, 315, _T(" OUR"));
		outtextxy(0, 340, _T(" SIDE"));
		newdraw_playerlist(flagnum, formnum);
	}
	setcolor(BLACK);
	rectangle(59, 39, 257, 190);
	rectangle(59, 249, 260, 400);
	setcolor(WHITE);
	outtextxy(300, 270, _T("START"));
	outtextxy(305, 340, _T("BACK"));
	FlushBatchDraw();
}

void show_211()
{
	settextstyle(20, 0, _T("gbcbig.shx"));
	int flagnum = 1, formnum = 1;
	initplayerlist();
	firststep();
	drawonetab(flagnum, formnum);
	drawtwotab();
	drawthrtab(flagnum);
}

int updateWithInput_211()
{
	int flagnum = 1, formnum = 1;
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		mouse_x = m.x;
		mouse_y = m.y;
		if (m.x >= 60 && m.y >= 40 && m.x <= 260 && m.y <= 190) {
			setcolor(WHITE);
			rectangle(59, 39, 257, 190);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				record = -1;
				if (flagnum < 5) flagnum++;
				else flagnum = 1;
				newdraw_solo(flagnum, formnum);
			}
		}
		else if (m.x >= 52 && m.y >= 243 && m.x <= 264 && m.y <= 402) {
			setcolor(WHITE);
			rectangle(59, 249, 260, 400);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				record = -1;
				if (formnum < 3) formnum++;
				else formnum = 1;
				newdraw_solo(flagnum, formnum);
			}
		}
		else if (m.x >= 298 && m.y >= 270 && m.x <= 343 && m.y <= 285) 
		{
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(300, 270, _T("START"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				table = 2111;		//单人游戏开始接口 
				return 0;
			}
		}
		else if (m.x >= 298 && m.y >= 340 && m.x <= 343 && m.y <= 355) {
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(305, 340, _T("BACK"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				table = 21;													//返回接口
				return 0;
			}
		}
		else if (m.x >= 440 && m.x <= 490 && m.y >= 60 && m.y <= 300)
		{
			for (int num = flagnum * 10, i = 1;i < 13;i++) {
				if (list_block[formnum][i] != 1 && m.y >= i * 20 + 40 && m.y <= i * 20 + 60) 
				{
					setcolor(RED);
					outtextxy(440, i * 20 + 40, player_code[num]);
					FlushBatchDraw();
					if (m.uMsg == WM_LBUTTONDOWN)
					{
						setcolor(BLACK);
						outtextxy(440, i * 20 + 40, player_code[num]);
						FlushBatchDraw();
						if (record == -1) record = num;
						else
						{
							remakeplayerlist(flagnum, formnum, num);
							newdraw_playerlist(flagnum, formnum);
						}
					}
				}
				else drawonetab(flagnum, formnum);

				if (list_block[formnum][i] != 1) num++;
			}
		}
		else if (m.x >= 440 && m.x <= 490 && m.y >= 320 && m.y <= 420) 
		{
			for (int num = 60, i = 1;num <= 64;i++, num++) 
			{
				if (m.y >= i * 20 + 300 && m.y <= i * 20 + 320)
				{
					setcolor(RED);
					outtextxy(440, i * 20 + 300, player_code[num]);
					FlushBatchDraw();
					if (m.uMsg == WM_LBUTTONDOWN)
					{
						setcolor(BLACK);
						outtextxy(440, i * 20 + 300, player_code[num]);
						FlushBatchDraw();
						if (record == -1) record = num;
						else
						{
							remakeplayerlist(flagnum, formnum, num);
							newdraw_playerlist(flagnum, formnum);
						}
					}
				}
				else drawtwotab();
			}
		}
		else if (m.x >= 440 && m.x <= 490 && m.y >= 440 && m.y <= 460) {
			setcolor(RED);
			if (strcmp((const char*)player_code[flagnum], "0") != 0)
				outtextxy(440, 440, player_code[flagnum]);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				setcolor(BLACK);
				outtextxy(440, 440, player_code[flagnum]);
				FlushBatchDraw();
				if (record == -1)	record = flagnum;
				else {
					remakeplayerlist(flagnum, formnum, flagnum);
					newdraw_playerlist(flagnum, formnum);
				}
			}
		}
		else {
			if (m.uMsg == WM_LBUTTONDOWN) {
				record = -1;
			}
			drawonetab(flagnum, formnum);
			drawtwotab();
			drawthrtab(flagnum);
			setcolor(BLACK);
			rectangle(59, 39, 257, 190);
			rectangle(59, 249, 260, 400);
			setcolor(WHITE);
			outtextxy(300, 270, _T("START"));
			outtextxy(305, 340, _T("BACK"));
			FlushBatchDraw();
		}
	}
}

void ability_1(int able_x, char able, int x1, int y1, int x2, int y2)
{
	int i;
	for (i = 0;i < 10;i++)
	{
		if (able_x >= i * 13 + x1 && able_x < (i + 1) * 13 + x1)//依据能力条位置输出字符
			able = '0' + i;
	}

	setcolor(RED);
	setfillcolor(RED);
	bar(x1, y1, able_x, y2);

	settextstyle(25, 0, _T("黑体"));
	setcolor(BLACK);
	outtextxy(x2 + 15, y1, able);
}

void ability_2(int able_x, char able, int x1, int y1, int x2, int y2)
{
	int i;
	for (i = 0;i < 10;i++)
	{
		if (able_x >= i * 13 + x1 && able_x < (i + 1) * 13 + x1)//同上
			able = '0' + i;
	}

	setcolor(RED);
	setfillcolor(RED);
	bar(x1, y1, able_x, y2);

	settextstyle(25, 0, _T("黑体"));
	setcolor(BLACK);
	outtextxy(x1 - 25, y1, able);
}

void name_put(int isput)
{
	int i;
	char input;

	if (isput == 1)
	{
		if (_kbhit())
		{
			input = _getch();
			if (input != 8)
				name[name_position++] = input;//判断输入
			else
			{
				if (name_position > 0)
					name_position--;
			}
		}
	}
	else
		if (_kbhit())
			input = _getch();//为0灰条不输入，带走输入流的字符
	name[name_position] = '\0';
	for (i = 0;i < name_position;i++)
	{
		settextcolor(BLACK);
		settextstyle(18, 0, _T("黑体"));
		outtextxy(85 + i * 10, 60, name[i]);
	}
	FlushBatchDraw();
}

float ability_num(int able_x, int x1)
{
	int i;
	for (i = 0;i < 10;i++)
	{
		if (able_x >= i * 13 + x1 && able_x < (i + 1) * 13 + x1)//给结构体值赋值函数
			return (float)i;
	}
}
int tips_ifreplace()//提示框
{
	MOUSEMSG m1;
	while (1)
	{

		while (MouseHit())
		{
			m1 = GetMouseMsg();
			mouse_x = m1.x;
			mouse_y = m1.y;
			if (m1.uMsg == WM_LBUTTONDOWN && mouse_x >= 230 && mouse_x <= 300 && mouse_y >= 250 && mouse_y <= 280)
				return 1;
			if (m1.uMsg == WM_LBUTTONDOWN && mouse_x >= 340 && mouse_x <= 410 && mouse_y >= 250 && mouse_y <= 280)
				return 0;
			if (m1.uMsg == WM_LBUTTONDOWN && mouse_x >= 420 && mouse_x <= 440 && mouse_y >= 130 && mouse_y <= 150)
				return 0;
		}
		setcolor(RGB(79, 79, 79));
		setfillcolor(RGB(79, 79, 79));//底框
		bar(203, 133, 443, 293);

		setcolor(RGB(190, 190, 190));
		setfillcolor(RGB(190, 190, 190));//大框
		bar(200, 130, 440, 290);

		setcolor(RGB(245, 245, 245));
		setfillcolor(RGB(245, 245, 245));//顶白
		bar(200, 130, 440, 150);

		if (mouse_x >= 230 && mouse_x <= 300 && mouse_y >= 250 && mouse_y <= 280)
		{
			setcolor(WHITE);
			setfillcolor(WHITE);//是否框
			bar(230, 250, 300, 280);
		}
		else
		{
			setcolor(RGB(210, 210, 210));
			setfillcolor(RGB(210, 210, 210));//是否框
			bar(230, 250, 300, 280);
		}
		if (mouse_x >= 340 && mouse_x <= 410 && mouse_y >= 250 && mouse_y <= 280)
		{
			setcolor(WHITE);
			setfillcolor(WHITE);//是否框
			bar(340, 250, 410, 280);
		}
		else
		{
			setcolor(RGB(210, 210, 210));
			setfillcolor(RGB(210, 210, 210));//是否框
			bar(340, 250, 410, 280);
		}

		setcolor(BLACK);
		rectangle(230, 250, 300, 280);//是否边线
		rectangle(340, 250, 410, 280);

		settextstyle(25, 0, 0);
		outtextxy(253, 253, _T("是"));
		outtextxy(363, 253, _T("否"));

		settextstyle(15, 0, 0);
		outtextxy(217, 170, _T("此国家已有自创球员是否替换"));


		if (mouse_x >= 420 && mouse_x <= 440 && mouse_y >= 130 && mouse_y <= 150)
		{
			setcolor(RED);
			setfillcolor(RED);
			bar(420, 130, 440, 150);
			setlinecolor(WHITE);
			line(425, 135, 435, 145);
			line(425, 145, 435, 135);
		}
		else
		{
			setlinecolor(BLACK);
			line(425, 135, 435, 145);
			line(425, 145, 435, 135);
		}
		FlushBatchDraw();
	}
}

/*void account_put(int isput)
{
	static int i, j;
	char input;
	if (isput == 1)
	{
		if (_kbhit());
		if (input != 8)
			account[i] = input;
		else
		{
			if (i > 0)
			{
				i--;
			}
		}
	}
	else
		if (_kbhit())
			input = _getch();
	account[i + 1] = '\0';
	for (j = 0;j < i + 1;j++)
	{
		settextcolor(BLACK);
		settextstyle(18, 0, _T("黑体"));
		outtextxy(0, 0, account[i]);
	}
	FlushBatchDraw();
}

void code_put(int isput)
{
	static int i, j;
	char input;
	if (isput == 1)
	{
		if (_kbhit());
		if (input != 8)
			code[i] = input;
		else
		{
			if (i > 0)
			{
				i--;
			}
		}
	}
	else
		if (_kbhit())
			input = _getch();
	code[i + 1] = '\0';
	for (j = 0;j < i + 1;j++)
	{
		settextcolor(BLACK);
		settextstyle(18, 0, _T("黑体"));
		outtextxy(0, 0, account[i]);
	}
	FlushBatchDraw();
}*/

void file_do(int isput)
{
	int i;
	Users pe;
	FILE* fp;
	//FILE* fp_temp;
	fopen_s(&fp, "D:\\account.txt", "rt+");
	//fopen_s(&fp_temp, "D:\\accounttemp.txt", "wt+");
	 while (fscanf_s(fp, "%s %s", &pe.account, &pe.code) != NULL)
	{
		_tcscpy_s(pe.account, put_1);
		_tcscpy_s(pe.code, put_2);
		for (i = 0;i < put_position1;i++)
			fprintf_s(fp, "%c", pe.account[i]);
		fprintf_s(fp, "%c", _T(" "));
		for (i = 0;i < put_position2;i++)
			fprintf_s(fp, "%c", pe.code[i]);
	}
	fclose(fp);
	//fclose(fp_temp);
	//remove("D:\\account.txt");		//删除原先文档
	//rename("D:\\accounttemp.txt", "D:\\account.txt");		//改名现在文档
}