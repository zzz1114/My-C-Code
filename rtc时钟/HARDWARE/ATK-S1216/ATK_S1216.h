#ifndef _ATK_S1216_H
#define _ATK_S1216_H
#include "sys.h"      
//GPS NMEA-0183 协议重要参数结构体定义
//卫星信息
__packed typedef struct 
{ 
    u8 num; //卫星编号
    u8 eledeg; //卫星仰角
    u16 azideg; //卫星方位角
    u8 sn; //信噪比 
}nmea_slmsg; 
//北斗 NMEA-0183 协议重要参数结构体定义
//卫星信息
__packed typedef struct 
{
    u8 beidou_num; //卫星编号
    u8 beidou_eledeg; //卫星仰角
    u16 beidou_azideg;//卫星方位角
    u8 beidou_sn; //信噪比 
}beidou_nmea_slmsg; 
//UTC 时间信息
__packed typedef struct 
{ 
    u16 year; //年份
    u8 month; //月份
    u8 date; //日期
    u8 hour; //小时
    u8 min; //分钟
    u8 sec; //秒钟
}nmea_utc_time; 
//NMEA 0183 协议解析后数据存放结构体
__packed typedef struct 
{ 
    u8 svnum; //可见 GPS 卫星数
    u8 beidou_svnum; //可见北斗卫星数
    nmea_slmsg slmsg[12]; //最多 12 颗 GPS 卫星
    beidou_nmea_slmsg beidou_slmsg[12]; //暂且算最多 12 颗北斗卫星
    nmea_utc_time utc; //UTC 时间
    u32 latitude; //纬度 分扩大 100000 倍,实际要除以 100000
    u8 nshemi; //北纬/南纬,N:北纬;S:南纬 
    u32 longitude; //经度 分扩大 100000 倍,实际要除以 100000
    u8 ewhemi; //东经/西经,E:东经;W:西经
    u8 gpssta;
    //GPS 状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算. 
    u8 posslnum; //用于定位的 GPS 卫星数,0~12.
    u8 possl[12]; //用于定位的卫星编号
    u8 fixmode; //定位类型:1,没有定位;2,2D 定位;3,3D 定位
    u16 pdop; //位置精度因子 0~500,对应实际值 0~50.0
    u16 hdop; //水平精度因子 0~500,对应实际值 0~50.0
    u16 vdop; //垂直精度因子 0~500,对应实际值 0~50.0 
    int altitude; //海拔高度,放大了 10 倍,实际除以 10.单位:0.1m
    u16 speed;
    //地面速率,放大了 1000 倍,实际除以 10.单位:0.001 公里/小时
}nmea_msg;
////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 配置波特率结构体
__packed typedef struct
{
    u16 sos; //启动序列，固定为 0XA0A1
    u16 PL; //有效数据长度 0X0004；
    u8 id; //ID，固定为 0X05
    u8 com_port; //COM 口，固定为 0X00，即 COM1 
    u8 Baud_id; 
    //波特率（0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600）
    u8 Attributes; 
    //配置数据保存位置 ,0 保存到 SRAM，1 保存到 SRAM&FLASH，2 临时保存
    u8 CS; //校验值
    u16 end; //结束符:0X0D0A 
}SkyTra_baudrate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 
//SkyTra S1216F8-BD 配置输出信息结构体
__packed typedef struct
{
    u16 sos; //启动序列，固定为 0XA0A1
    u16 PL; //有效数据长度 0X0009；
    u8 id; //ID，固定为 0X08
    u8 GGA; //1~255（s）,0:disable
    u8 GSA; //1~255（s）,0:disable
    u8 GSV; //1~255（s）,0:disable
    u8 GLL; //1~255（s）,0:disable
    u8 RMC; //1~255（s）,0:disable
    u8 VTG; //1~255（s）,0:disable
    u8 ZDA; //1~255（s）,0:disable
    u8 Attributes; 
    //配置数据保存位置 ,0 保存到 SRAM，1 保存到 SRAM&FLASH，2 临时保存
    u8 CS; //校验值
    u16 end; //结束符:0X0D0A 
}SkyTra_outmsg;
//////////////////////////////////////////////////////////////////////////////////////////////////// 
//SkyTra S1216F8-BD 配置位置更新率结构体
__packed typedef struct
{
    u16 sos; //启动序列，固定为 0XA0A1
    u16 PL; //有效数据长度 0X0003；
    u8 id; //ID，固定为 0X0E
    u8 rate; //取值范围:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
    u8 Attributes; 
    //配置数据保存位置 ,0 保存到 SRAM，1 保存到 SRAM&FLASH，2 临时保存
    u8 CS; //校验值
    u16 end; //结束符:0X0D0A 
}SkyTra_PosRate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 
//SkyTra S1216F8-BD 配置输出脉冲(PPS)宽度结构体
__packed typedef struct
{
    u16 sos; //启动序列，固定为 0XA0A1
    u16 PL; //有效数据长度 0X0007；
    u8 id; //ID，固定为 0X65
    u8 Sub_ID; //0X01
    u32 width; //1~100000(us)
    u8 Attributes; 
    //配置数据保存位置 ,0 保存到 SRAM，1 保存到 SRAM&FLASH，2 临时保存
    u8 CS; //校验值
    u16 end; //结束符:0X0D0A 
}SkyTra_pps_width;
//////////////////////////////////////////////////////////////////////////////////////////////////// 
//SkyTra S1216F8-BD ACK 结构体
__packed typedef struct
{
    u16 sos; //启动序列，固定为 0XA0A1
    u16 PL; //有效数据长度 0X0002；
    u8 id; //ID，固定为 0X83
    u8 ACK_ID; 
    u8 CS; //校验值
    u16 end; //结束符
}SkyTra_ACK;
//////////////////////////////////////////////////////////////////////////////////////////////////// 
//SkyTra S1216F8 -BD NACK 结构体
__packed typedef struct
{
    u16 sos; //启动序列，固定为 0XA0A1
    u16 PL; //有效数据长度 0X0002；
    u8 id; //ID，固定为 0X84
    u8 NACK_ID; 
    u8 CS; //校验值
    u16 end; //结束符
}SkyTra_NACK;


int NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_BDGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf);
u8 SkyTra_Cfg_Prt(u32 baud_id);
u8 SkyTra_Cfg_Tp(u32 width);
u8 SkyTra_Cfg_Rate(u8 Frep);
void SkyTra_Send_Date(u8* dbuf,u16 len);

#endif
