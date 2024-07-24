#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define size_y 7
#define size_x 30
#define KB_UP 72

//상수값******************************************************************************
enum { BLK, WAL, HUM, OBS };

//전역 변수***************************************************************************
int map[size_y][100];
char custom[100];
int x, y;
int len;
int fall = 1;
int p = 0;

//함수 선언***************************************************************************
void line();
void menu();
void menu1();
void mapbase();              //맵에 값 저장
void mapping();              //맵 출력
void Jump();                 //점프
void Down();	             //점프 하거나 말거나, 주기적으로 플레이어를 밑으로 내려가게 하는 함수(이름은 재밌게 중력으로 함 ㅎ)
void Obstacle();             //장애물
void MapMove();              //맵을 왼쪽으로 한 칸씩 밈
int Gameover();              //게임 오버 화면
void print_xy(int x, int y);    //화면을 안 깜빡거리게 할려고 콘솔창 맨 위로 가 덮어서 출력하게 하기 위해 씀
void CursorView(int show);  //커서를 안 보이게 하기 위해 씀


							//메인함수
int main()
{
	int i, j, turn, k = 0;
	int t = 0;
	int way = 1;
	int cnt = 0;
	custom[0] = '4';

	srand((unsigned)time(NULL));

	CursorView(0);

	while (1)
	{
		if (way == 1 || way == 2 || way == 3)
			menu();

		printf("메뉴를 선택하세요: ");
		scanf("%d", &way);

		if (way == 1)
		{
			menu1();
			while (1)
			{
				cnt = 0;
				scanf(" %s", custom);
				len = strlen(custom);
				for (i = 0;i < len;i++)
				{
					if (custom[i] >= '0'&&custom[i] <= '3')
						cnt++;
				}
				if (cnt == len)
					break;
				else
				{
					printf("맵이 올바르지 않습니다. 다시 입력해 주세요.\n");
					custom[0] = NULL;
				}
			}
		}
		else if (way == 2)
		{
			if (custom[0] == '4')
			{
				printf("먼저 맵을 만들어 주세요.\n");
				way = NULL;
			}
			else
			{
				turn = 1;

				system("cls");
				if (turn == 1)
				{
					print_xy(0, 0);
					mapbase();
					mapping();
				}

				while (turn)
				{
					if (_kbhit())
					{
						if (map[5][x] == HUM)
						{
							if (getch() == KB_UP) fall = 0;
						}
					}
					if (map[0][x] == HUM) fall = 1;
					if (fall == 0)
					{
						if (t % 10 == 0) Jump();
					}
					else
					{
						if (t % 10 == 0) Down();
					}
					if (t % 10 == 0) MapMove();
					if (t % 100 == 0)
					{
						Obstacle();
					}
					print_xy(0, 0);
					mapping();
					turn = Gameover();
					t++;
				}
				system("cls");
			}
		}
		else if (way == 3)
		{
			printf("게임을 종료합니다.\n");
			break;
		}
		else
		{
			printf("\"%d\"은 없는 메뉴입니다.\n", way);
			continue;
		}
	}
	return 0;
}
//줄 표현
void line()
{
	printf("==========================\n");
}
//메뉴출력
void menu()
{
	line();
	printf("%%%%%% 신나는 허들 게임 %%%%%%\n");
	printf("1: 맵 만들기\n");
	printf("2: 플레이\n");
	printf("3: 종료\n");
	line();
}
//맵만들기 메뉴 출력
void menu1()
{
	line();
	printf("맵을 입력합니다.\n");
	printf("(0: 바닥, 1~3: 장애물의 높이)\n");
	line();
}
//배열에 값을 입력
void mapbase()
{
	for (int i = 0;i<size_y;i++)
	{
		for (int j = 0;j<size_x;j++)
		{
			if (i == size_y - 1) map[i][j] = WAL;
			else map[i][j] = BLK;
		}
	}
	map[size_y - 2][4] = HUM;
	x = 4;
	y = size_y - 2;
}
//맵을 화면에 출력
void mapping()
{
	for (int i = 0; i<size_y; i++)
	{
		for (int j = 0; j<size_x; j++)
		{
			if (map[i][j] == BLK) printf(" ");
			if (map[i][j] == WAL) printf("=");
			if (map[i][j] == HUM) printf("O");
			if (map[i][j] == OBS) printf("X");
		}
		printf("\n");
	}
}
//점프
void Jump()
{
	int c;
	if (map[y - 1][x] == BLK)
	{
		c = map[y][x];
		map[y][x] = map[y - 1][x];
		map[y - 1][x] = c;
		y--;
	}
}
void Down()
{
	int c;
	if (map[y + 1][x] == BLK)
	{
		c = map[y][x];
		map[y][x] = map[y + 1][x];
		map[y + 1][x] = c;
		y++;
	}
}
//장애물
void Obstacle()
{
	if (custom[p] == '0')
	{
		map[size_y - 2][size_x - 2] = BLK;
		map[size_y - 2][size_x - 2] = BLK;
	}
	else if (custom[p] == '1')
	{
		map[size_y - 2][size_x - 2] = OBS;
	}
	else if (custom[p] == '2')
	{
		map[size_y - 3][size_x - 2] = OBS;
		map[size_y - 2][size_x - 2] = OBS;

	}
	else if (custom[p] == '3')
	{
		map[size_y - 4][size_x - 2] = OBS;
		map[size_y - 3][size_x - 2] = OBS;
		map[size_y - 2][size_x - 2] = OBS;
	}
	p++;
	if (len == p) p = 0;
}
//맵을 한 칸씩 밀어보리기
void MapMove()
{
	int c;

	for (int i = 1; i < size_y - 1; i++)
	{
		for (int j = 1; j < size_x - 1; j++)
		{
			if (j == 1) map[i][j] = BLK;
			else if (map[i][j] == BLK && map[i][j - 1] != HUM)
			{
				c = map[i][j];
				map[i][j] = map[i][j - 1];
				map[i][j - 1] = c;
			}
			else if (map[i][j] == OBS)
			{
				map[i][j - 1] = OBS;
				map[i][j] = BLK;
			}
		}
	}
}
//엔딩 여부 확인
int Gameover()
{
	int gameturning = 1;
	if (map[y][x] != HUM) gameturning = 0;
	return gameturning;
}
//그놈의 print_xy
void print_xy(int x, int y)
{
	COORD Pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
//커서 없애기용
void CursorView(int show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
