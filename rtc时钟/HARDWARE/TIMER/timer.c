#include "timer.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "page.h"
#include "remote.h"
#include "oled.h"

u8 key = 0;
extern u8 key2;
u8 flag;        //用于长按检测
u8 count2 = 0;  //用于长按检测
u8 count = 0;
u8 direc = 4;      //最新的方向数据
u8 old = 4;        //上一次的方向数据
u16* ps2_val;
static u8 ps2_flag = 1;

vu8 frame_cnt; 
//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		   	    
	//OSIntEnter();        
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)//溢出中断
	{
        //修改时间
        //key = KEY_Scan(0);
        key2 = KEY_Scan(1);
        ps2_val = PS2_Scan();
        if(key2 == WKUP_PRES)
            flag = 1;
        else 
            flag = 0;
        if(flag == 1 && key2 == WKUP_PRES)
            count2++;
        else 
            count2 = 0;
        if(count2 >= 60)
        {
            OLED_Clear();
            SetTime();            
        }   
        
        if(ps2_flag == 0)
            key = 0;
        //判断ps2遥感的动作
        if(ps2_flag && ps2_val[2] == 0) 
        {
            key = KEY1_PRES;
            direc = 0;    
        } 
        if(ps2_flag && (ps2_val[0] > 200 || ps2_val[0] < 130 || ps2_val[1] > 200 || ps2_val[1] < 130))
        {
            ps2_flag = 0;
            if(ps2_val[0] > ps2_val[1])  //y轴数据大于x轴  y轴向下 或 x轴向右
            {
                if(ps2_val[0] > 200)   //向下
                {
                    key = WKUP_PRES;
                     direc = 2;
                }
                else                   //向右
                    direc = 4;
            }
            if(ps2_val[0] < ps2_val[1])
            {
                if(ps2_val[0] < 130)   //向上
                {
                    key = KEY0_PRES;
                    direc = 1;
                }
                else                   //向左
                    direc = 3;    
            }
        }
        if(ps2_val[0] < 170 && ps2_val[0] > 160 && ps2_val[1] > 160 && ps2_val[1] < 170)
        {
             key = 0 ;
             ps2_flag = 1;
        }
            

        //为food坐标提供随机数
        count++;
        if(count >= 250)
            count = 0;

	}				   
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位 	 			    
	//OSIntExit();  											 
}

u8 GetDirection(void)
{
//    if(direc == 0);
//    else if(old == 0);             
//    else if((old + direc == 3) || (direc + old == 7))
//        direc = old;    //方向不变，防止蛇头缩到身体里了
    return direc;    //在这里，如果direc更新了，就起不到判断方向可行性的功能了
}

u8 GetRandom(u8 min, u8 max)
{
    u8 result;
    result = (count % (max - min + 1)) + min;
    return result;
}

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
	
}
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3											  
}  	 



