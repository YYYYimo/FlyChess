// FlyChess.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "EasyX/Include/graphics.h"
#include "EasyX/Include/easyx.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
//#define DEBUG
using namespace std;
enum square_type { beginning, processing, ending };
enum plane_type { red, green, yellow, blue, white };
enum colour_type { Red, Green, Yellow, Blue };
struct node
{
	int x;
	int y;
};
vector <node> ve = {  //每个square对应的坐标位置
					  {709,634}, {641,635}, {640,700}, {708,700}, {140,701}, {140,632},
					  {76,700},  {76,635},  {75,134},  {139,134}, {75,69},   {142,69},
					  {641,68},  {643,136}, {705,69},  {705,137}, {753,544}, {229,741},
					  {29,221},  {553,24},  {649,532}, {607,533}, {561,513}, {524,549},
					  {542,597}, {543,640}, {522,686}, {477,702}, {434,702}, {392,700},
					  {349,701}, {308,703}, {259,683}, {241,638}, {239,598}, {259,548},
					  {223,513}, {179,534}, {136,531}, {90,514},  {73,468},  {74,426},
					  {74,383},  {74,343},  {74,299},  {89,256},  {137,237}, {179,236},
					  {223,253}, {261,219}, {241,169}, {242,131}, {261,85},  {305,70},
					  {349,67},  {392,66},  {435,69},  {478,68},  {522,85},  {541,130},
					  {542,172}, {524,214}, {561,256}, {605,236}, {649,237}, {694,256},
					  {710,301}, {710,343}, {710,384}, {709,428}, {709,468}, {694,514},
					  {649,384}, {607,385}, {563,386}, {521,383}, {478,386}, {429,385},
					  {392,641}, {392,599}, {391,556}, {391,514}, {393,471}, {391,422},
					  {135,385}, {178,384}, {219,386}, {262,385}, {307,383}, {356,386},
					  {390,129}, {392,172}, {390,215}, {392,256}, {392,299}, {391,347},
};
vector <node> center = {
							{724,649},{656,650},{655,715},{723,715},{155,716},{155,647},
							{91,715}, {91,650}, {90,149}, {154,149},{90,84},  {157,84},
							{656,83}, {658,151},{720,84}, {720,152},{768,559},{244,756},
							{44,236}, {568,39}, {664,547},{622,548},{576,528},{539,564},
							{557,612},{558,655},{537,701},{492,717},{449,717},{407,715},
							{364,716},{323,718},{274,698},{256,653},{254,613},{274,563},
							{238,528},{194,549},{151,546},{105,529},{88,483}, {89,441},
							{89,398}, {89,358}, {89,314}, {104,271},{152,252},{194,251},
							{238,268},{276,234},{256,184},{257,146},{276,100},{320,85},
							{364,82}, {407,81}, {450,84}, {493,83}, {537,100},{556,145},
							{557,187},{539,229},{576,271},{620,251},{664,252},{709,271},
							{725,316},{725,358},{725,399},{724,443},{724,483},{709,529},
							{664,399},{622,400},{578,401},{536,398},{493,401},{444,400},
							{407,656},{407,614},{406,571},{406,529},{408,486},{406,437},
							{150,400},{193,399},{234,401},{277,400},{322,398},{371,401},
							{405,144},{407,187},{405,230},{407,271},{407,314},{406,362}
};
struct square
{
	int colour;
	int cur_plane_type; // 0——红 1——绿 2——黄 3——蓝 4——没有飞机
	int cur_plane_num;
};
struct player
{
	int chess[58]; //chess[0]代表机场 chess[57]代表终点
	int match_square[58]; //储存chess对应square的序号
};
void initiate_squ(square* squares) //初始化squares
{
	for (int i = 0; i < 16; ++i) // 0-15格表示机场位置，按红绿黄蓝顺序初始化方格颜色
	{
		squares[i].cur_plane_type = i / 4;
		squares[i].colour = i / 4;
		squares[i].cur_plane_num = 1;
	}
	int count = 0;
	for (int i = 16; i < 20; ++i) //16-19格表示4个起点的位置
	{
		squares[i].cur_plane_type = white;
		squares[i].colour = count;
		squares[i].cur_plane_num = 0;
		count++;
	}
	for (int i = 20; i < 72; ++i) //20-71格表示过程中的位置，按红绿黄蓝的顺序初始化
	{
		squares[i].cur_plane_type = white;
		squares[i].colour = i % 4;
		squares[i].cur_plane_num = 0;
	}
	for (int i = 72; i < 96; ++i) //72-95格表示终点位置
	{
		squares[i].cur_plane_type = white;
		squares[i].colour = (i - 72) / 6;
		squares[i].cur_plane_num = 0;
	}
}
void initiate_pla(player* players) //玩家每个格子棋子情况的初始化
{
	for (int i = 0; i < 4; ++i)
		memset(players[i].chess, 0, sizeof(players[i].chess));
	for (int i = 0; i < 4; ++i)  //chess[0]代表机场的棋子数，全部初始化为4
	{
		players[i].chess[0] = 4;
		players[i].match_square[0] = 0;
		switch (i)
		{
		case 0:
		{
			players[0].match_square[1] = 16;
			players[0].match_square[2] = 71;
			int begin = 20, end = 72;
			for (int t = 3; t < 52; ++t)
			{
				players[0].match_square[t] = begin;
				begin++;
			}
			for (int k = 52; k < 58; ++k)
			{
				players[0].match_square[k] = end;
				end++;
			}
			break;
		}
		case 1:
		{
			players[1].match_square[1] = 17;
			int begin1 = 32, begin2 = 20, end = 78;
			for (int t = 2; t < 42; ++t)
			{
				players[1].match_square[t] = begin1;
				begin1++;
			}
			for (int k = 42; k < 52; ++k)
			{
				players[1].match_square[k] = begin2;
				begin2++;
			}
			for (int n = 52; n < 58; ++n)
			{
				players[1].match_square[n] = end;
				end++;
			}
			break;
		}
		case 2:
		{
			players[2].match_square[1] = 18;
			int begin1 = 45, begin2 = 20, end = 84;
			for (int t = 2; t < 29; ++t)
			{
				players[2].match_square[t] = begin1;
				begin1++;
			}
			for (int k = 29; k < 52; ++k)
			{
				players[2].match_square[k] = begin2;
				begin2++;
			}
			for (int n = 52; n < 58; ++n)
			{
				players[2].match_square[n] = end;
				end++;
			}
			break;
		}
		case 3:
		{
			players[3].match_square[1] = 19;
			int begin1 = 58, begin2 = 20, end = 90;
			for (int t = 2; t < 16; ++t)
			{
				players[3].match_square[t] = begin1;
				begin1++;
			}
			for (int k = 16; k < 52; ++k)
			{
				players[3].match_square[k] = begin2;
				begin2++;
			}
			for (int n = 52; n < 58; ++n)
			{
				players[3].match_square[n] = end;
				end++;
			}
			break;
		}
		}
	}
}
void load_plane_image(int colour, IMAGE* imgplane)
{
	switch (colour)
	{
	case 0: loadimage(imgplane, "./flychess_image/Red.png", 30, 30, true); break;
	case 1: loadimage(imgplane, "./flychess_image/Green.png", 30, 30, true); break;
	case 2: loadimage(imgplane, "./flychess_image/Yellow.png", 30, 30, true); break;
	case 3: loadimage(imgplane, "./flychess_image/Blue.png", 30, 30, true); break;
	}
}
IMAGE img;
void Myshow(square* squares, int die_point, int index)
{
	IMAGE imgbk;
	loadimage(&imgbk, "./flychess_image/bk.png");
	putimage(0, 0, &imgbk);
	TCHAR point = die_point + '0';
	//规则框
	setlinecolor(LIGHTRED);
	roundrect(803, 45, 997, 198, 40, 20);
	RECT r1 = { 804, 60, 1000, 200 }, r2 = { 804, 85, 1000, 200 }, r3 = { 804, 110, 1000, 200 }, r4 = { 804, 135, 1000, 200 };
	LPCTSTR line1 = "你是人类", line2 = "你属于红色阵营", line3 = "当你为本轮出棋方时", line4 = "请用鼠标选择你想移动的棋子";
	settextcolor(BLACK);
	settextstyle(20, 0, _T("Consolas"));
	drawtext(line1, &r1, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	drawtext(line2, &r2, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	drawtext(line3, &r3, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	drawtext(line4, &r4, DT_LEFT | DT_WORDBREAK | DT_VCENTER);

	setlinecolor(LIGHTBLUE);
	roundrect(803, 201, 997, 298, 40, 20);
	RECT r5 = { 804, 215, 1000, 298 }, r6 = { 804, 240, 1000, 298 };
	LPCTSTR line5 = "请按回车键开始", line6 = "或进入下一轮";
	drawtext(line5, &r5, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	drawtext(line6, &r6, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	setlinecolor(DARKGRAY);
	roundrect(803, 301, 997, 398, 40, 20);
	RECT r7 = { 804, 315, 960, 498 }, r8 = { 804, 340, 960, 498 };
	drawtext(_T("本轮出棋方 ："), &r7, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	switch (index)
	{
	case 0: drawtext(_T("红方"), &r7, DT_RIGHT | DT_WORDBREAK | DT_VCENTER); break;
	case 1: drawtext(_T("绿方"), &r7, DT_RIGHT | DT_WORDBREAK | DT_VCENTER); break;
	case 2: drawtext(_T("黄方"), &r7, DT_RIGHT | DT_WORDBREAK | DT_VCENTER); break;
	case 3: drawtext(_T("蓝方"), &r7, DT_RIGHT | DT_WORDBREAK | DT_VCENTER); break;
	}
	drawtext(_T("本轮点数 ："), &r8, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	drawtext(point, &r8, DT_RIGHT | DT_WORDBREAK | DT_VCENTER);
	for (int i = 0; i < 96; ++i)
	{
		int planenum = squares[i].cur_plane_num;
		int planetype = squares[i].cur_plane_type;
		IMAGE imgplane;
		if (planenum)
		{
			IMAGE imgplane;
			IMAGE imgmask;
			load_plane_image(planetype, &imgplane);
			loadimage(&imgmask, "./flychess_image/mask.jpg", 30, 30, true);
			putimage(ve[i].x, ve[i].y, &imgmask, SRCAND);
			putimage(ve[i].x, ve[i].y, &imgplane, SRCPAINT);
			if (planenum == 2)
			{
				settextcolor(BLACK);
				settextstyle(25, 0, "Consolas");
				outtextxy(ve[i].x, ve[i].y, '2');
			}
			else if (planenum == 3)
			{
				settextcolor(BLACK);
				settextstyle(25, 0, "Consolas");
				outtextxy(ve[i].x, ve[i].y, '3');
			}
			else if (planenum == 4)
			{
				settextcolor(BLACK);
				settextstyle(25, 0, "Consolas");
				outtextxy(ve[i].x, ve[i].y, '4');
			}
		}
	}
}
int winner = -1;
bool judge_end(player* players) //判断游戏是否结束
{
	for (int i = 0; i < 4; ++i)
	{
		if (players[i].chess[57] == 4)
		{
			winner = i;
			return true;
		}
	}
	return false;
}
int moveable_plane_square[4];
int moveable_plane_chess[4];
bool mobility(square* squares, player* players, int index, int die_point) //判断是否有可移动棋子，并得出可移动棋子对应的square编号
{
	for (int i = 0; i < 4; ++i)
		moveable_plane_square[i] = -1;
	for (int i = 0; i < 4; ++i)
		moveable_plane_chess[i] = -1;
	if (players[index].chess[1] == 1) //有棋子在起点的情况
	{
		moveable_plane_chess[0] = 1;
		moveable_plane_square[0] = 16 + index;
		return true;
	}
	if (players[index].chess[0] + players[index].chess[57] == 4 && die_point != 6)
		return false;
	else if (players[index].chess[0] == 4 && die_point == 6) //棋子全在机场的情况
	{
		moveable_plane_square[0] = 4 * index;
		moveable_plane_chess[0] = 0;
		return true;
	}
	else if (players[index].chess[0] > 0 && players[index].chess[0] < 4 && die_point == 6) //有棋子在机场的情况且摇到6
	{
		for (int i = index * 4; i < index * 4 + 4; ++i)
		{
			if (squares[i].cur_plane_num)
			{
				moveable_plane_square[0] = i;
				moveable_plane_chess[0] = 0;
				break;
			}
		}
		return true;
	}
	else if (players[index].chess[0] > 0 && players[index].chess[0] < 4 && die_point != 6)
	{
		for (int i = 2; i < 57; ++i)
		{
			if (players[index].chess[i])
			{
				for (int j = 0; j < 4; ++j)
				{
					if (moveable_plane_square[j] == -1)
					{
						moveable_plane_square[j] = players[index].match_square[i];
						moveable_plane_chess[j] = i;
						break;
					}
				}
			}
		}
	}
	else if (players[index].chess[0] == 0) //没有棋子在机场的情况
	{
		for (int i = 2; i < 57; ++i)
		{
			if (players[index].chess[i])
			{
				for (int j = 0; j < 4; ++j)
				{
					if (moveable_plane_square[j] == -1)
					{
						moveable_plane_square[j] = players[index].match_square[i];
						moveable_plane_chess[j] = i;
						break;
					}
				}
			}
		}
	}
}
bool ishit(square* squares, int index, int match_square)
{
	if (squares[match_square].cur_plane_num != 0 && squares[match_square].cur_plane_type != index)
		return true;
	else
		return false;
}
bool isjump(square* squares, int index, int match_square)
{
	if (squares[match_square].colour == index && match_square < 72)
		return true;
	else
		return false;
}
bool isfly(square* squares, int index, int match_square)
{
	switch (index)
	{
	case 0:
		if (match_square == 36)
			return true;
		break;
	case 1:
		if (match_square == 49)
			return true;
		break;
	case 2:
		if (match_square == 62)
			return true;
		break;
	case 3:
		if (match_square == 23)
			return true;
		break;
	}
	return false;
}
int best_choice(square* squares, int index, int match_square, int die_point, int match_chess) 
{
	if (match_chess + die_point == 57)
		return 4;
	int count = 0;
	if (match_chess + die_point < 52 && ishit(squares, index, match_square))
		count++;
	if (match_chess + die_point < 51 && isfly(squares, index, match_square))
		count++;
	if (match_chess + die_point < 51 && isjump(squares, index, match_square))
		count++;
	return count;
}
void move(square* squares, player* players, int index, int die_point)
{
	//判断是否有棋子能移动
	if (mobility(squares, players, index, die_point))
	{
		if (players[index].chess[1] == 0 && players[index].chess[0] > 0 && die_point == 6) //机场存在飞机且摇到6且起点处无飞机，需要起飞
		{
			players[index].chess[0]--;
			players[index].chess[1]++;
			squares[moveable_plane_square[0]].cur_plane_num = 0;
			squares[moveable_plane_square[0]].cur_plane_type = white;
			squares[index + 16].cur_plane_num = 1;
			squares[index + 16].cur_plane_type = index;
		}
		else //不需要起飞的情况
		{
			//电脑玩家判断走哪颗棋子
			int max = 0, selected_chess = moveable_plane_chess[0], selected_square = moveable_plane_square[0], rightclick = 0;
			if (index >= 1)
			{
				for (int i = 0; moveable_plane_square[i] != -1 && i < 4; ++i)
				{
					int match_square = players[index].match_square[moveable_plane_chess[i] + die_point];
					if (best_choice(squares, index, match_square, die_point, moveable_plane_chess[i]) > max)
					{
						max = best_choice(squares, index, match_square, die_point, moveable_plane_chess[i]);
						selected_square = moveable_plane_square[i];
						selected_chess = moveable_plane_chess[i];
					}
				}
			}
			else if (index == 0)
			{
				ExMessage m;
				while (!rightclick)  //鼠标点击选择正确的棋子
				{
					while (true)
					{
						m = getmessage(WM_LBUTTONDOWN);
						if (m.lbutton)
						{
							circle(m.x, m.y, 5);
							for (int i = 0; i < 4 && moveable_plane_square[i] != -1; ++i)
							{
								if ((m.x - center[moveable_plane_square[i]].x) * (m.x - center[moveable_plane_square[i]].x) +
									(m.y - center[moveable_plane_square[i]].y) * (m.y - center[moveable_plane_square[i]].y) <= 17 * 17)
								{
									selected_chess = moveable_plane_chess[i];
									selected_square = moveable_plane_square[i];
									rightclick = 1;
									break;
								}
							}
							break;
						}
					}
					if (rightclick == 0)
					{
						RECT rr = { 803, 410, 998, 610 };
						TCHAR s[] = "== 请重新选择棋子 ==";
						settextcolor(RED);
						settextstyle(20, 0, _T("Consolas"));
						setbkmode(TRANSPARENT);
						drawtext(s, &rr, DT_LEFT | DT_VCENTER);
						getchar();
						setfillcolor(WHITE);
						solidrectangle(803, 410, 998, 798);
					}

				}
			}
			players[index].chess[selected_chess]--;
			squares[selected_square].cur_plane_num--;
			if (squares[selected_square].cur_plane_num == 0)
				squares[selected_square].cur_plane_type = white;
			if (selected_chess + die_point > 57)
			{
				int land_point = 57 - (die_point - (57 - selected_chess));
				players[index].chess[land_point]++;
				squares[players[index].match_square[land_point]].cur_plane_num++;
				squares[players[index].match_square[land_point]].cur_plane_type = index;
			}
			else
			{
				if (selected_chess + die_point < 52 && ishit(squares, index, players[index].match_square[selected_chess + die_point])) //hit - fly/jump - hit
				{
					int hit_type = squares[players[index].match_square[selected_chess + die_point]].cur_plane_type;
					int hit_num = squares[players[index].match_square[selected_chess + die_point]].cur_plane_num;
					int hit_chess;
					for (int i = 2; i < 52; ++i)
					{
						if (players[hit_type].match_square[i] == players[index].match_square[selected_chess + die_point])
						{
							hit_chess = i;
							break;
						}
					}
					players[hit_type].chess[hit_chess] = 0;
					players[hit_type].chess[0] += hit_num;
					while (hit_num > 0)
					{
						for (int i = hit_type * 4; i < hit_type * 4 + 4; ++i)
						{
							if (squares[i].cur_plane_num == 0)
							{
								squares[i].cur_plane_num++;
								squares[i].cur_plane_type = hit_type;
								break;
							}
						}
						hit_num--;
					}
					players[index].chess[selected_chess + die_point]++;
					squares[players[index].match_square[selected_chess + die_point]].cur_plane_num = 1;
					squares[players[index].match_square[selected_chess + die_point]].cur_plane_type = index;
					if (selected_chess + die_point == 19)
					{
						players[index].chess[19]--;
						players[index].chess[31]++;
						squares[players[index].match_square[19]].cur_plane_num--;
						if (squares[players[index].match_square[19]].cur_plane_num == 0)
							squares[players[index].match_square[19]].cur_plane_type = white;
						if (ishit(squares, index, players[index].match_square[31])) // 飞完之后可能hit
						{
							int hit_type = squares[players[index].match_square[31]].cur_plane_type;
							int hit_num = squares[players[index].match_square[31]].cur_plane_num;
							int hit_chess;
							for (int i = 2; i < 57; ++i)
							{
								if (players[hit_type].match_square[i] == players[index].match_square[31])
								{
									hit_chess = i;
									break;
								}
							}
							players[hit_type].chess[hit_chess] = 0;
							players[hit_type].chess[0] += hit_num;
							squares[players[index].match_square[31]].cur_plane_num = 0;
							squares[players[index].match_square[31]].cur_plane_type = white;
							while (hit_num > 0)
							{
								for (int i = hit_type * 4; i < hit_type * 4 + 4; ++i)
								{
									if (squares[i].cur_plane_num == 0)
									{
										squares[i].cur_plane_num++;
										squares[i].cur_plane_type = hit_type;
										break;
									}
								}
								hit_num--;
							}
						}
						squares[players[index].match_square[31]].cur_plane_num++;
						squares[players[index].match_square[31]].cur_plane_type = index;
					}
					else if (isjump(squares, index, players[index].match_square[selected_chess + die_point])) // 跳完之后可能hit - fly之后可能hit
					{
						players[index].chess[selected_chess + die_point]--;
						players[index].chess[selected_chess + 4 + die_point]++;
						squares[players[index].match_square[selected_chess + die_point]].cur_plane_num--;
						if (squares[players[index].match_square[selected_chess + die_point]].cur_plane_num == 0)
							squares[players[index].match_square[selected_chess + die_point]].cur_plane_type = white;
						if (selected_chess + 4 + die_point < 52 && ishit(squares, index, players[index].match_square[selected_chess + 4 + die_point]))
						{
							int hit_type = squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_type;
							int hit_num = squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_num;
							int hit_chess;
							for (int i = 2; i < 52; ++i)
							{
								if (players[hit_type].match_square[i] == players[index].match_square[selected_chess + 4 + die_point])
								{
									hit_chess = i;
									break;
								}
							}
							players[hit_type].chess[hit_chess] = 0;
							players[hit_type].chess[0] += hit_num;
							squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_num = 0;
							squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_type = white;
							while (hit_num > 0)
							{
								for (int i = hit_type * 4; i < hit_type * 4 + 4; ++i)
								{
									if (squares[i].cur_plane_num == 0)
									{
										squares[i].cur_plane_num++;
										squares[i].cur_plane_type = hit_type;
										break;
									}
								}
								hit_num--;
							}
						}
						squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_num++;
						squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_type = index;
						if (selected_chess + 4 + die_point == 19 )
						{
							players[index].chess[19]--;
							players[index].chess[31]++;
							squares[players[index].match_square[19]].cur_plane_num--;
							squares[players[index].match_square[19]].cur_plane_type = white;
							if (ishit(squares, index, players[index].match_square[31])) // 飞完之后可能hit
							{
								int hit_type = squares[players[index].match_square[31]].cur_plane_type;
								int hit_num = squares[players[index].match_square[31]].cur_plane_num;
								int hit_chess;
								for (int i = 2; i < 57; ++i)
								{
									if (players[hit_type].match_square[i] == players[index].match_square[31])
									{
										hit_chess = i;
										break;
									}
								}
								players[hit_type].chess[hit_chess] = 0;
								players[hit_type].chess[0] += hit_num;
								squares[players[index].match_square[31]].cur_plane_num = 0;
								squares[players[index].match_square[31]].cur_plane_type = white;
								while (hit_num > 0)
								{
									for (int i = hit_type * 4; i < hit_type * 4 + 4; ++i)
									{
										if (squares[i].cur_plane_num == 0)
										{
											squares[i].cur_plane_num++;
											squares[i].cur_plane_type = hit_type;
											break;
										}
									}
									hit_num--;
								}
							}
							squares[players[index].match_square[31]].cur_plane_num++;
							squares[players[index].match_square[31]].cur_plane_type = index;
						}
					}
				}
				else if (selected_chess + die_point == 19) // fly - hit
				{
					players[index].chess[31]++;
					if (ishit(squares, index, players[index].match_square[31])) // 飞完之后可能hit
					{
						int hit_type = squares[players[index].match_square[31]].cur_plane_type;
						int hit_num = squares[players[index].match_square[31]].cur_plane_num;
						int hit_chess;
						for (int i = 2; i < 52; ++i)
						{
							if (players[hit_type].match_square[i] == players[index].match_square[31])
							{
								hit_chess = i;
								break;
							}
						}
						players[hit_type].chess[hit_chess] = 0;
						players[hit_type].chess[0] += hit_num;
						squares[players[index].match_square[31]].cur_plane_num = 0;
						squares[players[index].match_square[31]].cur_plane_type = white;
						while (hit_num > 0)
						{
							for (int i = hit_type * 4; i < hit_type * 4 + 4; ++i)
							{
								if (squares[i].cur_plane_num == 0)
								{
									squares[i].cur_plane_num++;
									squares[i].cur_plane_type = hit_type;
									break;
								}
							}
							hit_num--;
						}
					}
					squares[players[index].match_square[31]].cur_plane_num++;
					squares[players[index].match_square[31]].cur_plane_type = index;
				}
				else if (selected_chess + die_point < 51 && isjump(squares, index, players[index].match_square[selected_chess + 4 + die_point])) //jump - hit - fly - hit
				{
					players[index].chess[selected_chess + 4 + die_point]++;
					if (ishit(squares, index, players[index].match_square[selected_chess + 4 + die_point]))
					{
						int hit_type = squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_type;
						int hit_num = squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_num;
						int hit_chess;
						for (int i = 2; i < 52; ++i)
						{
							if (players[hit_type].match_square[i] == players[index].match_square[selected_chess + 4 + die_point])
							{
								hit_chess = i;
								break;
							}
						}
						players[hit_type].chess[hit_chess] = 0;
						players[hit_type].chess[0] += hit_num;
						squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_num = 0;
						squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_type = white;
						while (hit_num > 0)
						{
							for (int i = hit_type * 4; i < hit_type * 4 + 4; ++i)
							{
								if (squares[i].cur_plane_num == 0)
								{
									squares[i].cur_plane_num++;
									squares[i].cur_plane_type = hit_type;
									break;
								}
							}
							hit_num--;
						}
					}
					squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_num++;
					squares[players[index].match_square[selected_chess + 4 + die_point]].cur_plane_type = index;
					if (selected_chess + 4 + die_point == 19)
					{
						players[index].chess[19]--;
						players[index].chess[31]++;
						squares[players[index].match_square[19]].cur_plane_num--;
						squares[players[index].match_square[19]].cur_plane_type = white;
						if (ishit(squares, index, players[index].match_square[31])) // 飞完之后可能hit
						{
							int hit_type = squares[players[index].match_square[31]].cur_plane_type;
							int hit_num = squares[players[index].match_square[31]].cur_plane_num;
							int hit_chess;
							for (int i = 2; i < 52; ++i)
							{
								if (players[hit_type].match_square[i] == players[index].match_square[31])
								{
									hit_chess = i;
									break;
								}
							}
							players[hit_type].chess[hit_chess] = 0;
							players[hit_type].chess[0] += hit_num;
							squares[players[index].match_square[31]].cur_plane_num = 0;
							squares[players[index].match_square[31]].cur_plane_type = white;
							while (hit_num > 0)
							{
								for (int i = hit_type * 4; i < hit_type * 4 + 4; ++i)
								{
									if (squares[i].cur_plane_num == 0)
									{
										squares[i].cur_plane_num++;
										squares[i].cur_plane_type = hit_type;
										break;
									}
								}
								hit_num--;
							}
						}
						squares[players[index].match_square[31]].cur_plane_num++;
						squares[players[index].match_square[31]].cur_plane_type = index;
					}
				}
				else 
				{
					players[index].chess[selected_chess + die_point]++;
					squares[players[index].match_square[selected_chess + die_point]].cur_plane_num++;
					squares[players[index].match_square[selected_chess + die_point]].cur_plane_type = index;
				}
			}
		}
	}
}

int main()
{
	//数据初始化
	square squares[97];
	initiate_squ(squares);
	player players[4];
	initiate_pla(players);

	//棋盘背景初始化
	initgraph(1000, 800);
	setbkcolor(WHITE);
	cleardevice();
	int player_index = 0, die_point;
	srand(time(NULL));
	rand();

	//游戏过程
	while (!judge_end(players))
	{
		die_point = (rand() % 6) + 1;
		Myshow(squares, die_point, player_index);
	    getchar();
		move(squares, players, player_index, die_point);
		if(die_point != 6)
			player_index = (player_index + 1) % 4;
		cleardevice();
	}

	//结束界面
	RECT r = { 0, 0, 800, 800 };
	settextstyle(40, 0, _T("Consolas"));
	if (winner == 0)
	{
		settextcolor(RED);
		drawtext(_T("YOU WIN!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		getchar();
	}
	else
	{
		settextcolor(BLACK);
		drawtext(_T("YOU LOSE~"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		getchar();
	}
	closegraph();
	return 0;
}

