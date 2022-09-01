#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;

void start(); //开始界面
void Init(); //初始化游戏数据
void chose(); //选择界面
void DrawMap();//绘画界面
void Init();//初始化游戏界面
void Addfood();//添加食物
enum {
	SPACE, WALL, SNAKE, HEAD,FOOD  //依次代表空地，墙，蛇身，头
};
IMAGE img;
#define ROW 46
#define COL 64
int map[ROW][COL];
int mode; //模式
int score; //分数
char SnakeDir;//蛇的方向
int t1, t2; //代表时间
COORD food;
struct snake
{
    COORD pos;    //typedef struct _COORD {SHORT X;   SHORT Y;} COORD
    struct snake* next; //连接结点
};
struct snake* Head; //表示蛇头
void AddPoint(int x, int y); //增加结点
void DelPoint(); //删除结点
void ChangeDir(); //调整方向
void move(); //蛇动起来
struct note
{
    int x;
    int y;
    char snakeDir[2500];
};
struct note que[2500];
int head, tail;
char Next[4] = { 'D','A','S','W' };
void autoseek()
{
    head = 0, tail = 0;
    int s = 0, min = 2500;
    que[head].x = Head->pos.X, que[head].y = Head->pos.Y;
    tail++;
    while (1)
    {
        if (s < min)
            min = s;
        while (head < tail)
        {
            for (int k = 0; k < 3; k++)
            {
                if (k == 0 && map[que[head].x][que[head].y + 1] != SNAKE && map[que[head].x][que[head].y + 1] != HEAD && map[que[head].x][que[head].y + 1] != WALL)
                {
                    que[head].snakeDir[head] = Next[k];
                    head++, que[head].y++;
                }
                if (k == 1 && map[que[head].x][que[head].y - 1] != SNAKE && map[que[head].x][que[head].y - 1] != HEAD && map[que[head].x][que[head].y - 1] != WALL)
                {
                    que[head].snakeDir[head] = Next[k];
                    head++, que[head].y--;
                }
                if (k == 2 && map[que[head].x + 1][que[head].y] != SNAKE && map[que[head].x + 1][que[head].y] != HEAD && map[que[head].x + 1][que[head].y] != WALL)
                {
                    que[head].snakeDir[head] = Next[k];
                    head++, que[head].x++;
                }
                if (k == 3 && map[que[head].x - 1][que[head].y] != SNAKE && map[que[head].x - 1][que[head].y] != HEAD && map[que[head].x - 1][que[head].y] != WALL)
                {
                    que[head].snakeDir[head] = Next[k];
                    head++, que[head].x--;
                }
                if (map[que[tail].x][que[tail].y] == FOOD)
                    break;
                Head->pos.X = que[head].x, Head->pos.Y = que[head].y;
                s++, tail++;
            }
        }
    }
    SnakeDir = que[Head->pos.X].snakeDir[Head->pos.Y];
}
int main()
{
	initgraph(640, 480);
	start();
	outtextxy(160, 240, L"->");
	chose();
	Init();
    while (1)
    {
        PlaySound(L"彩豆森林.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
        while (1)
        {
            DrawMap();
            if (_kbhit())
            {
                ChangeDir();
                move();
                t2 = GetTickCount();
                t1 = t2;
            }
            t2 = GetTickCount();
            if (t2 - t1 > 10)
            {
                move();
                if (mode == 3)
                    autoseek();
                t1 = t2;
            }
        }
    }
	getchar();
    closegraph();
	return 0;
}
void start()
{
	cleardevice();
	loadimage(&img, L"Image0.jpg", 640, 480);  //保存图片
	putimage(0, 0, &img);  //显示图片
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	settextcolor(WHITE);		//设置字体颜色为白色
	outtextxy(170, 200, L"模式选择");
	outtextxy(170, 240, L"1.普通模式");
	outtextxy(170, 280, L"2.穿墙模式");
	outtextxy(170, 320, L"3.无敌模式");
    outtextxy(170, 360, L"4.自动模式");
	outtextxy(380, 220, L"游戏说明:");
	outtextxy(360, 280, L"Enter键进入游戏"); 
	outtextxy(300, 320, L"字母键 W,S,A,D 方向键 上下左右 控制方向");
	outtextxy(350, 360, L"长按方向加速，空格暂停");
}
void chose()
{
	while (1)
	{
		switch (_getch())
		{
		case '1':
			start();
			outtextxy(160, 240, L"->");
			mode = 0;
			break;
		case '2':
			start();
			outtextxy(160, 280, L"->");
			mode = 1;
			break;
		case '3':
			start();
			outtextxy(160, 320, L"->");
			mode = 2;
			break;
        case '4':
            start();
            outtextxy(160, 360, L"->");
            mode = 3;
            break;
		case 13:  //Enter键进入游戏
			return;
		deault:
			break;
		}
	}
}
void Init()
{
	srand((unsigned)time(NULL));//随机种子，给后面DrawMap中画蛇头使用
	memset(map, SPACE, sizeof(map));//初始化数组map为SPACE
	for (int x =0; x < COL; x++)
		map[0][x] = map[ROW - 1][x] = WALL;
	for (int y = 0; y < ROW; y++)
		map[y][0] = map[y][COL-1] = WALL;
    SnakeDir = 'D';
    AddPoint(5, 3);
    AddPoint(5, 4);
    AddPoint(5, 5);
    AddPoint(5, 6);
    Addfood();
}
void ChangeDir()
{
    switch (_getch())
    {
    case 'A':
    case 'a':
    case 75:
        if (SnakeDir != 'D')
            SnakeDir = 'A';
        break;
    case 'D':
    case 'd':
    case 77:
        if (SnakeDir != 'A')
            SnakeDir = 'D';
        break;
    case 'W':
    case 'w':
    case 72:
        if (SnakeDir != 'S')
            SnakeDir = 'W';
        break;
    case 'S':
    case 's':
    case 80:
        if (SnakeDir != 'W')
            SnakeDir = 'S';
        break;
    case 32:
        _getch();
        break;
    default:
        break;
    }
}
void AddPoint(int x, int y)
{
    struct snake* point = (struct snake*)malloc(sizeof(struct snake));
    if (point == NULL)
    {
        return;
    }
    else
    {
        point->pos.X = x;
        point->pos.Y = y;
        if (Head != NULL)
        {
            map[Head->pos.X][Head->pos.Y] = SNAKE;  //这里X,Y分别代表纵坐标，横坐标
        }
           point->next = Head;
           Head = point;
           map[Head->pos.X][Head->pos.Y] = HEAD;
    }
}
void DelPoint()
{
    if (Head == NULL || Head->next == NULL)
    {
        return;
    }
    struct snake* p = (struct snake*)malloc(sizeof(struct snake));
    struct snake* q = (struct snake*)malloc(sizeof(struct snake));
    if (p == NULL || q == NULL)
    {
        return;
    }
    p = Head;
    q = p->next;
    while (q->next)
    {
        p = p->next;
        q = p->next;
    }
    map[q->pos.X][q->pos.Y] = SPACE;
    free(q);
    q = NULL;
    p->next = NULL;
}
void Addfood()
{
    int x, y;
    do
    {
        x = rand() % (ROW-1)+1;
        y = rand() % (COL - 1) + 1;
    } while (map[x][y] != SPACE);
    food.X = x;
    food.Y = y;
    map[x][y] = FOOD;
}
void move()
{
    struct snake* newpoint = (struct snake*)malloc(sizeof(struct snake));
    if (newpoint == NULL)
    {
        return;
    }
    switch (SnakeDir)
    {
    case 'D':
        newpoint->pos.X = Head->pos.X;
        newpoint->pos.Y = Head->pos.Y + 1; //理清X,Y代表什么
        break;
    case 'A':
        newpoint->pos.X = Head->pos.X;
        newpoint->pos.Y = Head->pos.Y - 1;
        break;
    case 'W':
        newpoint->pos.X = Head->pos.X - 1;
        newpoint->pos.Y = Head->pos.Y;
        break;
    case 'S':
        newpoint->pos.X = Head->pos.X + 1;
        newpoint->pos.Y = Head->pos.Y;
        break;
    default:
        break;
    }
    switch (map[newpoint->pos.X][newpoint->pos.Y])
    {
    case SPACE:
        AddPoint(newpoint->pos.X, newpoint->pos.Y);
        DelPoint();
        break;
    case SNAKE:
        if (mode==2)
        {
            AddPoint(newpoint->pos.X, newpoint->pos.Y);
            DelPoint();
        }
        else
        {
            PlaySound(NULL, 0, 0);
            MessageBox(GetHWnd(), L"游戏结束，继续努力", L"结果", MB_OK);
          		exit(0);
        }
        break;
    case WALL:
        if (mode==1||mode==2)
        {
            switch (SnakeDir)
            {
            case 'A':
                newpoint->pos.Y = COL - 2;
                break;
            case 'D':
                newpoint->pos.Y = 1;
                break;
            case 'W':
                newpoint->pos.X = ROW - 2;
                break;
            case 'S':
                newpoint->pos.X = 1;
                break;
            default:
                break;
            }
            AddPoint(newpoint->pos.X, newpoint->pos.Y);
            DelPoint();
        }
        else
        {
            PlaySound(NULL, 0, 0);
            MessageBox(GetHWnd(), L"游戏结束，继续努力", L"结果", MB_OK);
            exit(0);
        }
        break;
    case FOOD:
        score++;
        AddPoint(newpoint->pos.X, newpoint->pos.Y);
        Addfood();
        break;
    default:
        break;
    }
}
void DrawMap()
{
    BeginBatchDraw(); //开始绘图
    settextcolor(RGB(238, 0, 0)); //设置字体颜色为红色
    cleardevice(); //刷新屏幕
    loadimage(&img, L"Image.jpg", 640, 480);  //保存图片
    putimage(0, 0, &img);  //显示图片
    WCHAR arr[10]; //保存分数
    wsprintf(arr, L"得分：%d", score); //分数放在arr中
    outtextxy(0, 0, arr); //显示分数
    for (int y = 0; y < ROW; y++)
        for (int x = 0; x < COL; x++)
        {
            switch (map[y][x])
            {
            case SPACE:
                break;
            case WALL:
                setlinecolor(RED);  //边框线条颜色
                setfillcolor(RGB(187, 255, 255));  //填充颜色
                fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);  //画矩形
                break;
            case SNAKE:
                setlinecolor(RGB(78, 238, 148));
                setfillcolor(WHITE);
                fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            case HEAD:
                switch (rand() % 7)
                {
                case 0:
                    setlinecolor(RGB(78, 238, 148));
                    setfillcolor(RED);
                    fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                    break;
                case 1:
                    setlinecolor(RGB(78, 238, 148));
                    setfillcolor(WHITE);
                    fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                    break;
                case 2:
                    setlinecolor(RGB(78, 238, 148));
                    setfillcolor(RGB(100, 149, 237));
                    fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                    break;
                case 3:
                    setlinecolor(RGB(78, 238, 148));
                    setfillcolor(RGB(127, 255, 0));
                    fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                    break;
                case 4:
                    setlinecolor(RGB(78, 238, 148));
                    setfillcolor(RGB(176, 224, 230));
                    fillrectangle(x * 10, y * 10+20, x * 10 + 10, y * 10 + 30);
                    break;
                case 5:
                    setlinecolor(RGB(78, 238, 148));
                    setfillcolor(RGB(102, 139, 139));
                    fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 +30);
                    break;
                case 6:
                    setlinecolor(RGB(78, 238, 148));
                    setfillcolor(RGB(46, 139, 87));
                    fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 +30);
                    break;
                default:
                    break;
                }
                break;
            case FOOD:
                setlinecolor(WHITE);
                setfillcolor(RED);
                fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
            }
        }
    EndBatchDraw(); //结束绘图
}