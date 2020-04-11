#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 		 	 
#include "oled.h"	
#include "key.h" 	 
#include "string.h"
#include "snake.h"
#include "timer.h"
#include "rtc.h"
#include "page.h"
#include "remote.h"


u8 key2;    //用于长按检测 
Page pageMain;
extern u16 ADCConvertedValue[10][2];
extern u16* ps2_val;

int main(void)
{	 
    u8 count;   //检测长按使用的变量
    //u8 txt_time[2] = {14, 15};
    u8 menu[2 ] = {0, 1};    //游戏 设置 。。
    u8 menu_choce = 0;     //菜单界面，指向子功能，0:游戏
    u8 key;     //用于点按检测
    
    
    
    RTC_Init();
 	delay_init();	    	 //延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
    TIM3_Int_Init(4999, 71);    //5ms
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键 
    OLED_Init();
    OLED_Clear();
    
    Page_main(&pageMain);
    ShowPage(&pageMain);
    
    AdcInit();
	DMAInit();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//开始采集    
       
    while(1)
    {
        key = KEY_Scan(1);
        //PlaySnake();       
        if(key == KEY1_PRES)   //进入菜单页面
        {
            OLED_Clear();
            key = 0;
            while(1)
            {
                key = KEY_Scan(0);
                key2 = KEY_Scan(1); 
                OLED_ShowWord(48, 6, menu, 2, 0); //先只显示文字，图像部分先不搞
                
                if(key2 == KEY0_PRES)
                     count++;
                else 
                    count = 0;
                if(count >= 20)
                {
                    OLED_Clear();
                    ShowPage(&pageMain);
                    break;                
                }
                if(key == KEY0_PRES)
                {
                     menu_choce++;
                  
                    //显示别的子功能
                }
                if(key == WKUP_PRES)
                {
                     menu_choce--;
                    //显示别的子功能
                }
                if(key == KEY1_PRES)
                    PlaySnake();    //进入游戏
                

                delay_ms(20);
            }            
        }

        
        OLED_ShowTime(calendar.x, calendar.y, calendar.hour, calendar.min, calendar.sec); 
        LED1 =  ~LED1;
        delay_ms(200);
    }
}

