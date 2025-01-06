//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F407ZGT6,正点原子Explorer STM32F4开发板,主频168MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 FSMC
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VDD          接        DC5V/3.3V        //电源
//      GND          接          GND            //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口总线
//     LCD模块                STM32单片机      _
//			 DB0          接          PD14           |
//			 DB1          接          PD15           | 
//       DB2          接          PD0            |
//       DB3          接          PD1            |
//       DB4          接          PE7            |
//       DB5          接          PE8            |
//       DB6          接          PE9            |
//       DB7          接          PE10           |==>>液晶屏16位并口数据信号
//       DB8          接          PE11           | 
//       DB9          接          PE12           |
//       DB10         接          PE13           |
//       DB11         接          PE14           |
//       DB12         接          PE15           |
//       DB13         接          PD8            |
//       DB14         接          PD9            |
//       DB15         接          PD10          _|
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 				        STM32单片机 
//       WR          接          PD5           //液晶屏写数据控制信号
//       RD          接          PD4           //液晶屏读数据控制信号
//       RS          接          PF12          //液晶屏数据/命令控制信号
//       RST         接        复位引脚        //液晶屏复位控制信号
//       CS          接          PG12          //液晶屏片选控制信号
//       BL          接          PB15          //液晶屏背光控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//       PEN         接          PB1           //触摸屏触摸中断信号
//       MISO        接          PB2           //触摸屏SPI总线读信号
//       MOSI        接          PF11          //触摸屏SPI总线写信号
//       T_CS        接          PC13          //触摸屏片选控制信号
//       CLK         接          PB0           //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/		
#include "touch.h" 
#include "lcd.h"
#include "tim_Delay.h"
#include "stdlib.h"
#include "math.h"

	

_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	0,
	0,
	0,
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

/*****************************************************************************
 * @name       :void TP_Write_Byte(u8 num)   
 * @date       :2018-08-09 
 * @function   :Write a byte data to the touch screen IC with SPI bus
 * @parameters :num:Data to be written
 * @retvalue   :None
******************************************************************************/  	 			    					   
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 	 
		tim6_delay_us(1);
		TCLK=1;		//上升沿有效	        
	}		 			    
}

/*****************************************************************************
 * @name       :u16 TP_Read_AD(u8 CMD)	  
 * @date       :2018-08-09 
 * @function   :Reading adc values from touch screen IC with SPI bus
 * @parameters :CMD:Read command,0xD0 for x,0x90 for y
 * @retvalue   :Read data
******************************************************************************/    
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//先拉低时钟 	 
	TDIN=0; 	//拉低数据线
	TCS=0; 		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	tim6_delay_us(6);//ADS7846的转换时间最长为6us
	TCLK=0; 	     	    
	tim6_delay_us(1);    	   
	TCLK=1;		//给1个时钟，清除BUSY	    	    
	tim6_delay_us(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	//下降沿有效  	    	   
		tim6_delay_us(1);    
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//只有高12位有效.
	TCS=1;		//释放片选	 
	return(Num);  
//#endif
}

#define READ_TIMES 5 	//读取次数
#define LOST_VAL 1	  	//丢弃值
/*****************************************************************************
 * @name       :u16 TP_Read_XOY(u8 xy)  
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates (x or y),
								Read the READ_TIMES secondary data in succession 
								and sort the data in ascending order,
								Then remove the lowest and highest number of LOST_VAL 
								and take the average
 * @parameters :xy:Read command(CMD_RDX/CMD_RDY)
 * @retvalue   :Read data
******************************************************************************/  
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

/*****************************************************************************
 * @name       :u8 TP_Read_XY(u16 *x,u16 *y)
 * @date       :2018-08-09 
 * @function   :Read touch screen x and y coordinates,
								The minimum value can not be less than 100
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}

#define ERR_RANGE 50 //误差范围 
/*****************************************************************************
 * @name       :u8 TP_Read_XY2(u16 *x,u16 *y) 
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates twice in a row, 
								and the deviation of these two times can not exceed ERR_RANGE, 
								satisfy the condition, then think the reading is correct, 
								otherwise the reading is wrong.
								This function can greatly improve the accuracy.
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 



/*****************************************************************************
 * @name       :u8 TP_Scan(u8 tp)
 * @date       :2018-08-09 
 * @function   :Scanning touch event				
 * @parameters :tp:0-screen coordinate 
									 1-Physical coordinates(For special occasions such as calibration)
 * @retvalue   :Current touch screen status,
								0-no touch
								1-touch
******************************************************************************/  					  
u8 TP_Scan(u8 tp)
{			   
	if(PEN==0)//有按键按下
	{
		if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);//读取物理坐标
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))//读取屏幕坐标
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;//将结果转换为屏幕坐标
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//之前没有被按下
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//按键按下  
			tp_dev.x0=tp_dev.x;//记录第一次按下时的坐标
			tp_dev.y0=tp_dev.y;  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);//标记按键松开	
		}else//之前就没有被按下
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//返回当前的触屏状态
}
	  
//////////////////////////////////////////////////////////////////////////	 
//保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
/*****************************************************************************
 * @name       :void TP_Save_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Save calibration parameters		
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 										    

	

/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
								1-Has been calibrated
******************************************************************************/  
u8 TP_Init(void)
{			    		   
	//注意,时钟使能之后,对GPIO的操作才有效
	//所以上拉之前,必须使能时钟.才能实现真正的上拉输出
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOB,C,F时钟

	//GPIOB1,2初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;//PB1/PB2 设置为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB0设置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13设置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PF11设置推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化	
		
	TP_Read_XY(&tp_dev.x,&tp_dev.y);//第一次读取初始化	 


	tp_dev.xfac=0.063532;tp_dev.yfac=-0.111628;tp_dev.xoff=-11;tp_dev.yoff=439;//写死校准参数。

		
	return 1; 									 
}


