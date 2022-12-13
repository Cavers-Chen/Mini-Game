#include <conio.h>
#include <graphics.h>
#include <tchar.h>
#include <stdio.h>

#define Width 640
#define High 480

IMAGE russia,china,german,korea,britain,choosebk,formation433,formation343,formation334;    //���Һ�����ͼƬ
int table = 211;																			//������ڱ���211��22�ֱ��ǵ��˺�˫��
int record = -1;																			//��¼��Ա�б�����һ�ε���Ķ�Ա�ı��
int list_block[4][21] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, { 0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0 }, { 0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0 }, { 0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0} };//��¼LIST�����ͷ��״�������������õ�λ������
TCHAR player_code[65][20] = { _T("��ɵ"),_T("���12��"),_T("����"),_T("����"),_T("������"),_T("��С��"),_T("�Ʊ���"),_T("����"),_T("�ξ�"),_T("�ξ�"),_T("��62ɵ"),_T("�����"),_T("����"),_T("����"),_T("������"),_T("��С��"),_T("�Ʊ���"),_T("����"),_T("�ξ�"),_T("�ξ�"),_T("��ɵ"),_T("�����"),_T("����"),_T("����"),_T("������"),_T("��С��"),_T("�Ʊ���"),_T("����"),_T("�ξ�"),_T("�ξ�"),_T("��ɵ"),_T("�����"),_T("����"),_T("����"),_T("������"),_T("��С��"),_T("�Ʊ���"),_T("����"),_T("�ξ�"),_T("�ξ�"),_T("��ɵ"),_T("�����"),_T("����"),_T("����"),_T("������"),_T("��С��"),_T("�Ʊ���"),_T("����"),_T("�ξ�"),_T("�ξ�"),_T("��ɵ"),_T("�����"),_T("����"),_T("����"),_T("������"),_T("��С��"),_T("�Ʊ���"),_T("����"),_T("�ξ�"),_T("�ξ�"),_T("��С��"),_T("�Ʊ���"),_T("����"),_T("�ξ�"),_T("�ξ�") };				//���Ե�ȡ��***�ⲿ��Ӧ����initplayerlist�������ֱ���ʼ��***
int player_num[66] = {0};																	//0-65�����Ա��Ű�ÿ�����ֵ�����λ����ʮλ��������������λ����Ա����ʮλΪ6�����油			

void choose_main_solo_quickrun();															//���˽������
void choose_main_double();																	//˫�˽������
void startup();																				//��ʼ���ļ�
void newdraw_solo(int flagnum, int formnum);												//���˽���ˢ�»���
void newdraw_double(int flagnum1, int formnum1, int flagnum2, int formnum2);				//˫�˽���ˢ�»���
void newdraw_playerlist(int flagnum, int formnum);											//�����������ֱ���Ķ��κ���Ա����
void initplayerlist();																		//���ļ����ⲿ�ֺ�����������ע�͵��ˣ�****����vsд���º�cд�����Ǻܼ��ݣ�������д����Ҫô�Ǳ�������ˣ�Ҫô������ʱ�������ϸ������΢�о�һ�£������������Ӧ��Ҳ���Ǻܴ�\��0.0��/****
void remakeplayerlist(int flagnum, int formnum, int num);									//���Ķ�Ա���˳�������
void firststep();																			//��ʼ���˽���
void drawonetab(int flagnum, int formnum);													//���ƹ��Ҷ�Ա����
void drawtwotab();																			//���ƹ����油��Ա����
void drawthrtab(int flagnum);																//�����Դ������油��Ա����

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

void startup() {																		//����ͼƬ
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

void firststep() {																		//��ʼ���˽���
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
	FlushBatchDraw();																		//��ֹ��ʼʱ����
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
	fopen_s(&fp, "D://player.txt", "r");										//�����Ǹ����ĵ�,������ź�����			******���ܻ�����⣬��Ҫ����һ��******
	/*while (!feof(fp)) {															//��Ϊû�����ṹ�壬���Զ�д��ʽ��Ҫ����һ��
																				//���demo����Ҫ���Ƕ�Ա��ź����֣�������������Ҫ
		fscanf_s(fp, _T("%d %s"), &n,name);										//����ط���һֱ�������ϲ���һ�£�vs�ϵ�cд����Щ�����ݣ��ϸ��������취���Կ��ܲ��ܸģ���������Ҳ�����ر��
		player_num[i] = n;														//�����ȰѴ˲���while������ע�͵�����Ч��
		strcpy_s(player_code[i], name);
		i++;
	}*/
	fclose(fp);
}

void remakeplayerlist(int flagnum, int formnum,int num) {						//������Ա���ֵ�ʱ�򣬶Զ�Ա���������и��ĵ��������ĺ���
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

void choose_main_solo_quickrun() {												//���˽������
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
				exit(1);													//������Ϸ��ʼ�ӿ� 
			}
		}
		else if (m.x >= 298 && m.y >= 340 && m.x <= 343 && m.y <= 355) {
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(305, 340, _T("BACK"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(1);													//���ؽӿ�
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

	FlushBatchDraw();								//��ֹ��ʼʱ����
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
				exit(1);													//˫����Ϸ��ʼ�ӿ� ����flagnum12 formnum12���ɣ����P1 P2����һ����continue���ɣ�
			}
		}
		else if (m.x >= 298 && m.y >= 340 && m.x <= 343 && m.y <= 355) {
			setcolor(WHITE);
			setcolor(RED);
			outtextxy(305, 340, _T("BACK"));
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(1);													//���ز˵��ӿ�
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
																			//˫�˽������
void newdraw_double(int flagnum1, int formnum1, int flagnum2, int formnum2) {
	MOUSEMSG m;
	m = GetMouseMsg();
	putimage(0, 0, &choosebk);

	if (flagnum1 == 1) {																//P1ѡ����Ŀ
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

	if (flagnum2 == 1) {																//P2ѡ����Ŀ
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