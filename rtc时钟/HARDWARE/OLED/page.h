#ifndef _PAGE_H
#define _PAGE_H

//这个是管理olde显示页面的文件，包括页面、控件结构体等
//只是构建出一个页面的基本框架
#include "sys.h"
#include "oled.h"
#include "key.h"

#define ShowStyle_normal 0      //正常
#define ShowStyle_hifglight 1   //反白


typedef struct Page
{ 
    u8 id;          //页面id    
    u8* txt;        //储存一个页面内所有汉字对应的下标
    u8* num;        //储存每一个字符串的汉字数量
    u8* num2;       //num数组各个值累加  累加值，方便显示,例如字数都是2，则为0, 2. 4。。。
    u8* coorX;      //储存各个字符串的坐标
    u8* coorY;  
    u8 ContrNum;    //控件数量
    u8 choices;     //页面提供的交互控件数量  小于等于num    
}Page;

void SetTime(void);     //设置时间
void Page_main(Page* page); //主页面
void ShowPage(Page* page);  

#endif
