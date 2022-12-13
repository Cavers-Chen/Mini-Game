#include <conio.h>
#include <graphics.h>
#include <tchar.h>
#include <stdio.h>

#define Width 640
#define High 480

IMAGE russia,china,german,korea,britain,choosebk,formation433,formation343,formation334;    //国家和阵型图片
int table = 211;																			//界面入口变量211和22分别是单人和双人
int record = -1;																			//记录队员列表中上一次点击的队员的编号
int list_block[4][21] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, { 0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0 }, { 0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0 }, { 0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0} };//记录LIST表格中头名状况，方便跳过该单位格的输出
TCHAR player_code[65][20] = { _T("大傻"),_T("二愣12子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大62傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("大傻"),_T("二愣子"),_T("张三"),_T("李四"),_T("王老五"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九"),_T("尚小六"),_T("云比七"),_T("天大八"),_T("何九"),_T("何九") };				//测试的取名***这部分应该在initplayerlist函数部分被初始化***
int player_num[66] = {0};																	//0-65代表队员编号把每个数字当作两位数，十位当作国家名，各位是球员名，十位为6的是替补			

void choose_main_solo_quickrun();															//单人界面入口
void choose_main_double();																	//双人界面入口
void startup();																				//初始化文件
void newdraw_solo(int flagnum, int formnum);												//单人界面刷新绘制
void newdraw_double(int flagnum1, int formnum1, int flagnum2, int formnum2);				//双人界面刷新绘制
void newdraw_playerlist(int flagnum, int formnum);											//两个参数，分别更改队形和人员数据
void initplayerlist();																		//读文件（这部分函数内容我先注释掉了）****这里vs写法下和c写法不是很兼容，所以我写出来要么是编译过不了，要么是运行时崩溃，老哥可以稍微研究一下，解决不了问题应该也不是很大\（0.0）/****
void remakeplayerlist(int flagnum, int formnum, int num);									//更改队员编号顺序和姓名
void firststep();																			//初始单人界面
void drawonetab(int flagnum, int formnum);													//绘制国家队员名字
void drawtwotab();																			//绘制公用替补队员名字
void drawthrtab(int flagnum);																//绘制自创国家替补队员名字

int main() {
	startup();
	while (true) {
		switch (table) {
		case 211:choose_main_solo_quickrun();
			break;
		case 22:choose_main_double();
			break;
		default:
			break;
		}
	}
	return 0;
}

void startup() {																		//加载图片
	initgraph(Width,High);
	loadimage(&china, _T("D:\\china.jpg"));
	loadimage(&korea, _T("D:\\korea.jpg"));
	loadimage(&britain, _T("D:\\britain.jpg"));
	loadimage(&russia, _T("D:\\russia.jpg"));
	loadimage(&german, _T("D:\\german.jpg"));
	loadimage(&choosebk, _T("D:\\choosebk.gif"));
	loadimage(&formation433, _T("D:\\433.jpg"),200,150);
	loadimage(&formation343, _T("D:\\343.jpg"), 200, 150);
	loadimage(&formation334, _T("D:\\334.jpg"), 200, 150);
	BeginBatchDraw();
}

void firststep() {																		//初始单人界面
	int flagnum = 1, formnum = 1;
	putimage(0, 0, &choosebk);
	putimage(60, 40, &china);
	putimage(60, 250, &formation433);
	setbkmode(TRANSPARENT);
	outtextxy(60, 20, _T("COUNTRY"));
	outtextxy(60, 195, _T("CHINA"));
	outtextxy(60, 230, _T("FORMATION"));
	outtextxy(60, 405, _T("4-3-3"));
	outtextxy(0, 315, _T("     OUR"));
	outtextxy(0, 340, _T("     SIDE"));
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
	for (int num = flagnum * 10, i = 1;i < 13;i++) {
		if (list_block[formnum][i] == 1) continue;
		else if (num == record) {
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

void drawthrtab(int flagnum) {
	setcolor(WHITE);
	int num = flagnum;
	if (strcmp(player_code[num], "0") != 0&&num != record)
	outtextxy(440, 440, player_code[num]);
}

void initplayerlist() {
	FILE *fp;
	int n, i = 0;
	TCHAR name[20];
	fopen_s(&fp, "D://player.txt", "r");										//读的是副本文档,拷贝编号和名字			******可能会出问题，需要测试一下******
	/*while (!feof(fp)) {															//因为没有做结构体，所以读写格式需要更改一下
																				//这个demo所需要的是队员编号和名字，其他参数不重要
		fscanf_s(fp, _T("%d %s"), &n,name);										//这个地方会一直报错，网上查了一下，vs上的c写法有些不兼容，老哥可以想想办法试试看能不能改，不能问题也不是特别大
		player_num[i] = n;														//可以先把此部分while部分先注释掉，看效果
		strcpy_s(player_code[i], name);
		i++;
	}*/
	fclose(fp);
}

void remakeplayerlist(int flagnum, int formnum,int num) {						//交换队员名字的时候，对队员编号数组进行更改调换操作的函数
	TCHAR temps[20];
	int temp;
	strcpy_s(temps, player_code[num]);
	strcpy_s(player_code[num], player_code[record]);
	strcpy_s(player_code[record], temps);
	temp = player_num[num];
	player_num[num] = player_num[record];
	player_num[record] = temp;

	record = -1;

	drawonetab(flagnum, formnum);
	drawtwotab();
	drawthrtab(flagnum);
}

void choose_main_solo_quickrun() {												//单人界面绘制
	int flagnum = 1, formnum = 1;
	MOUSEMSG m;

	initplayerlist();
	firststep();
	drawonetab(flagnum, formnum);
	drawtwotab();
	drawthrtab(flagnum);

	while (true)
	{
		m = GetMouseMsg();

		if (m.x >= 60 && m.y >= 40 && m.x <= 260 && m.y <= 190) {
			setcolor(WHITE);
			rectangle(59, 39, 257, 190);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
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
			if (m.uMsg == WM_LBUTTONDOWN) {
				record = -1;
				if (formnum < 3) formnum++;
				else formnum = 1;
				newdraw_solo(flagnum, formnum);
			}
		}
		else if (m.x >= 298 && m.y >= 270 && m.x <= 343 && m.y <= 285) {
			setcolor(WHITE);

			setcolor(RED);
			outtextxy(300, 270, _T("START"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(1);													//单人游戏开始接口 
			}
		}
		else if (m.x >= 298 && m.y >= 340 && m.x <= 343 && m.y <= 355) {
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(305, 340, _T("BACK"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(1);													//返回接口
			}
		}
		else if (m.x >= 440&&m.x <= 490&&m.y>= 60&&m.y <= 300) {
			for (int num = flagnum * 10, i = 1;i < 13;i++) {
				if (list_block[formnum][i] != 1 && m.y >= i * 20 + 40 && m.y <= i * 20 + 60) {
					setcolor(RED);
					outtextxy(440, i * 20 + 40, player_code[num]);
					FlushBatchDraw();
					if (m.uMsg == WM_LBUTTONDOWN) {
						setcolor(BLACK);
						outtextxy(440, i * 20 + 40, player_code[num]);
						FlushBatchDraw();
						if (record == -1) record = num;
						else {
							remakeplayerlist(flagnum, formnum, num);
							newdraw_playerlist(flagnum, formnum);
						}
					}
				}
				else drawonetab(flagnum, formnum);

				if (list_block[formnum][i] != 1) num++;
			}
		}
		else if (m.x >= 440 && m.x <= 490 && m.y >= 320 && m.y <= 420) {
			for (int num = 60, i = 1;num <= 64;i++, num++) {
				if (m.y >= i * 20 + 300 && m.y <= i * 20 + 320) {
					setcolor(RED);
					outtextxy(440, i * 20 + 300, player_code[num]);
					FlushBatchDraw();
					if (m.uMsg == WM_LBUTTONDOWN) {
						setcolor(BLACK);
						outtextxy(440, i * 20 + 300, player_code[num]);
						FlushBatchDraw();
						if (record == -1) record = num;
						else {
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
			if (strcmp(player_code[flagnum], "0") != 0)
				outtextxy(440, 440, player_code[flagnum]);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				setcolor(BLACK);
				outtextxy(440, 440, player_code[flagnum]);
				FlushBatchDraw();
				if (record == -1)	record = flagnum;
				else {
					remakeplayerlist(flagnum, formnum, flagnum);
					newdraw_playerlist(flagnum,formnum);
				}
			}
		}
		else {
			if (m.uMsg == WM_LBUTTONDOWN) {
				record=-1;
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

void newdraw_playerlist(int flagnum, int formnum) {
	setcolor(RGB(0, 205, 0));
	setfillcolor(RGB(0, 139, 0));
	fillrectangle(370, 0, 640, 480);
	for (int i = 2;i * 20 < 480;i++) {
		rectangle(370, i * 20, 640, i * 20 + 20);
	}
	setcolor(WHITE);
	if (formnum == 1) {
		outtextxy(373, 15, _T("PLAYER LIST"));
		outtextxy(373, 40, _T("Striker Soccer"));
		outtextxy(373, 120, _T("Center Forward"));
		outtextxy(373, 200, _T("Football Back"));
		outtextxy(373, 300, _T("Reserve Player"));
		outtextxy(373, 420, _T("Special Player"));
	}
	else if (formnum == 2) {
		outtextxy(373, 15, _T("PLAYER LIST"));
		outtextxy(373, 40, _T("Striker Soccer"));
		outtextxy(373, 120, _T("Center Forward"));
		outtextxy(373, 220, _T("Football Back"));
		outtextxy(373, 300, _T("Reserve Player"));
		outtextxy(373, 420, _T("Special Player"));
	}
	else if (formnum == 3) {
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

void newdraw_solo(int flagnum, int formnum) {
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
		outtextxy(0, 315, _T("     OUR"));
		outtextxy(0, 340, _T("     SIDE"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (formnum == 2) {
		putimage(60, 250, &formation343);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-4-3"));
		outtextxy(0, 315, _T("     OUR"));
		outtextxy(0, 340, _T("     SIDE"));
		newdraw_playerlist(flagnum, formnum);
	}
	else if (formnum == 3) {
		putimage(60, 250, &formation334);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-3-4"));
		outtextxy(0, 315, _T("     OUR"));
		outtextxy(0, 340, _T("     SIDE"));
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

void choose_main_double() {
	putimage(0, 0, &choosebk);
	putimage(60, 40, &china);
	putimage(376, 40, &china);
	putimage(60, 250, &formation433);
	putimage(376, 250, &formation334);
	setbkmode(TRANSPARENT);
	outtextxy(60, 20, _T("COUNTRY"));
	outtextxy(60, 195, _T("CHINA"));
	outtextxy(60, 230, _T("FORMATION"));
	outtextxy(60, 405, _T("4-3-3"));
	outtextxy(376, 20, _T("COUNTRY"));
	outtextxy(376, 195, _T("CHINA"));
	outtextxy(376, 230, _T("FORMATION"));
	outtextxy(376, 405, _T("4-3-3"));
	outtextxy(0, 315, _T("     P1"));
	outtextxy(0, 340, _T("     SIDE"));
	outtextxy(570, 315, _T("     P2"));
	outtextxy(570, 340, _T("     SIDE"));
	outtextxy(300, 270, _T("START"));
	outtextxy(305, 340, _T("BACK"));
	setcolor(BLACK);
	rectangle(59, 39, 257, 190);
	rectangle(59, 249, 260, 400);
	rectangle(375, 39, 573, 190);
	rectangle(376, 250, 576, 399);

	FlushBatchDraw();								//防止开始时黑屏
	int flagnum1 = 1, formnum1 = 1;
	int flagnum2 = 1, formnum2 = 1;
	MOUSEMSG m;

	while (true)
	{
		m = GetMouseMsg();
		if (m.x >= 60 && m.y >= 40 && m.x <= 260 && m.y <= 190) {
			setcolor(WHITE);
			rectangle(59, 39, 257, 190);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (flagnum1 < 5) flagnum1++;
				else flagnum1 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (m.x >= 52 && m.y >= 243 && m.x <= 264 && m.y <= 402) {
			setcolor(WHITE);
			rectangle(59, 249, 260, 400);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (formnum1 < 3) formnum1++;
				else formnum1 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (m.x >= 376 && m.y >= 40 && m.x <= 588 && m.y <= 190) {
			setcolor(WHITE);
			rectangle(375, 39, 573, 190);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (flagnum2 < 5) flagnum2++;
				else flagnum2 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (m.x >= 52 && m.y >= 243 && m.x <= 264 && m.y <= 402) {
			setcolor(WHITE);
			rectangle(59, 249, 260, 400);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (formnum1 < 3) formnum1++;
				else formnum1 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (m.x >= 375 && m.y >= 247 && m.x <= 580 && m.y <= 400) {
			setcolor(WHITE);
			rectangle(376, 250, 576, 399);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (formnum2 < 3) formnum2++;
				else formnum2 = 1;
				newdraw_double(flagnum1, formnum1, flagnum2, formnum2);
			}
		}
		else if (m.x >= 298 && m.y >= 270 && m.x <= 343 && m.y <= 285) {
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(300, 270, _T("START"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(1);													//双人游戏开始接口 调用flagnum12 formnum12即可（如果P1 P2国家一样，continue即可）
			}
		}
		else if (m.x >= 298 && m.y >= 340 && m.x <= 343 && m.y <= 355) {
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(305, 340, _T("BACK"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(1);													//返回菜单接口
			}
		}
		else {
			setcolor(BLACK);
			rectangle(59, 39, 257, 190);
			rectangle(59, 249, 260, 400);
			rectangle(375, 39, 573, 190);
			rectangle(376, 250, 576, 399);
			//rectangle(298, 270, 343, 285);
			setcolor(WHITE);
			outtextxy(300, 270, _T("START"));
			outtextxy(305, 340, _T("BACK"));
			FlushBatchDraw();
		}
	}
}
																			//双人界面绘制
void newdraw_double(int flagnum1, int formnum1, int flagnum2, int formnum2) {
	MOUSEMSG m;
	m = GetMouseMsg();
	putimage(0, 0, &choosebk);

	if (flagnum1 == 1) {																//P1选择栏目
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
		outtextxy(0, 315, _T("     P1"));
		outtextxy(0, 340, _T("     SIDE"));
	}
	else if (formnum1 == 2) {
		putimage(60, 250, &formation343);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-4-3"));
		outtextxy(0, 315, _T("     P1"));
		outtextxy(0, 340, _T("     SIDE"));
	}
	else if (formnum1 == 3) {
		putimage(60, 250, &formation334);
		outtextxy(60, 230, _T("FORMATION"));
		outtextxy(60, 405, _T("3-3-4"));
		outtextxy(0, 315, _T("     P1"));
		outtextxy(0, 340, _T("     SIDE"));
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
		outtextxy(570, 315, _T("     P2"));
		outtextxy(570, 340, _T("     SIDE"));
	}
	else if (formnum2 == 2) {
		putimage(376, 250, &formation343);
		outtextxy(376, 230, _T("FORMATION"));
		outtextxy(376, 405, _T("3-4-3"));
		outtextxy(570, 315, _T("     P2"));
		outtextxy(570, 340, _T("     SIDE"));
	}
	else if (formnum2 == 3) {
		putimage(376, 250, &formation433);
		outtextxy(376, 230, _T("FORMATION"));
		outtextxy(376, 405, _T("3-3-4"));
		outtextxy(570, 315, _T("     P2"));
		outtextxy(570, 340, _T("     SIDE"));
	}
	setcolor(BLACK);
	rectangle(59, 39, 257, 190);
	rectangle(59, 249, 260, 400);
	rectangle(375, 39, 573, 190);
	rectangle(376, 250, 576, 399);
	setcolor(WHITE);
	outtextxy(300, 270, _T("START"));
	outtextxy(305, 340, _T("BACK"));

	FlushBatchDraw();
}