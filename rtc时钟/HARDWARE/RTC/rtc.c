#include "delay.h"
#include "led.h"
#include "rtc.h"

_calenda_obj calendar;

u8 Is_Leap_Year(u16 year)
{
    if(((year / 4 == 0) && (year / 100 != 0)) || ((year / 100 == 0) && (year / 400 == 0)))
        return 1;
    return 0;
}


u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}

u8 RTC_Get()
{
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;
    timecount = RTC_GetCounter();
    temp = timecount / 86400;   //天数
    if(daycnt != temp)  //超过一天
    {
        daycnt = temp;
        temp1 = 1970;   //从1970年开始
        while(temp >= 365)
        {
             if(Is_Leap_Year(temp1))
             {
                 if(temp >= 366)   temp -= 366; //闰年多一天
                 else 
                 {
                     temp1++;
                     break;
                 }
             }
             else temp -= 365;
             temp1++;   //年份加一年；
        }
        calendar.year = temp1;//得到年份
        
        temp1 = 0;
        while(temp >= 28)
        {
            if(Is_Leap_Year(calendar.year) && temp1 == 1)
            {
                if(temp >= 29) temp -= 29;
                else break;
            }
            else 
            {
                if(temp >= mon_table[temp1]) temp -= mon_table[temp1];
                else break;
            }
            temp1++;
        }
        calendar.month = temp1;     //得到月份
        calendar.date = temp;       //得到日期
    }
    
    temp = timecount % 86400;   //秒钟数
    calendar.hour = temp / 3600;
    calendar.min = (temp % 3600) / 60;
    calendar.sec = (temp % 3600) % 60;
    calendar.week=RTC_Get_Week(calendar.year,calendar.month,calendar.date);//获取星期
    return 0;
}

u8* RTC_GetTime(void)
{
    static u8 time[3];
    time[0] = calendar.sec;
    time[1] = calendar.min;
    time[2] = calendar.hour;
    return time;
}

u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if( syear < 1970 || syear >2099)  return 1; 
    for( t = 1970; t < syear; t++)                   //把所有年份的秒钟相加
    { 
        if(Is_Leap_Year(t)) seccount += 31622400;   //闰年的秒钟数
        else seccount += 31536000;                  //平年的秒钟数
    }
    smon -= 1;
    for( t = 0; t < smon; t++) //把前面月份的秒钟数相加
    { 
        seccount += (u32)mon_table[t] * 86400; //月份秒钟数相加
        if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400;//闰年 2 月份增加一天的秒钟数
     
    }
    seccount += (u32)(sday - 1) * 86400;    //把前面日期的秒钟数相加
    seccount += (u32)hour * 3600;           //小时秒钟数
    seccount += (u32)min * 60;              //分钟秒钟数
    seccount += sec;                        //最后的秒钟加上去
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能 PWR 和 BKP 外设时钟 
    PWR_BackupAccessCmd(ENABLE);            //使能 RTC 和后备寄存器访问
    RTC_SetCounter(seccount);               //设置 RTC 计数器的值
    RTC_WaitForLastTask();                  //等待最近一次对 RTC 寄存器的写操作完成 
    return 0; 
}

u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount = 0;
	if(syear < 1970 || syear > 2099)    return 1;	   
	for( t = 1970; t < syear; t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))  seccount += 31622400;//闰年的秒钟数
		else seccount += 31536000;			  //平年的秒钟数
	}
	smon -= 1;
	for( t = 0;t < smon; t++)	   //把前面月份的秒钟数相加
	{                    
		seccount += (u32)   mon_table[t] * 86400;//月份秒钟数相加
		if(Is_Leap_Year(syear) && t == 1)   seccount += 86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount += (u32)(sday - 1) * 86400;//把前面日期的秒钟数相加 
	seccount += (u32)hour * 3600;//小时秒钟数
    seccount += (u32)min * 60;	 //分钟秒钟数
	seccount += sec;//最后的秒钟加上去 			    
	//设置时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	//上面三步是必须的!
	
	RTC_SetAlarm(seccount);
 
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	    
}

static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

u8 RTC_Init(void)
{
    u8 temp = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);   //使能电源时钟,以及备份区域时钟
    PWR_BackupAccessCmd(ENABLE);    //允许访问备份区域
    
    if(BKP_ReadBackupRegister(BKP_DR1) != 0x5050)     //如果是第一次配置
    {
        BKP_DeInit();                   //备份区域初始化
        RCC_LSEConfig(RCC_LSE_ON);      //设置成外部低速晶振
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250) //等晶振就绪
        {
            temp++;
            delay_ms(10);
        }
        if(temp >= 250) return 1;               //超时，说明晶振有问题
        
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟 
        RCC_RTCCLKCmd(ENABLE);  //使能rtc时钟
        RTC_WaitForLastTask();  //等待最后一次 写rtc操作完成
        RTC_WaitForSynchro();   //等待 rtc同步
        
        RTC_ITConfig(RTC_IT_SEC, ENABLE);   //使能秒中断
        RTC_WaitForLastTask();
        
        RTC_EnterConfigMode();   //允许配置，进入配置模式
        RTC_SetPrescaler(32767);  //设置rtx预分频值
        RTC_WaitForLastTask();
        
        RTC_Set(2020, 2, 24, 13, 35, 24);    //设置默认时间
        
        RTC_ExitConfigMode();   //退出配置模式
        BKP_WriteBackupRegister(BKP_DR1, 0x5050);
        
        
    }
    else
    {
        RTC_WaitForLastTask();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
    
    RTC_NVIC_Config();  //设置中断分组
    RTC_Get();          //更新时间
    
    calendar.x = Show_X;
    calendar.y = Show_Y;
    
    return 0;           //初始化成功
}

void RTC_IRQHandler(void)  
{
    if(RTC_GetFlagStatus(RTC_IT_SEC) != RESET)      //秒中断
    {
        
        RTC_Get();            //更新时间
    }
    if(RTC_GetFlagStatus(RTC_IT_ALR) != RESET)  //闹钟中断
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);  //闹钟中断清零
        RTC_Get();            //更新时间
    }
    
    RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);  //清秒中断
    
    RTC_WaitForLastTask();
}










