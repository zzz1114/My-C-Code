#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define MapHeigth 30    //地图的 长，宽
#define MapWidth  100
#define SnakeMax 150    //蛇的最大长度

struct
{
    int x[SnakeMax]; //蛇每一节坐标
    int y[SnakeMax];
    int len;         //蛇长度
    int speed;
}snake;

struct
{
    int x;      //食物坐标
    int y;
}food;

void gotoxy(int, int);
void DrawMap();     //画地图
void newSnake();    //蛇更新
void DrawSnake();   //画蛇
void CreateFood();  //画食物
void KeyDown();     //按键操作
void ShowInformation();     //显示蛇的信息
void AutoMove();                //让电脑自己走
void Auto();                    //电脑自动走的时候更新蛇
void ColorPrint(char *, char);         //改变蛇的颜色
void DrawRoad();                        //A星寻路
int SnakeState();   //蛇状态
char key = 'w';
int score = 0;

int main()
{
    snake.len = 3;
                            snake.speed = 50;
    DrawMap();
    int flag = 1;
    while(flag)
    {
       KeyDown();
        //AutoMove();
        //Auto();
       DrawSnake();
       CreateFood();
       flag = SnakeState();
       ShowInformation();
       Sleep(snake.speed);


    }
    gotoxy(0, 32);
    printf("GameOver!!");
    system("pause");
    return 0;
}


//画蛇
void DrawSnake()
{
    int i;
    for(i = 0; i < snake.len; i++)
    {
        gotoxy(snake.x[i], snake.y[i]);
        if(i == 0) ColorPrint("■", 4);
        else
            printf("■");
    }
    gotoxy(0, 32);
}

//画地图
void DrawMap()
{
    int i, j;
    for(i = 0; i <= MapHeigth; i++)
    {
        gotoxy(0, i);
        printf("■");
        gotoxy(MapWidth, i);
        printf("■");
    }
    for(j = 0; j <= MapWidth; j+=2)
    {
        gotoxy(j, 0);
        printf("■");
        gotoxy(j, MapHeigth);
        printf("■");
    }

    snake.x[0] = MapWidth / 2;
    snake.y[0] = MapHeigth / 2;
    int p;
    for (p = 1; p < snake.len; p++)
    {
        snake.x[p] = snake.x[p - 1];
        snake.y[p] = snake.y[p - 1] + 1;
    }

    int k;
    for (k = 0; k < snake.len; k++)
    {
        gotoxy(snake.x[k], snake.y[k]);
        printf("■");
    }
    //画食物
    srand(time(NULL));
    while(1)
    {
        food.x = rand() % (MapWidth - 4) + 2;
        food.y = rand() % (MapHeigth - 2) + 1;
        int flag = 1;
        int q;
        for (q = 0; q < snake.len; q++)
        {
            if(food.x == snake.x[q] && food.y == snake.y[q])
                flag = 0;
        }
        if(flag == 1 && food.x % 2 == 0)
            break;
    }
    gotoxy(food.x, food.y);
    //printf("☆");
    ColorPrint("☆", 0x27);
}

int SnakeState()
{
    int flag = 1;
    int i;
    for(i = 1; i < snake.len; i++)
    {
        if(snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
        {
            flag = 0;
            break;
        }
    }
    if(snake.x[0] == 0 || snake.x[0] == MapWidth || snake.y[0]  == 0 || snake.y[0] == MapHeigth)
        flag = 0;
    return flag;
}

void CreateFood()
{

    if(snake.x[0] == food.x && snake.y[0] == food.y)
    {
        while(1)
        {

            food.x = rand() % (MapWidth - 4) + 2;
            food.y = rand() % (MapHeigth - 2) + 1;
            int flag = 1;
            int q;
            for (q = 0; q < snake.len; q++)
            {
                if(food.x == snake.x[q] && food.y == snake.y[q])
                    flag = 0;
            }
            if(flag == 1 && food.x % 2 == 0)
                break;
        }
        gotoxy(food.x, food.y);
        //printf("☆");
        ColorPrint("☆", 0x27);
        gotoxy(0, 32);
        //snake.speed -= 10;
        snake.len++;
    }

}

void ShowInformation()
{
    gotoxy(MapWidth + 4, 3);
        printf("长度\t：%d", snake.len);
    gotoxy(MapWidth + 4, 5);
        printf("得分\t：%d", (snake.len - 3) * 50);
    gotoxy(MapWidth + 4, 7);
        printf("当前速度：%d", (300 - snake.speed) / 10);
    gotoxy(0, 32);
}

void KeyDown()
{
    char flag;
    if(_kbhit())
    {
        flag = getch();
        if(flag - key != 4 && flag - key != -4 && flag - key != 3 && flag - key != -3 && flag != 'p' && flag != 'P')
            key = flag;
        fflush(stdin);
    }
    //清除尾部
    gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
    printf("  ");
    gotoxy(0, 32);
    int i;
    for(i = snake.len - 1; i > 0; i--)
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }
    switch(key)
    {
    case 'a':
    case 'A':
        snake.x[0] -= 2;
        break;
    case 'd':
    case 'D':
        snake.x[0] += 2;
        break;
    case 'w':
    case 'W':
        snake.y[0]--;
        break;
    case 's':
    case 'S':
        snake.y[0]++;
        break;
    case 'p':
    case 'P':
        system("pause");
        break;
    }



}

void Auto()
{
    gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
    printf("  ");
    gotoxy(0, 32);
    int i;
    for(i = snake.len - 1; i > 0; i--)
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }
    switch(key)
    {
    case 'a':
    case 'A':
        snake.x[0] -= 2;
        break;
    case 'd':
    case 'D':
        snake.x[0] += 2;
        break;
    case 'w':
    case 'W':
        snake.y[0]--;
        break;
    case 's':
    case 'S':
        snake.y[0]++;
        break;
    case 'p':
    case 'P':
        system("pause");
        break;
    }
}

void AutoMove()         //现在要实现不让电脑咬自己
{
    char flag = key;
    int j;
    int i;
    while(1)
    {

        if(snake.y[0] > food.y)
        {
            if(key == 's')
                flag = 'd';
            else
                flag = 'w';
        }
        if(snake.y[0] < food.y )
        {
            if(key == 'w')
            {
                if(snake.x[0] > food.x)
                    flag = 'a';
                else
                    flag = 'd';
            }
            else
                flag = 's';
        }
        if(snake.y[0] == food.y)        //实现了蛇头、食物同行(y)
        {
            if(snake.x[0] < food.x)
            {
                if(key == 'a')
                    flag = 'w';
                else if(key != 'd')
                    flag = 'd';
            }
            if(snake.x[0] > food.x)
            {
                if(key == 'd')
                    flag = 'w';
                else if(key != 'a')
                    flag = 'a';
            }
        }

         switch(flag)
        {
        case 'a':
        case 'A':
            for(i = 1; i < snake.len; i++)
            {
                if(snake.x[0] == snake.x[i] + 2 && snake.y[0] == snake.y[i])
                {
                    for(i = 1; i < snake.len; i++)
                    {
                        if(snake.x[i] == snake.x[0] && snake.y[i] != snake.y[0] + 1 && snake.y[i] != snake.y[0] - 1)
                        {
                             if(snake.y[0] < snake.y[i])             //判断蛇头这一列是否还有其他的块（身体）
                                flag = 'w';
                            else
                                flag = 's';
                            break;

                        }
                        if(snake.x[i] == snake.x[0] && snake.y[i] != snake.y[0] + 1)
                        {
                            flag = 's';
                            break;
                        }

                        if(snake.x[i] == snake.x[0])
                        {
                            flag = 'w';
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'd':
        case 'D':
            for(i = 1; i < snake.len; i++)
            {
                if(snake.x[0] == snake.x[i] - 2 && snake.y[0] == snake.y[i])
                {
                    for(i = 1; i < snake.len; i++)
                    {
                        if(snake.x[i] == snake.x[0] && snake.y[i] != snake.y[0] + 1 && snake.y[i] != snake.y[0] - 1)
                        {
                            if(snake.y[0] < snake.y[i])             //判断蛇头这一列是否还有其他的块（身体）
                                flag = 'w';
                            else
                                flag = 's';
                            break;

                        }
                        if(snake.x[i] == snake.x[0] && snake.y[i] != snake.y[0] + 1)
                        {
                            flag = 's';
                            break;
                        }

                        if(snake.x[i] == snake.x[0])
                        {
                           flag = 'w';
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'w':
        case 'W':
            for(i = 1; i < snake.len; i++)
            {
                if(snake.y[0] == snake.y[i] + 1 && snake.x[0] == snake.x[i])
                {
                    for(i = 1; i < snake.len; i++)
                    {
                        if(snake.y[i] == snake.y[0] && snake.x[i] != snake.x[0] - 2 && snake.x[i] != snake.x[0] + 2)
                        {
                            if(snake.x[0] < snake.x[i])             //判断蛇头这一列是否还有其他的块（身体）
                                flag = 'a';
                            else
                                flag = 'd';
                            break;

                        }
                        if(snake.y[i] == snake.y[0] && snake.x[i] != snake.x[0] - 2)
                        {
                            flag = 'a';
                            break;
                        }
                        if(snake.y[i] == snake.y[0])
                        {
                             flag = 'd';
                             break;
                        }
                    }
                    break;
                }
            }
            break;
        case 's':
        case 'S':
            for(i = 1; i < snake.len; i++)
            {
                if(snake.y[0] == snake.y[i] - 1 && snake.x[0] == snake.x[i])
                {
                    for(i = 1; i < snake.len; i++)
                    {
                        if(snake.y[i] == snake.y[0] && snake.x[i] != snake.x[0] - 2 && snake.x[i] != snake.x[0] + 2)
                        {
                            if(snake.x[0] < snake.x[i])             //判断蛇头这一列是否还有其他的块（身体）
                                flag = 'a';
                            else
                                flag = 'd';
                            break;
                        }
                        if(snake.y[i] == snake.y[0] && snake.x[i] != snake.x[0] - 2)
                        {
                            flag = 'a';
                            break;
                        }

                        if(snake.y[i] == snake.y[0])
                        {
                             flag = 'd';
                             break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'p':
        case 'P':
            system("pause");
            break;
        }
        //避免蛇向相反的方向走
        if(flag - key != 4 && flag - key != -4 && flag - key != 3 && flag - key != -3 && flag != 'p' && flag != 'P')
         {
             key = flag;
             break;
         }
    }
}

//光标移动
void gotoxy(int x, int y)
{
    //找到控制台的这个窗口
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    //光标的结构体
    COORD coord;
    //设置坐标
    coord.X = x;
    coord.Y = y;
    //同步到控制台
    SetConsoleCursorPosition(handle, coord);
}

void ColorPrint(char *m, char n)
{
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, n);
    printf("%s", m);
    SetConsoleTextAttribute(hOut, 7);
}

void DrawRoad()
{

}































