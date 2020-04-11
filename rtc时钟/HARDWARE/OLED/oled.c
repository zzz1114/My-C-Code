#include "oled.h"
#include "delay.h"
#include "oledfont.h"

u8 OLED_da[8][128] = {0};     //第一维是页， 第二维是横坐标

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i = 0; i < 8; i++)
	{			  
		OLED_SCLK_Clr();
		if(dat & 0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat <<= 1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 

void OLED_WR_Byte2(u8 dat,u8 cmd)     //用于反白显示（cmd 为数据！！）
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i = 0; i < 8; i++)
	{			  
		OLED_SCLK_Clr();
		if(dat & 0x80)
           OLED_SDIN_Clr();  
		else 
		   OLED_SDIN_Set();
		OLED_SCLK_Set();
		dat <<= 1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}

void OLED_Set_Pos(u8 x, u8 y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);    //  y是纵向坐标，即页
    OLED_WR_Byte((x & 0x0f), OLED_CMD);    //列的低四位
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);//高四位 
}

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令  //电荷泵设置
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON       //开启电荷泵
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON    //开启显示
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

void OLED_Clear(void)  //把寄存器所有值清零
{
    u8 i, j;
    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);
        OLED_WR_Byte(0x00, OLED_CMD);
        OLED_WR_Byte(0x10, OLED_CMD);
        for(j = 0; j < 128; j++)
            OLED_WR_Byte(0, OLED_DATA);
    }  
}
void OLED_ShowChar(u8 x, u8 y, u8 ch, u8 style)
{
    u8 c = 0, i = 0;
    c = ch - ' ';
    if(x > 127) 
    {
        x = 0; 
        y = y + 2;
    }
    OLED_Set_Pos(x, y);
    for(i = 0; i < 8; i++)
        if(!style)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        else
            OLED_WR_Byte2(F8X16[c * 16 + i], OLED_DATA); 
    OLED_Set_Pos(x, y + 1);
    for(i = 0; i < 8; i++)
        if(!style)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
        else
            OLED_WR_Byte2(F8X16[c * 16 + i + 8], OLED_DATA);
}

void OLED_ShowString(u8 x, u8 y, u8* stri, u8 style)
{    
    u8* p;
    u8 m, n;
    m = x;
    n = y;
    p = stri;
    while(*p)
    {
        OLED_ShowChar(m, n, *p, style);
        m += 8;
        if(m < 128 && m + 8 > 128)
        {
            m = 0;
            n++;
        }
            
        p++;
    }
}

u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size, u8 mode, u8 style)          //mode是显示模式  0缺位空格补齐， 1缺位 0 补齐
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
                if(!mode)
                    OLED_ShowChar(x+(size/2)*t,y,' ', style);
                else
                    OLED_ShowChar(x+(size/2)*t,y,'0', style);   
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0', style); 
	}
}

//显示汉字
void OLED_ShowChinese(u8 x,u8 y,u8 no, u8 style)    //style显示样式 0正常显示  1反白显示
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
        if(!style)
            OLED_WR_Byte(str[2*no][t],OLED_DATA);
        else
            OLED_WR_Byte2(str[2*no][t],OLED_DATA);
        adder+=1;
    }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
    {	
        if(!style)
            OLED_WR_Byte(str[2*no+1][t],OLED_DATA);
        else
            OLED_WR_Byte2(str[2*no+1][t],OLED_DATA);
        adder+=1;
    }					                                       
}

void OLED_ShowWord(u8 x, u8 y, u8* st, u8 len, u8 style) //显示中文文本  len是字数
{
    u8 temp;
    u8 m, n;
    m = x;
    n = y;
    for(temp = 0; temp < len; temp++)
    {
        OLED_ShowChinese(m, n, st[temp], style);
        m += 16;
        if(m > 128 || m + 16 > 128)
        {
            m = 0;
            n++;
        }
    }
}

void SetData(u8 x, u8 y, u8 dat)       //把坐标上点的数据储存在数据中
{
    u8 m, n;
    m = y / 8;
    n = y % 8;
    if(dat)
    {
        OLED_da[m][x] |= (0x03 << n);
        OLED_da[m][x + 1] |= (0x03 << n);
    }
    else
    {
        OLED_da[m][x] &= ~(0x03 << n);
        OLED_da[m][x + 1] &= ~(0x03 << n);
    }        
}

void OLED_ShowTime(u8 x, u8 y, u8 h, u8 m, u8 s)  //显示时间
{
    OLED_ShowChar(x + 15, y, ':', 0); 
    OLED_ShowChar(x + 37, y, ':', 0);
    OLED_ShowNum(x, y, h, 2, 16, 1, 0); // 16, 1, h, 2, 16
    OLED_ShowNum(x + 22, y, m, 2, 16, 1, 0);
    OLED_ShowNum(x + 44, y, s, 2, 16, 1, 0);  
}

void XY_On(u8 x, u8 y)        
{
    u8 m, n;
    m = y / 8;
    n = y % 8;
    OLED_da[m][x] |= (0x03 << n);       //纵坐标小的是低位
    OLED_da[m][x + 1] |= (0x03 << n);    
    OLED_Set_Pos(x, m);
    OLED_WR_Byte(OLED_da[m][x], OLED_DATA);
    
    OLED_Set_Pos(x + 1, m);
    OLED_WR_Byte(OLED_da[m][x + 1], OLED_DATA);
    
    SetData(x, y, 1);
}

void XY_Off(u8 x, u8 y)
{
    u8 m, n;
    m = y / 8;
    n = y % 8;
    OLED_da[m][x] &= ~(0x03 << n);       //纵坐标小的是低位
    OLED_da[m][x + 1] &= ~(0x03 << n);
    OLED_Set_Pos(x, m);
    OLED_WR_Byte(OLED_da[m][x], OLED_DATA);
    
    OLED_Set_Pos(x + 1, m);
    OLED_WR_Byte(OLED_da[m][x + 1], OLED_DATA); 
    
    SetData(x, y, 0);
}

void OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);       //开启时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
//    OLED_RST_Set();
//	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set();
    delay_ms(100);
					   

    OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(0x50,OLED_CMD); //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0
	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.									    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;   0xA0<-->0xA1  
	OLED_WR_Byte(0xC8,OLED_CMD); //设置COM扫描方向; 0xC0<-->0xC1:垂直反向
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示

	OLED_Clear();
	OLED_Set_Pos(0,0);   
}





















