#include "page.h"
#include "rtc.h"
#include "key.h" 
#include "led.h"
#include "delay.h"
#include <stdlib.h>


//用于oled显示管理
static u8 pageID = 0;
extern Page pageMain;

void SetTime()     //设置时间  还有一些问题，一些小bug 修改延时太长
{
    u8* val;         //记录时分秒数值
    u8 temp = 0;    //当前正在修改的属性 年5，月4，日3, 时2，分1，秒0
    u8 key;
    u8 key2;    //用于检测按键长按
    u8 flag = 0;//用于检测长按
    u16 count2 = 0;//用于检测长按
    u16 count = 0;
    OLED_ShowString(5, 0, "Setting time...", 0);
    while(1)
    { 
        val = RTC_GetTime();
        if(count > 250)
            OLED_ShowNum((44 - temp * 22 + calendar.x), calendar.y, val[temp], 2, 16, 1, 1);
        else
            OLED_ShowTime(calendar.x, calendar.y, calendar.hour, calendar.min, calendar.sec);  
        key = KEY_Scan(0);
        key2 = KEY_Scan(1);
        
        if(key == KEY0_PRES)
        {
            temp++;
            if(temp >= 3)
                temp = 0;
        }     
        else if(key == WKUP_PRES)
        {
            if(temp == 0)
                temp = 3;    //暂时只支持修改时分秒
            temp--;
            
        }
        else if(key == KEY1_PRES)   //进入修改
        {
            while(1)
            {
                if(count > 200)
                    OLED_ShowNum((44 - temp * 22 + calendar.x), calendar.y, val[temp], 2, 16, 1, 1);
                else 
                    OLED_ShowNum((44 - temp * 22 + calendar.x), calendar.y, val[temp], 2, 16, 1, 0);                    
                    //OLED_ShowTime(calendar.x, calendar.y, calendar.hour, calendar.min, calendar.sec);
                key = KEY_Scan(0);
                
                if(key == KEY0_PRES)
                {
                    val[temp]++;
                    if(val[temp] >= 60 || (temp == 2 && val[temp] >= 24))
                        val[temp] = 0;
                }     
                else if(key == WKUP_PRES)
                {
                    if(val[temp] == 0)
                    {
                        if(temp == 2)
                            val[temp] = 24;
                        else
                            val[temp] = 60;    //暂时只支持修改时分秒                        
                    }
                    val[temp]--;
                }
                else if(key == KEY1_PRES)   //退出修改 
                {
                    RTC_Set(calendar.year, calendar.month, calendar.date, val[2], val[1], val[0]);
                    break;
                }                    
                                        
                count++;
                if(count >= 500)
                    count = 0;
            }
        }  
        if(key2 == KEY0_PRES)
            flag = 1;
        else 
            flag = 0;
        if(flag == 1 && key2 == KEY0_PRES)
            count2++;
        else
            count2 = 0;
        if(count2 >= 100)
        {
            OLED_Clear();
            ShowPage(&pageMain);
            break;            
        }

        
        count++;
        if(count >= 500)
        {
            count = 0; 
           LED1 = ~LED1;
        }
    }
}

u8 tx[4] = {21, 22, 23, 24};       //菜单  测试
u8 X[2] = {0, 96};        //坐标：
u8 Y[2] = {6, 6};
u8 Num[2] = {2, 2};      //字数：
u8 Num2[2] = {0, 2};

//初始化页面的模板
//void Page(Page* page)
//{
//    u8 tx[] = {};       //内容
//    u8 X[] = {};        //坐标：
//    u8 Y[] = {};
//    u8 Num[] = {};      //字数：
//    u8 Num2[] = {};
//    page = (Page*)malloc(sizeof(Page));
//    page->choices = 2;
//    page->ContrNum = 2;
//    page->txt = tx;
//    page->coorX = X;
//    page->coorY = Y;
//    page->num = Num;
//    page->num2 = Num2;
//    page->id = pageID++; 
//}

void Page_main(Page* page)
{
    page->choices = 2;           //不能把局域变量赋值给结构体，当函数结束后，局域变量的空间被释放，指针指向的值就不确定了
    page->ContrNum = 2;
    page->txt = tx;
    page->coorX = X;
    page->coorY = Y;
    page->num = Num;
    page->num2 = Num2;
    page->id = pageID++; 
}



void ShowPage(Page* page)
{
    u8 i;
    OLED_Clear();
    
    for(i = 0; i < 2; i++)  //(page->ContrNum)
    {
        OLED_ShowWord(page->coorX[i], page->coorY[i], page->txt + page->num2[i], page->num[i], 0);
        //OLED_ShowWord(page->coorX[i], 6, page->num2[i] + page->txt, 2, 0);     //page->coorY[i]  page->num[i]
    }
}








