#ifndef _SNAKE_H
#define _SNAKE_H

#include "sys.h"

#define OK 1    //正常状态
#define ERRO 0  //非法状态，游戏结束

#define Move_Up     1     // 蛇移动方向
#define Move_Down   2
#define Move_Left   3
#define Move_Right  4


#define MAP_Witdh   32
#define MAP_Longth  64

typedef struct Coordinate
{
    u8 x;
    u8 y;
}Coordinate;

typedef struct SnakeNode       //蛇节点
{
    Coordinate Coor;           //坐标
    struct SnakeNode *next;    //指向下一节点
}SnakeNode, * SnakePtr;

typedef struct Snake
{
    SnakePtr front;             // 蛇头
    SnakePtr rear;              // 蛇尾
    u8 length;                  //长度
}Snake;

typedef struct Food
{
    Coordinate Coor;
}Food;

u8 SnakeMove(Snake * snake,Food* food, u8 dir); 

u8 PlaySnake(void);                 //顶层函数，贪吃蛇游戏死循环
void DrawWall(void); 
void DestroySnake(Snake * snake); 
void lost(Snake* snake, Food* food);
void CreatFood(Snake* snake, Food* food);
void Snake_Init(Snake * snake, Food* food);
void InsertFront(Snake * snake, Coordinate * coor);

#endif
