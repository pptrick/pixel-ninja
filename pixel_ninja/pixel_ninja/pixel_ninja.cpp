#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<string.h>
#include<easyx.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mmsystem.h>
#pragma comment(lib, "Winmm.lib")


using namespace std;
IMAGE background;
IMAGE hero_rc;
IMAGE enemy_rc;

IMAGE hero_walk[8][2];
IMAGE hero_jump[14][2];
IMAGE hero_kick[14][2];
IMAGE hero_stable[2][2];
IMAGE hero_hurt[2][2];
IMAGE hero_die[8][2];

IMAGE bullet_pic[2][2];
IMAGE bullet_shoot[2][2];
IMAGE knife[2][2];

IMAGE enemy_walk[8][2];
IMAGE enemy_stable[2][2];
IMAGE enemy_attack[2][2];
IMAGE enemy_throw[4][2];
IMAGE enemy_hurt[2][2];
IMAGE enemy_die[6][2];

IMAGE bottle;
IMAGE shoes;
IMAGE dart;

#define velocity 40;
#define G 10;
#define length 600;
#define width 480;


class hero
{
private:
	int hero_x;
	int hero_y;
	bool direction;//direction为1表示向左，为0表示向右；
	bool trigger;//跳动触发
	bool wtrigger;//走动触发
	int walk_state;//state存储动作数
	int walk_num;//num存储数组编号
	int jump_state;
	int jump_num;
	int kick_state;
	int kick_num;
	int shoot_state;
	int stable_state;
	int fullHp;
	int fullEp;
public:
	int dart_num;
	int fulldart;
	int Hp;
	int Ep;
	int hurt_state;
	int score;
	int money;
	int steplong;
	friend class bullet;//将子弹类视为朋友
	friend class enemy;//将敌人类视为朋友
	hero(int x, int y, int z, int w)
	{
		hero_x = x; hero_y = y; walk_state = z; jump_state = w;
		steplong = 2;
		score = 0;
		money = 0;
		direction = 0; kick_state = 0; shoot_state = 0; hurt_state = 0;
		Hp = 100; Ep = 0; dart_num = 20;
		fullEp = 100; fullHp = 100; fulldart = 20;
	};
	void get_walkstate(bool left, bool right, bool up, bool down, bool jump);
	void get_jumpstate(bool left, bool right, bool up, bool down, bool jump);
	void get_kickstate(bool kick);
	void hero_walking(bool left, bool right, bool up, bool down, bool jump);
	void hero_jumping(bool left, bool right, bool up, bool down, bool jump);
	void hero_kicking(bool kick);
	void hero_shoot();
	void hero_draw();
	void draw_herostate();
	void hero_death(int score);
};

class enemy
{
private:
	int enemy_x;
	int enemy_y;
	bool e_direction;
	int enemy_walknum;
	int enemy_thrownum;
	int enemy_state;
	float buffer;
	int lie_stable;//躺尸指数
				   //以下是刀的相关变量
	int knife_x;
	int knife_y;
	bool knife_show;
	bool k_direction;
	int button;
public:
	enemy()
	{
		srand((unsigned)time(0));
		enemy_x = -1;
		enemy_y = -1;
		x = 50;
		y = 50;
		e_direction = 0;
		enemy_Hp = 100;
		enemy_walknum = 0;
		enemy_deathnum = 0;
		enemy_thrownum = 0;
		enemy_state = 0;
		enemy_show = 0;
		lie_stable = 0;
		button = 0;
		//以下是“刀”的初始化
		knife_x = 0;
		knife_y = 0;
		knife_show = 0;
		k_direction = 0;
	}
	int x;
	int y;
	int enemy_Hp;
	int enemy_deathnum;
	bool enemy_show;
	void show_enemy();
	void enemy_walking(hero b, int j);
	bool enemy_hurting1(bullet a);
	bool enemy_hurting2(hero b);
	void draw_enemystate();
	bool enemy_death();
	bool enemy_attacking(hero b);
	bool enemy_throwing(hero& b);
	bool compare(hero b);
	void knife_same();
	void enemy_restart(int i);
};

class bullet
{
public:
	int bullet_x;
	int bullet_y;
	bool headto;
	bullet() { bullet_x = -50; bullet_y = -50; show = 0; }
	bool show;
	void bullet_draw();
	void same(hero h);
};

void bullet::bullet_draw()
{
	if (headto == 0)
	{
		bullet_x = bullet_x + 30;
		putimage(bullet_x, bullet_y, &bullet_pic[1][1], SRCAND);
		putimage(bullet_x, bullet_y, &bullet_pic[1][0], SRCINVERT);
	}
	else
	{
		bullet_x = bullet_x - 30;
		putimage(bullet_x, bullet_y, &bullet_pic[0][1], SRCAND);
		putimage(bullet_x, bullet_y, &bullet_pic[0][0], SRCINVERT);
	}
}

void bullet::same(hero h)
{
	bullet_x = h.hero_x;
	bullet_y = h.hero_y;
	headto = h.direction;
}

void initialize_graph()
{
	int l = length;
	int w = width;
	loadimage(&background, _T("D:\\res\\background.jpg"), l, w);
	putimage(0, 0, &background);
	setbkmode(TRANSPARENT);
	//完成背景初始化

	//以下是英雄初始化阶段
	loadimage(&hero_rc, _T("D:\\res\\hero.jpg"));

	SetWorkingImage(&hero_rc);
	for (int i = 0; i < 4; i++)
	{
		getimage(&hero_walk[i][0], 40 * i, 0, 40, 40);
		getimage(&hero_walk[i][1], 40 * i, 40, 40, 40);
		getimage(&hero_walk[i + 4][0], 40 * i, 80, 40, 40);
		getimage(&hero_walk[i + 4][1], 40 * i, 120, 40, 40);
	}
	//完成英雄步行初始化；前四个为右行，后四个为左行
	for (int i = 0; i < 7; i++)
	{
		getimage(&hero_jump[i][0], 40 * i, 880, 40, 40);
		getimage(&hero_jump[i][1], 40 * i, 920, 40, 40);
		getimage(&hero_jump[i + 7][0], 40 * i, 960, 40, 40);
		getimage(&hero_jump[i + 7][1], 40 * i, 1000, 40, 40);
	}
	//完成英雄跳跃初始化；前七个为右行，后七个为左行
	getimage(&hero_stable[0][0], 80, 480, 40, 40);
	getimage(&hero_stable[0][1], 80, 520, 40, 40);
	getimage(&hero_stable[1][0], 80, 560, 40, 40);
	getimage(&hero_stable[1][1], 80, 600, 40, 40);
	//完成英雄静止状态初始化，0为向右，1为向左
	for (int i = 0; i < 7; i++)
	{
		getimage(&hero_kick[i][0], 40 * i, 1040, 40, 40);
		getimage(&hero_kick[i][1], 40 * i, 1080, 40, 40);
		getimage(&hero_kick[i + 7][0], 40 * i, 1120, 40, 40);
		getimage(&hero_kick[i + 7][1], 40 * i, 1160, 40, 40);
	}
	//完成英雄“踢”技能初始化
	getimage(&hero_hurt[0][0], 40, 1280, 40, 40);
	getimage(&hero_hurt[0][1], 40, 1320, 40, 40);
	getimage(&hero_hurt[1][0], 40, 1200, 40, 40);
	getimage(&hero_hurt[1][1], 40, 1240, 40, 40);
	//完成英雄“受伤”初始化
	for (int i = 0; i < 4; i++)
	{
		getimage(&hero_die[i][0], 40 + 40 * i, 1200, 40, 40);
		getimage(&hero_die[i][1], 40 + 40 * i, 1240, 40, 40);
		getimage(&hero_die[i + 4][0], 40 + 40 * i, 1280, 40, 40);
		getimage(&hero_die[i + 4][1], 40 + 40 * i, 1320, 40, 40);
	}
	//完成英雄“死亡“初始化
	getimage(&bullet_pic[0][0], 200, 320, 40, 40);
	getimage(&bullet_pic[0][1], 200, 360, 40, 40);
	getimage(&bullet_pic[1][0], 200, 400, 40, 40);
	getimage(&bullet_pic[1][1], 200, 440, 40, 40);
	getimage(&bullet_shoot[0][0], 160, 320, 40, 40);
	getimage(&bullet_shoot[0][1], 160, 360, 40, 40);
	getimage(&bullet_shoot[1][0], 160, 400, 40, 40);
	getimage(&bullet_shoot[1][1], 160, 440, 40, 40);
	//完成子弹图像初始化
	SetWorkingImage(NULL);

	//以下是敌人初始化阶段
	loadimage(&enemy_rc, _T("D:\\res\\enemy.jpg"));
	SetWorkingImage(&enemy_rc);
	for (int i = 0; i < 4; i++)
	{
		getimage(&enemy_walk[i][0], 40 * i, 0, 40, 40);
		getimage(&enemy_walk[i][1], 40 * i, 40, 40, 40);
		getimage(&enemy_walk[i + 4][0], 40 * i, 80, 40, 40);
		getimage(&enemy_walk[i + 4][1], 40 * i, 120, 40, 40);
	}
	//完成敌人步行初始化；前四个为右行，后四个为左行
	getimage(&enemy_stable[0][0], 160, 0, 40, 40);
	getimage(&enemy_stable[0][1], 160, 40, 40, 40);
	getimage(&enemy_stable[1][0], 160, 80, 40, 40);
	getimage(&enemy_stable[1][1], 160, 120, 40, 40);
	//完成敌人静止状态初始化；前四个向右，后四个向左
	getimage(&enemy_attack[0][0], 280, 160, 40, 40);
	getimage(&enemy_attack[0][1], 280, 200, 40, 40);
	getimage(&enemy_attack[1][0], 280, 240, 40, 40);
	getimage(&enemy_attack[1][1], 280, 280, 40, 40);
	//完成敌人攻击状态初始化；
	getimage(&enemy_throw[0][0], 280, 160, 40, 40);
	getimage(&enemy_throw[0][1], 280, 200, 40, 40);
	getimage(&enemy_throw[1][0], 320, 160, 40, 40);
	getimage(&enemy_throw[1][1], 320, 200, 40, 40);
	getimage(&enemy_throw[2][0], 280, 240, 40, 40);
	getimage(&enemy_throw[2][1], 280, 280, 40, 40);
	getimage(&enemy_throw[3][0], 320, 240, 40, 40);
	getimage(&enemy_throw[3][1], 320, 280, 40, 40);
	//完成敌人投掷状态初始化；
	getimage(&enemy_hurt[1][0], 0, 320, 40, 40);
	getimage(&enemy_hurt[1][1], 0, 360, 40, 40);
	getimage(&enemy_hurt[0][0], 0, 400, 40, 40);
	getimage(&enemy_hurt[0][1], 0, 440, 40, 40);
	//完成敌人受伤初始化；
	for (int i = 0; i < 3; i++)
	{
		getimage(&enemy_die[i][0], 40 * (i + 1), 400, 40, 40);
		getimage(&enemy_die[i][1], 40 * (i + 1), 440, 40, 40);
		getimage(&enemy_die[i + 3][0], 40 * (i + 1), 320, 40, 40);
		getimage(&enemy_die[i + 3][1], 40 * (i + 1), 360, 40, 40);
	}
	//完成敌人死亡初始化
	getimage(&knife[0][0], 360, 160, 40, 40);
	getimage(&knife[0][1], 360, 200, 40, 40);
	getimage(&knife[1][0], 360, 240, 40, 40);
	getimage(&knife[1][1], 360, 280, 40, 40);

	loadimage(&bottle, _T("D:\\res\\bottle.jpg"));
	loadimage(&shoes, _T("D:\\res\\shoes.jpg"));
	loadimage(&dart, _T("D:\\res\\dart.jpg"));

	SetWorkingImage(NULL);

}

void hero::get_walkstate(bool left, bool right, bool up, bool down, bool jump)
{
	if (left == 0 && right == 0 && up == 0 && down == 0)
	{
		walk_state = 0;
		stable_state = 1;
		wtrigger = 0;
	}//无左右键输入，设置为静止
	else if (left == 0 && right == 0 && up != 0)
	{
		stable_state = 0;

		if (walk_state == 0)//若不为零，就不更改状态
		{
			walk_state = 4;
			wtrigger = 1;
		}
	}
	else if (left == 0 && right == 0 && down != 0)
	{
		stable_state = 0;

		if (walk_state == 0)//若不为零，就不更改状态
		{
			walk_state = 4;
			wtrigger = 1;
		}
	}
	else if (left == 1 && right == 0)
	{
		stable_state = 0;//设置为运动

		if (walk_state == 0)//若不为零，就不更改状态
		{
			walk_state = 4;
			wtrigger = 0;
		}
		direction = 1;
	}
	else if (left == 0 && right == 1)
	{
		stable_state = 0;

		if (walk_state == 0)
		{
			walk_state = 4;
			wtrigger = 0;
		}
		direction = 0;
	}
}

void hero::hero_walking(bool left, bool right, bool up, bool down, bool jump)
{
	int step = steplong;//设置步长

	get_walkstate(left, right, up, down, jump);//判断行走状态（处于哪个动作）

	if (wtrigger == 1)
	{

		if (walk_state != 0)
		{
			if (direction == 1)//判断为“左”
			{
				walk_num = 8 - walk_state;//设置图片序号

				if (up != 0 && down == 0)
				{
					hero_y = hero_y - 4 * step;
				}
				else if (down != 0 && up == 0)
				{
					hero_y = hero_y + 4 * step;
				}
				if (left != 0)
				{
					direction = 1;
					hero_x = hero_x - 4 * step;
				}
				else if (right != 0)
				{
					direction = 0;
					hero_x = hero_x + 4 * step;
				}
				walk_state--;//动作完成，更新行走状态
			}
			else if (direction == 0)//判断为“右”
			{
				walk_num = 4 - walk_state;
				if (up != 0 && down == 0)
				{
					hero_y = hero_y - 4 * step;
				}
				else if (down != 0 && up == 0)
				{
					hero_y = hero_y + 4 * step;
				}
				if (left != 0)
				{
					direction = 1;
					hero_x = hero_x - 4 * step;
				}
				else if (right != 0)
				{
					direction = 0;
					hero_x = hero_x + 4 * step;
				}
				walk_state--;
			}
		}

	}
	else if (wtrigger == 0)
	{
		if (walk_state != 0)
		{
			if (direction == 1)//判断为“左”
			{
				hero_x = hero_x - 4 * step;//移动英雄
				walk_num = 8 - walk_state;//设置图片序号

				if (up != 0 && down == 0)
				{
					hero_y = hero_y - 4 * step;
				}
				else if (down != 0 && up == 0)
				{
					hero_y = hero_y + 4 * step;
				}
				walk_state--;//动作完成，更新行走状态
			}
			else if (direction == 0)//判断为“右”
			{
				hero_x = hero_x + 4 * step;
				walk_num = 4 - walk_state;
				if (up != 0 && down == 0)
				{
					hero_y = hero_y - 4 * step;
				}
				else if (down != 0 && up == 0)
				{
					hero_y = hero_y + 4 * step;
				}
				walk_state--;
			}
		}
	}



}

void hero::get_jumpstate(bool left, bool right, bool up, bool down, bool jump)
{
	if (jump == 0 && jump_state == 0)
	{
		jump_state = 0;
		trigger = 0;
	}
	else
	{
		if (left == 0 && right == 0 && jump_state == 0)
		{
			jump_state = 7;
			trigger = 0;
		}
		else if (left == 1 && right == 0 && jump_state == 0)
		{
			jump_state = 7;
			direction = 1;
			trigger = 1;
		}
		else if (left == 0 && right == 1 && jump_state == 0)
		{
			jump_state = 7;
			direction = 0;
			trigger = 1;
		}
	}
}

void hero::hero_jumping(bool left, bool right, bool up, bool down, bool jump)
{
	int step = steplong;
	int t;
	int v0 = velocity;
	int g = G;
	get_jumpstate(left, right, up, down, jump);

	if (trigger == 0 && jump_state != 0)
	{
		t = 8 - jump_state;
		hero_y = hero_y - (v0 - g * t);
		if (direction == 1) { jump_num = 14 - jump_state; }
		else { jump_num = 7 - jump_state; }
		jump_state--;
	}
	else if (trigger == 1 && jump_state != 0)
	{
		t = 8 - jump_state;
		hero_y = hero_y - (v0 - g * t);
		if (direction == 1) { jump_num = 14 - jump_state; hero_x = hero_x - 4 * step; }
		else { jump_num = 7 - jump_state; hero_x = hero_x + 4 * step; }
		jump_state--;
	}


}

void hero::get_kickstate(bool kick)
{
	if (kick == 0 && kick_state == 0)
	{
		kick_state = 0;
	}
	else
	{
		if (kick != 0 && kick_state == 0 && Ep > 20)
		{
			kick_state = 7;
			Ep = Ep - 20;
		}
	}
}

void hero::hero_kicking(bool kick)
{
	get_kickstate(kick);
	if (kick_state != 0)
	{
		if (direction == 1) { kick_num = 14 - kick_state; hero_x = hero_x - 2 * steplong; }

		else if (direction == 0) { kick_num = 7 - kick_state; hero_x = hero_x + 2 * steplong; }
		kick_state--;
	}
}

void hero::hero_draw()
{
	if (stable_state == 1 && jump_state == 0 && kick_state == 0 && shoot_state == 0 && hurt_state == 0)
	{
		putimage(hero_x, hero_y, &hero_stable[direction][1], SRCAND);
		putimage(hero_x, hero_y, &hero_stable[direction][0], SRCINVERT);
	}
	else if (hurt_state != 0)
	{
		putimage(hero_x, hero_y, &hero_hurt[direction][1], SRCAND);
		putimage(hero_x, hero_y, &hero_hurt[direction][0], SRCINVERT);
		hurt_state = 0;
	}
	else if (shoot_state != 0)
	{
		putimage(hero_x, hero_y, &bullet_shoot[direction][1], SRCAND);
		putimage(hero_x, hero_y, &bullet_shoot[direction][0], SRCINVERT);
		shoot_state = 0;
	}
	else if (jump_state != 0)
	{
		putimage(hero_x, hero_y, &hero_jump[jump_num][1], SRCAND);
		putimage(hero_x, hero_y, &hero_jump[jump_num][0], SRCINVERT);
	}
	else if (kick_state != 0)
	{
		putimage(hero_x, hero_y, &hero_kick[kick_num][1], SRCAND);
		putimage(hero_x, hero_y, &hero_kick[kick_num][0], SRCINVERT);
	}
	else
	{
		putimage(hero_x, hero_y, &hero_walk[walk_num][1], SRCAND);
		putimage(hero_x, hero_y, &hero_walk[walk_num][0], SRCINVERT);
	}

	if (Ep<100)Ep++;
}

void hero::hero_shoot()
{
	shoot_state = 1;
	dart_num--;
}

void hero::draw_herostate()
{
	settextstyle(16, 0, _T("Showcard Gothic"));
	//settextstyle(16, 0, _T("测试"));
	TCHAR HPtext[20], EPtext[20], darttext[20], score_num[20], money_num[20];

	_stprintf_s(HPtext, _T("HP     %d/%d"), Hp, fullHp);
	_stprintf_s(EPtext, _T("EP     %d/%d"), Ep, fullEp);
	_stprintf_s(darttext, _T("         %d/%d"), dart_num, fulldart);
	_stprintf_s(score_num, _T("score:  %d"), score);
	_stprintf_s(money_num, _T("money:  %d"), money);

	rectangle(20, 1, 122, 20);
	rectangle(20, 22, 122, 42);
	rectangle(20, 44, 122, 64);

	setfillcolor(RED);
	solidrectangle(21, 2, 21 + Hp, 19);
	setfillcolor(BLUE);
	solidrectangle(21, 23, 21 + Ep, 41);
	setfillcolor(BLACK);
	solidrectangle(21, 45, 21 + 5 * dart_num, 63);
	outtextxy(0, 3, HPtext);
	outtextxy(0, 25, EPtext);
	outtextxy(5, 47, darttext);
	putimage(-10, 30, &bullet_pic[1][1], SRCAND);
	putimage(-10, 30, &bullet_pic[1][0], SRCINVERT);
	outtextxy(3, 70, score_num);
	outtextxy(90, 70, money_num);
}

bool enemy::compare(hero b)
{
	if (b.hero_y > enemy_y)return true;
	else return false;
}

void enemy::knife_same()
{
	knife_x = enemy_x;
	knife_y = enemy_y;
	knife_show = enemy_show;
	k_direction = e_direction;
}

bool enemy::enemy_throwing(hero& b)
{
	int l = length;
	srand(time(0));
	int num = rand() % 7;
	//int num = 1;
	if (num == 1 && abs(enemy_y - b.hero_y) < 15 && knife_show == 0 && abs(enemy_x - b.hero_x) > 80)
	{
		if (b.hero_x > enemy_x)e_direction = 0;
		else e_direction = 1;
		knife_same();
		enemy_state = 5;
		enemy_thrownum = 2;
		button = 1;
	}
	else if (knife_show == 1 && knife_x >= 0 && knife_x <= l)
	{
		if (k_direction == 0)
		{
			knife_x = knife_x + 20;
		}
		else knife_x = knife_x - 20;
		putimage(knife_x, knife_y, &knife[k_direction][1], SRCAND);
		putimage(knife_x, knife_y, &knife[k_direction][0], SRCINVERT);
	}
	else
	{
		enemy::knife_show = 0;
		knife_x = -1;
		knife_y = -1;
		if (button == 1)
		{
			b.score = b.score + 5;
			button = 0;
		}
	}
	if (abs(knife_x - b.hero_x) < 10 && abs(knife_y - b.hero_y + 5) < 10 && b.jump_state == 0)
	{
		b.score = b.score - 5;
		knife_show = 0;
		knife_x = 0;
		knife_y = 0;
		return true;
	}
	else return false;
}

void enemy::enemy_walking(hero b, int j)
{
	if (enemy_state == 2)
	{
		x = enemy_x;
		y = enemy_y;
	}
	if (lie_stable == 0 && enemy_thrownum == 0)
	{
		if (abs(x - enemy_x) < 10 && abs(y - enemy_y) < 10)
		{
			srand((j + 1)*time(0));
			x = (rand() % 10) * 50 + 50;
			y = (rand() % 10) * 42 + 40;
			enemy_walknum = 0;
			enemy_state = 0;
		}
		else if (abs(b.hero_x - enemy_x) < 100 && abs(b.hero_y - enemy_y) < 100 && enemy_walknum == 1)
		{
			x = max(abs(b.hero_x), 1);
			y = max(abs(b.hero_y), 1);
			enemy_walknum = 4;
		}
		else if (enemy_walknum > 1)
		{
			enemy_walknum--;
			enemy_state = 1;
			int rate_x = 1 + abs((x - enemy_x) / x);
			int rate_y = 1 + abs((y - enemy_y) / y);
			if (x > enemy_x&&abs(x - enemy_x) > 5)
			{
				e_direction = 0;
				enemy_x = enemy_x + 3 * rate_x;
			}
			else if (x < enemy_x&&abs(x - enemy_x)>5)
			{
				e_direction = 1;
				enemy_x = enemy_x - 3 * rate_x;
			}
			if (y > enemy_y&&abs(y - enemy_y) > 5)
			{
				enemy_y = enemy_y + 3 * rate_y;
			}
			else if (y < enemy_y&&abs(y - enemy_y)>5)
			{
				enemy_y = enemy_y - 3 * rate_y;
			}
		}
		else
		{
			enemy_walknum = 4;
			enemy_state = 1;
			int rate_x = 1 + abs((x - enemy_x) / x);
			int rate_y = 1 + abs((y - enemy_y) / y);
			if (x > enemy_x&&abs(x - enemy_x) > 5)
			{
				e_direction = 0;
				enemy_x = enemy_x + 3 * rate_x;
			}
			else if (x < enemy_x&&abs(x - enemy_x)>5)
			{
				e_direction = 1;
				enemy_x = enemy_x - 3 * rate_x;
			}
			if (y > enemy_y&&abs(y - enemy_y) > 5)
			{
				enemy_y = enemy_y + 3 * rate_y;
			}
			else if (y < enemy_y&&abs(y - enemy_y)>5)
			{
				enemy_y = enemy_y - 3 * rate_y;
			}
		}
	}
}

bool enemy::enemy_hurting1(bullet a)
{
	if (abs(a.bullet_x - enemy_x) < 31 && abs(a.bullet_y - enemy_y) < 20)
	{
		enemy_state = 2;
		enemy_Hp = enemy_Hp - 20;
		return true;
	}
	else return false;
}

bool enemy::enemy_hurting2(hero b)
{
	if (abs(b.hero_x - enemy_x) < 25 && b.hero_y - enemy_y < 10 && enemy_y - b.hero_y<20 && b.kick_state != 0 && lie_stable == 0)
	{
		enemy_state = 3;
		enemy_Hp = enemy_Hp - 20;
		lie_stable = 5;
		if (b.direction == 0)
		{
			enemy_x = enemy_x + 20;
			e_direction = 1;
		}
		else
		{
			enemy_x = enemy_x - 20;
			e_direction = 0;
		}
		return true;
	}
	else return false;
}

bool enemy::enemy_attacking(hero b)
{
	if ((enemy_state == 0 || enemy_state == 1) && abs(enemy_x - b.hero_x) < 15 && abs(enemy_y - b.hero_y) < 15 && lie_stable == 0 && b.kick_state == 0 && b.jump_state == 0)
	{
		enemy_state = 4;
		lie_stable = 2;
		return true;
	}
	else if (lie_stable == 1 && enemy_state == 4) { enemy_state = 0; }
	return false;
}

bool enemy::enemy_death()
{
	if (enemy_Hp <= 0 && enemy_deathnum == 0 && enemy_show != 0)
	{
		enemy_deathnum = 3;
		buffer = 3;
		enemy_show = 0;
		return false;
	}
	else if (enemy_deathnum != 0 && enemy_show == 0)
	{
		if (e_direction == 0)
		{
			putimage(enemy_x, enemy_y, &enemy_die[3 - enemy_deathnum][1], SRCAND);
			putimage(enemy_x, enemy_y, &enemy_die[3 - enemy_deathnum][0], SRCINVERT);
		}
		else
		{
			putimage(enemy_x, enemy_y, &enemy_die[6 - enemy_deathnum][1], SRCAND);
			putimage(enemy_x, enemy_y, &enemy_die[6 - enemy_deathnum][0], SRCINVERT);
		}
		buffer = buffer - 0.6;
		enemy_deathnum = buffer;
		if (enemy_deathnum == 0) { enemy_x = 600; enemy_y = -100; return true; }
		else return false;
	}
	else return false;

}

void enemy::show_enemy()
{
	if (enemy_state == 0)
	{
		putimage(enemy_x, enemy_y, &enemy_stable[e_direction][1], SRCAND);
		putimage(enemy_x, enemy_y, &enemy_stable[e_direction][0], SRCINVERT);
	}
	else if (enemy_state == 1)
	{
		if (e_direction == 0)
		{
			putimage(enemy_x, enemy_y, &enemy_walk[4 - enemy_walknum][1], SRCAND);
			putimage(enemy_x, enemy_y, &enemy_walk[4 - enemy_walknum][0], SRCINVERT);
		}
		else if (e_direction == 1)
		{
			putimage(enemy_x, enemy_y, &enemy_walk[8 - enemy_walknum][1], SRCAND);
			putimage(enemy_x, enemy_y, &enemy_walk[8 - enemy_walknum][0], SRCINVERT);
		}
	}
	else if (enemy_state == 2)
	{
		putimage(enemy_x, enemy_y, &enemy_hurt[e_direction][1], SRCAND);
		putimage(enemy_x, enemy_y, &enemy_hurt[e_direction][0], SRCINVERT);
		enemy_thrownum = 0;
		enemy_state = 0;
	}
	else if (enemy_state == 3)
	{
		if (e_direction == 0)
		{
			putimage(enemy_x, enemy_y, &enemy_die[2][1], SRCAND);
			putimage(enemy_x, enemy_y, &enemy_die[2][0], SRCINVERT);
		}
		else
		{
			putimage(enemy_x, enemy_y, &enemy_die[5][1], SRCAND);
			putimage(enemy_x, enemy_y, &enemy_die[5][0], SRCINVERT);
		}
	}
	else if (enemy_state == 4)
	{
		putimage(enemy_x, enemy_y, &enemy_attack[e_direction][1], SRCAND);
		putimage(enemy_x, enemy_y, &enemy_attack[e_direction][0], SRCINVERT);
	}
	else if (enemy_state == 5 && enemy_thrownum>0)
	{
		putimage(enemy_x, enemy_y, &enemy_throw[2 * e_direction + 2 - enemy_thrownum][1], SRCAND);
		putimage(enemy_x, enemy_y, &enemy_throw[2 * e_direction + 2 - enemy_thrownum][0], SRCINVERT);
		enemy_thrownum--;
		if (enemy_thrownum == 0)enemy_state = 0;
	}
	if (lie_stable > 0)
	{
		lie_stable--;
	}
	draw_enemystate();
}

void enemy::draw_enemystate()
{
	rectangle(enemy_x, enemy_y - 4, enemy_x + 40, enemy_y);
	setfillcolor(RED);
	solidrectangle(enemy_x + 1, enemy_y - 3, enemy_x + 1 + enemy_Hp * 0.38, enemy_y - 1);
}

void enemy::enemy_restart(int i)
{
	srand((unsigned)time(0)*(i + 1));
	x = rand() % 500 + 10;
	y = rand() % 400 + 10;
	enemy_x = rand() % 500 + 10;
	enemy_y = 0;
	e_direction = 0;
	enemy_Hp = 100;
	enemy_walknum = 0;
	enemy_deathnum = 0;
	enemy_thrownum = 0;
	enemy_state = 0;
	enemy_show = 1;
	lie_stable = 0;
	button = 0;
	//以下是“刀”的初始化
	knife_x = 0;
	knife_y = 0;
	knife_show = 0;
	k_direction = 0;
}

void enemy_appear(int killing_num, enemy *c)
{
	if (killing_num == 0)
	{
		if (c[0].enemy_show == 0 && c[0].enemy_deathnum == 0)
		{
			c[0].enemy_restart(0);
		}
	}
	else if (killing_num > 0 && killing_num < 5)
	{
		for (int i = 0; i < 3; i++)
		{
			if (c[i].enemy_show == 0 && c[i].enemy_deathnum == 0)
			{
				c[i].enemy_restart(i);
			}
		}
	}
	else if (killing_num > 4 && killing_num < 7)
	{
		for (int i = 0; i < 4; i++)
		{
			if (c[i].enemy_show == 0 && c[i].enemy_deathnum == 0)
			{
				c[i].enemy_restart(i);
			}
		}
	}
	else if (killing_num > 6 && killing_num < 14)
	{
		for (int i = 0; i < 6; i++)
		{
			if (c[i].enemy_show == 0 && c[i].enemy_deathnum == 0)
			{
				c[i].enemy_restart(i);
			}
		}
	}
	else if (killing_num > 13 && killing_num < 19)
	{
		for (int i = 0; i < 8; i++)
		{
			if (c[i].enemy_show == 0 && c[i].enemy_deathnum == 0)
			{
				c[i].enemy_restart(i);
			}
		}
	}
	else if (killing_num > 18 && killing_num < 24)
	{
		for (int i = 0; i < 11; i++)
		{
			if (c[i].enemy_show == 0 && c[i].enemy_deathnum == 0)
			{
				c[i].enemy_restart(i);
			}
		}
	}
	else
	{
		for (int i = 0; i < 15; i++)
		{
			if (c[i].enemy_show == 0 && c[i].enemy_deathnum == 0)
			{
				c[i].enemy_restart(i);
			}
		}
	}
}

void start_game()
{
	int l = length;
	int w = width;
	initgraph(l, w);

draw:
	settextstyle(40, 0, _T("Rage Italic"));
	RECT r0 = { 0,0,l,w / 3 };
	RECT r1 = { 250,120,350,150 };
	RECT r2 = { 250,150,350,180 };
	RECT r3 = { 250,180,350,210 };
	RECT r4 = { 250,210,350,240 };
	drawtext(_T("PIXEL NINJA"), &r0, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	settextstyle(20, 0, _T("等线"));
	rectangle(230, 120, 370, 150);
	drawtext(_T("start"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rectangle(230, 150, 370, 180);
	drawtext(_T("about game"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rectangle(230, 180, 370, 210);
	drawtext(_T("instruction"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rectangle(230, 210, 370, 240);
	drawtext(_T("quit"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	outtextxy(152, 440, _T("please read 'instruction' before you play"));

	int appage = 0;
	MOUSEMSG m;
	while (1)
	{
		BeginBatchDraw();
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			EndBatchDraw();
			if (m.x > 230 && m.x < 370 && m.y>120 && m.y < 150 && appage == 0)
			{
				goto end;
				break;
			}
			else if (m.x > 230 && m.x < 370 && m.y>150 && m.y < 180 && appage == 0)
			{
				cleardevice();
				rectangle(50, 50, 350, 220);
				rectangle(48, 48, 352, 222);
				outtextxy(52, 52, _T("game introduction："));
				outtextxy(52, 82, _T("pixel ninja 1.0"));
				outtextxy(52, 112, _T("fight with kongfu and dart"));
				outtextxy(52, 142, _T("@copyright:"));

				outtextxy(52, 295, _T("special thanks for:"));
				settextstyle(40, 0, _T("Rage Italic"));
				outtextxy(60, 172, _T("pptrick"));
				outtextxy(52, 330, _T("YYJ   CYY   LZY"));
				settextstyle(20, 0, _T("等线"));
				RECT back_button = { l - 80,w - 50,l - 10,w - 10 };
				rectangle(l - 80, w - 50, l - 10, w - 10);
				settextstyle(20, 0, _T("等线"));
				drawtext(_T("back"), &back_button, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				appage = 1;
				break;
			}
			else if (m.x > 230 && m.x < 370 && m.y>180 && m.y < 210 && appage == 0)
			{
				cleardevice();
				rectangle(50, 50, 320, 340);
				rectangle(48, 48, 322, 342);
				outtextxy(52, 52, _T("movements:"));
				outtextxy(52, 82, _T("left:                  A"));
				outtextxy(52, 112, _T("right:                D"));
				outtextxy(52, 142, _T("up:                   W"));
				outtextxy(52, 172, _T("down:               S"));
				outtextxy(52, 202, _T("jump:                space"));
				outtextxy(52, 232, _T("kick:                  K"));
				outtextxy(52, 262, _T("throw darts:      J"));
				outtextxy(52, 292, _T("pause & store:   P"));
				settextstyle(18, 0, _T("等线"));
				outtextxy(40, 360, _T("PCY's ADVICE:"));
				outtextxy(40, 390, _T("your darts are limited."));
				outtextxy(40, 420, _T("kicking will consume your energy."));
				outtextxy(40, 450, _T("you can buy 'anything' you need in the STORE."));
				RECT back_button = { l - 80,w - 50,l - 10,w - 10 };
				rectangle(l - 80, w - 50, l - 10, w - 10);
				settextstyle(20, 0, _T("等线"));
				drawtext(_T("back"), &back_button, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				appage = 1;
				break;
			}
			else if (m.x > 230 && m.x < 370 && m.y>210 && m.y < 240 && appage == 0)
			{
				exit(0);
			}
			else if (m.x > l - 80 && m.x < l - 10 && m.y>w - 50 && m.y < w - 10 && appage == 1)
			{
				cleardevice();
				goto draw;
			}
		case WM_MOUSEMOVE:
			if (appage == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (m.x > 230 && m.x < 370 && m.y>120 + i * 30 && m.y < 150 + i * 30)
					{
						setfillcolor(RED);
						fillrectangle(230, 120 + i * 30, 370, 150 + i * 30);
						setbkmode(TRANSPARENT);
						RECT r = { 230, 120 + i * 30, 370, 150 + i * 30 };
						settextstyle(25, 0, _T("等线"));
						switch (i)
						{
						case 0:
							drawtext(_T("start"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext(_T("about game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext(_T("instruction"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 3:
							drawtext(_T("quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					else
					{
						if (getpixel(231, 121 + i * 30) == RED)
						{
							setfillcolor(BLACK);
							fillrectangle(230, 120 + i * 30, 370, 150 + i * 30);
							setbkmode(TRANSPARENT);
							RECT r = { 230, 120 + i * 30, 370, 150 + i * 30 };
							settextstyle(20, 0, _T("等线"));
							switch (i)
							{
							case 0:
								drawtext(_T("start"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext(_T("about game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 2:
								drawtext(_T("instruction"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 3:
								drawtext(_T("quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							}
						}
					}
				}
			}
			else if (appage == 1)
			{
				if (m.x > l - 80 && m.x < l - 10 && m.y>w - 50 && m.y < w - 10)
				{
					setfillcolor(RED);
					fillrectangle(l - 80, w - 50, l - 10, w - 10);
					setbkmode(TRANSPARENT);
					RECT r = { l - 80, w - 50, l - 10, w - 10 };
					settextstyle(25, 0, _T("等线"));
					drawtext(_T("back"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					setfillcolor(BLACK);
					fillrectangle(l - 80, w - 50, l - 10, w - 10);
					setbkmode(TRANSPARENT);
					RECT r = { l - 80, w - 50, l - 10, w - 10 };
					settextstyle(20, 0, _T("等线"));
					drawtext(_T("back"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}
			FlushBatchDraw();
		}
	}
	//system("pause");

end:;

}

void store(hero &b)
{
	putimage(60, 120, &bottle);
	putimage(60, 200, &shoes);
	putimage(60, 280, &dart);
	RECT text1 = { 100,120,380,160 };
	RECT text2 = { 100,200,380,240 };
	RECT text3 = { 100,280,380,320 };
	settextstyle(15, 0, _T("等线"));
	drawtext(_T("Hp bottle: 40$ | regain 10 Hp"), &text1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T("shoes of Hermes: 300$ | make ninja faster"), &text2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T("ninja's darts: 50$ | obtain 10 darts"), &text3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	TCHAR money_num[20];
	_stprintf_s(money_num, _T("money:  %d"), b.money);
	outtextxy(90, 70, money_num);
}

void pause_game(hero &b)
{
	cleardevice();
	int l = length;
	int w = width;

draw:
	settextstyle(40, 0, _T("Rage Italic"));
	RECT r0 = { 0,0,l,w / 3 };
	RECT r1 = { 250,120,350,150 };
	RECT r2 = { 250,150,350,180 };
	RECT r3 = { 250,180,350,210 };
	RECT r4 = { 250,210,350,240 };
	RECT r5 = { 200,300,400,360 };
	drawtext(_T("GAME PAUSED"), &r0, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	settextstyle(20, 0, _T("等线"));
	rectangle(230, 120, 370, 150);
	drawtext(_T("continue"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rectangle(230, 150, 370, 180);
	drawtext(_T("about game"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rectangle(230, 180, 370, 210);
	drawtext(_T("instruction"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rectangle(230, 210, 370, 240);
	drawtext(_T("quit"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rectangle(200, 300, 400, 360);
	rectangle(198, 298, 402, 362);
	settextstyle(40, 0, _T("等线"));
	drawtext(_T("STORE"), &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	settextstyle(20, 0, _T("等线"));
	int appage = 0;
	MOUSEMSG m;
	while (1)
	{
		BeginBatchDraw();
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			EndBatchDraw();
			if (m.x > 230 && m.x < 370 && m.y>120 && m.y < 150 && appage == 0)
			{
				goto end;
				break;
			}
			else if (m.x > 230 && m.x < 370 && m.y>150 && m.y < 180 && appage == 0)
			{
				cleardevice();
				rectangle(50, 50, 350, 220);
				rectangle(48, 48, 352, 222);
				outtextxy(52, 52, _T("game introduction："));
				outtextxy(52, 82, _T("pixel ninja 1.0"));
				outtextxy(52, 102, _T("fight with kongfu and dart"));
				outtextxy(52, 132, _T("@copyright:"));
				settextstyle(40, 0, _T("Rage Italic"));
				outtextxy(60, 172, _T("pptrick"));
				settextstyle(20, 0, _T("等线"));
				RECT back_button = { l - 80,w - 50,l - 10,w - 10 };
				rectangle(l - 80, w - 50, l - 10, w - 10);
				settextstyle(20, 0, _T("等线"));
				drawtext(_T("back"), &back_button, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				appage = 1;
				break;
			}
			else if (m.x > 230 && m.x < 370 && m.y>180 && m.y < 210 && appage == 0)
			{
				cleardevice();
				rectangle(50, 50, 320, 300);
				rectangle(48, 48, 322, 302);
				outtextxy(52, 52, _T("movements:"));
				outtextxy(52, 82, _T("left:             A"));
				outtextxy(52, 112, _T("right:           D"));
				outtextxy(52, 142, _T("up:              W"));
				outtextxy(52, 172, _T("down:          S"));
				outtextxy(52, 202, _T("jump:           space"));
				outtextxy(52, 232, _T("kick:             K"));
				outtextxy(52, 262, _T("throw darts: J"));
				RECT back_button = { l - 80,w - 50,l - 10,w - 10 };
				rectangle(l - 80, w - 50, l - 10, w - 10);
				settextstyle(20, 0, _T("等线"));
				drawtext(_T("back"), &back_button, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				appage = 1;
				break;
			}
			else if (m.x > 230 && m.x < 370 && m.y>210 && m.y < 240 && appage == 0)
			{
				exit(0);
			}
			else if (m.x > 200 && m.x < 400 && m.y>300 && m.y < 360 && appage == 0)
			{
			store:
				cleardevice();
				store(b);
				settextstyle(40, 0, _T("Rage Italic"));
				RECT r0 = { 0,0,l,w / 3 };
				RECT r1 = { 400,120,450,150 };
				RECT r2 = { 400,200,450,230 };
				RECT r3 = { 400,280,450,310 };
				RECT r4 = { 400,360,450,390 };
				drawtext(_T("STORE"), &r0, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextstyle(20, 0, _T("等线"));
				rectangle(400, 120, 450, 150);
				drawtext(_T("buy"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				rectangle(400, 200, 450, 230);
				drawtext(_T("buy"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				rectangle(400, 280, 450, 310);
				drawtext(_T("buy"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				//rectangle(400, 360, 450, 390);
				//drawtext(_T("buy"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				RECT back_button = { l - 80,w - 50,l - 10,w - 10 };
				rectangle(l - 80, w - 50, l - 10, w - 10);
				settextstyle(20, 0, _T("等线"));
				drawtext(_T("back"), &back_button, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				appage = 2;
				break;
			}
			else if (m.x > l - 80 && m.x < l - 10 && m.y>w - 50 && m.y < w - 10 && (appage == 1 || appage == 2))
			{
				cleardevice();
				goto draw;
			}
			else if (appage == 2)
			{
				for (int i = 0; i < 3; i++)
				{
					if (m.x > 400 && m.x < 450 && m.y>120 + 80 * i&&m.y < 150 + 80 * i)
					{
						switch (i)
						{
						case 0:
							if (b.money >= 40)
							{
								b.money = b.money - 40;
								b.Hp = min(b.Hp + 10, 100);
							}
							goto store;
						case 1:
							if (1)
							{
								if (b.money >= 300)
								{
									b.money = b.money - 300;
									b.steplong++;
								}
							}
							goto store;
						case 2:
							if (b.money >= 50)
							{
								b.money = b.money - 50;
								b.dart_num = min(b.dart_num + 10, 20);
							}
							goto store;
						}
					}
				}
			}
		case WM_MOUSEMOVE:
			if (appage == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (m.x > 230 && m.x < 370 && m.y>120 + i * 30 && m.y < 150 + i * 30)
					{
						setfillcolor(RED);
						fillrectangle(230, 120 + i * 30, 370, 150 + i * 30);
						setbkmode(TRANSPARENT);
						RECT r = { 230, 120 + i * 30, 370, 150 + i * 30 };
						settextstyle(25, 0, _T("等线"));
						switch (i)
						{
						case 0:
							drawtext(_T("continue"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext(_T("about game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext(_T("instruction"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 3:
							drawtext(_T("quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					else
					{
						if (getpixel(231, 121 + i * 30) == RED)
						{
							setfillcolor(BLACK);
							fillrectangle(230, 120 + i * 30, 370, 150 + i * 30);
							setbkmode(TRANSPARENT);
							RECT r = { 230, 120 + i * 30, 370, 150 + i * 30 };
							settextstyle(20, 0, _T("等线"));
							switch (i)
							{
							case 0:
								drawtext(_T("continue"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext(_T("about game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 2:
								drawtext(_T("instruction"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 3:
								drawtext(_T("quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							}
						}
					}
				}
				if (m.x > 200 && m.x < 400 && m.y>300 && m.y < 360)
				{
					setfillcolor(RED);
					fillrectangle(200, 300, 400, 360);
					setbkmode(TRANSPARENT);
					RECT r = { 200, 300, 400, 360 };
					settextstyle(45, 0, _T("等线"));
					drawtext(_T("STORE"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					if (getpixel(201, 301) == RED)
					{
						setfillcolor(BLACK);
						fillrectangle(200, 300, 400, 360);
						setbkmode(TRANSPARENT);
						RECT r = { 200, 300, 400, 360 };
						settextstyle(40, 0, _T("等线"));
						drawtext(_T("STORE"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						settextstyle(20, 0, _T("等线"));
					}
				}
			}
			else if (appage == 1)
			{
				if (m.x > l - 80 && m.x < l - 10 && m.y>w - 50 && m.y < w - 10)
				{
					setfillcolor(RED);
					fillrectangle(l - 80, w - 50, l - 10, w - 10);
					setbkmode(TRANSPARENT);
					RECT r = { l - 80, w - 50, l - 10, w - 10 };
					settextstyle(25, 0, _T("等线"));
					drawtext(_T("back"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					setfillcolor(BLACK);
					fillrectangle(l - 80, w - 50, l - 10, w - 10);
					setbkmode(TRANSPARENT);
					RECT r = { l - 80, w - 50, l - 10, w - 10 };
					settextstyle(20, 0, _T("等线"));
					drawtext(_T("back"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}
			else if (appage == 2)
			{
				for (int i = 0; i < 3; i++)
				{
					if (m.x > 400 && m.x < 450 && m.y>120 + i * 80 && m.y < 150 + i * 80)
					{
						setfillcolor(RED);
						fillrectangle(400, 120 + i * 80, 450, 150 + i * 80);
						setbkmode(TRANSPARENT);
						RECT r = { 400, 120 + i * 80, 450, 150 + i * 80 };
						settextstyle(25, 0, _T("等线"));
						switch (i)
						{
						case 0:
							drawtext(_T("buy"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext(_T("buy"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext(_T("buy"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					else
					{
						if (getpixel(401, 121 + i * 80) == RED)
						{
							setfillcolor(BLACK);
							fillrectangle(400, 120 + i * 80, 450, 150 + i * 80);
							setbkmode(TRANSPARENT);
							RECT r = { 400, 120 + i * 80, 450, 150 + i * 80 };
							settextstyle(20, 0, _T("等线"));
							switch (i)
							{
							case 0:
								drawtext(_T("buy"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext(_T("buy"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 2:
								drawtext(_T("buy"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							}
						}
					}
				}
				if (m.x > l - 80 && m.x < l - 10 && m.y>w - 50 && m.y < w - 10)
				{
					setfillcolor(RED);
					fillrectangle(l - 80, w - 50, l - 10, w - 10);
					setbkmode(TRANSPARENT);
					RECT r = { l - 80, w - 50, l - 10, w - 10 };
					settextstyle(25, 0, _T("等线"));
					drawtext(_T("back"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					setfillcolor(BLACK);
					fillrectangle(l - 80, w - 50, l - 10, w - 10);
					setbkmode(TRANSPARENT);
					RECT r = { l - 80, w - 50, l - 10, w - 10 };
					settextstyle(20, 0, _T("等线"));
					drawtext(_T("back"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}
			FlushBatchDraw();
		}
	}
	//system("pause");

end:;
}

void hero::hero_death(int score)
{
	int highest_score;
	for (int i = 0; i < 4; i++)
	{
		cleardevice();
		if (direction == 0)
		{
			putimage(hero_x, hero_y, &hero_die[i + 4][1], SRCAND);
			putimage(hero_x, hero_y, &hero_die[i + 4][0], SRCINVERT);
			hero_x = hero_x - 10;
		}
		else
		{
			putimage(hero_x, hero_y, &hero_die[i][1], SRCAND);
			putimage(hero_x, hero_y, &hero_die[i][0], SRCINVERT);
			hero_x = hero_x + 10;
		}
		Sleep(500);
	}
	settextstyle(80, 0, _T("Harrington"));
	RECT r0 = { 0,0,600,480 };
	drawtext(_T("GAME OVER"), &r0, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	ifstream infile;
	ofstream outfile;
	infile.open("score.dat", ios::in | ios::_Nocreate);
	if (!infile)
	{
		highest_score = 0;
		outfile.open("score.dat", ios::out);
		outfile << highest_score;
	}
	else infile >> highest_score;

	if (score > highest_score)
	{
		outfile.open("score.dat", ios::out);
		highest_score = score;
		outfile << highest_score;
	}

	TCHAR scoretext[20];
	TCHAR highscoretext[20];
	settextstyle(25, 0, _T("等线"));
	_stprintf_s(scoretext, _T("score: %d"), score);
	_stprintf_s(highscoretext, _T("highest score: %d"), highest_score);
	outtextxy(200, 330, scoretext);
	outtextxy(200, 360, highscoretext);
	system("pause");
}

int main()
{
	mciSendString(_T("open D:\\res\\beginning.mp3 alias mymusic1"), NULL, 0, NULL);
	mciSendString(_T("open D:\\res\\Gunshow.mp3 alias mymusic2"), NULL, 0, NULL);
	mciSendString(_T("open D:\\res\\death.mp3 alias mymusic3"), NULL, 0, NULL);
	mciSendString(_T("open D:\\res\\enemy_yell.mp3 alias mymusic4"), NULL, 0, NULL);
	mciSendString(_T("open D:\\res\\hero_yell.mp3 alias mymusic5"), NULL, 0, NULL);
	mciSendString(_T("setaudio mymusic1 volume to 300"), NULL, 0, NULL);
	mciSendString(_T("setaudio mymusic2 volume to 300"), NULL, 0, NULL);
	mciSendString(_T("setaudio mymusic3 volume to 300"), NULL, 0, NULL);
	mciSendString(_T("setaudio mymusic4 volume to 300"), NULL, 0, NULL);
	mciSendString(_T("setaudio mymusic5 volume to 300"), NULL, 0, NULL);
	mciSendString(_T("play mymusic1 repeat"), NULL, 0, NULL);
	start_game();
	mciSendString(_T("stop mymusic1"), NULL, 0, NULL);
	mciSendString(_T("play mymusic2 repeat"), NULL, 0, NULL);
	initialize_graph();
	hero LSR(100, 100, 0, 0);
	enemy LYS[15];
	bullet dart[20];
	int dart_num = 0;
	int hardness = 0;
	int killing_num = 0;
	while (1)
	{
		bool left = 0, right = 0, up = 0, down = 0, jump = 0, kick = 0, shoot = 0, pause = 0;
		left = GetAsyncKeyState(65);
		right = GetAsyncKeyState(68);
		up = GetAsyncKeyState(87);
		down = GetAsyncKeyState(83);
		jump = GetAsyncKeyState(32);
		kick = GetAsyncKeyState(75);
		shoot = GetAsyncKeyState(74);
		pause = GetAsyncKeyState(80);
		bool gua = GetAsyncKeyState(9);

		if (pause == 1)pause_game(LSR);

		BeginBatchDraw();
		putimage(0, 0, &background);
		LSR.hero_walking(left, right, up, down, jump);
		LSR.hero_jumping(left, right, up, down, jump);
		LSR.hero_kicking(kick);
		//以下是子弹运行部分
		if (shoot == 1 && LSR.dart_num>0)
		{
			LSR.hero_shoot();
			for (int i = 0; i < 20; i++)
			{
				if (dart[i].show == 0)
				{
					dart_num = i;
					dart[dart_num].same(LSR);
					dart[dart_num].show = 1;
					break;
				}
			}
		}
		for (int i = 0; i < 20; i++)
		{
			if (dart[i].show == 1)
			{
				dart[i].bullet_draw();
			}
			if (dart[i].bullet_x < 0 || dart[i].bullet_x>600)
			{
				dart[i].show = 0;
			}
		}
		if (gua == 1) { LSR.dart_num = LSR.fulldart; gua = 0; }

		enemy_appear(killing_num, LYS);
		for (int j = 0; j < 15; j++)
		{
			if (LYS[j].enemy_show == 1)
			{
				LYS[j].enemy_walking(LSR, j);
			}
			for (int i = 0; i < 20; i++)
			{
				if (LYS->enemy_show == 1)
				{
					if (LYS[j].enemy_hurting1(dart[i]))
					{
						dart[i].show = 0;
						dart[i].bullet_x = -100;
						dart[i].bullet_y = -100;
					}
				}
			}
			if (LYS->enemy_show == 1)
			{
				if (LYS[j].enemy_hurting2(LSR)) { LSR.score = LSR.score + 10; mciSendString(_T("play mymusic4 from 0"), NULL, 0, NULL); }
				if (LYS[j].enemy_throwing(LSR) && LSR.Hp > 0 && LYS->enemy_show == 1) { LSR.Hp = LSR.Hp - 10; LSR.hurt_state = 1; mciSendString(_T("play mymusic5 from 0"), NULL, 0, NULL); }
				if (LYS[j].enemy_attacking(LSR) && LSR.Hp > 0 && LYS->enemy_show == 1) { LSR.Hp = LSR.Hp - 5; LSR.hurt_state = 1; mciSendString(_T("play mymusic5 from 0"), NULL, 0, NULL); }
			}

			/*if (LYS.compare(LSR))
			{
			if (LYS.enemy_show == 1)
			{
			LYS.show_enemy();
			}
			LSR.hero_draw();
			}
			else
			{
			LSR.hero_draw();
			if (LYS.enemy_show == 1)
			{
			LYS.show_enemy();
			}
			}//实现前后的仿真*/
			if (LYS[j].enemy_show == 1)
			{
				LYS[j].show_enemy();
			}
			if (LYS[j].enemy_death()) { killing_num++; LSR.score = LSR.score + 40; LSR.money = LSR.money + 30; }
		}
		if (LSR.Hp <= 0) { EndBatchDraw(); break; }
		LSR.hero_draw();
		LSR.draw_herostate();

		Sleep(100);
		EndBatchDraw();
	}
	mciSendString(_T("stop mymusic2"), NULL, 0, NULL);
	mciSendString(_T("play mymusic3 repeat"), NULL, 0, NULL);
	LSR.hero_death(LSR.score);
	return 0;
}