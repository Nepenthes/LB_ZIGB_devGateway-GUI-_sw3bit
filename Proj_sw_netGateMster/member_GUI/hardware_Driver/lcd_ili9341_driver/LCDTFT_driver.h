#ifndef __LCDTFT_DRIVER_H_
#define __LCDTFT_DRIVER_H_

/**********************************************************
                     外部函数头文件
         应用到不同的外设头文件请在这里修改即可                        
**********************************************************/

#include "stm32f10x.h"
#include "IO_Map.h"

/**********************************************************
                    控制LCD相关的宏定义
**********************************************************/


/**********************************************************
                    定义背光接口信息
               不同的控制接口请修改这里                       
**********************************************************/

#define LCD_BLControl               PAout(1)    //背光控制

//                                      GPIO时钟        GPIO组   引脚号     翻转速度       输入输出模式
#define LCD_BLInit					RCC_APB2Periph_GPIOA,GPIOB,GPIO_Pin_1,GPIO_Speed_50MHz,GPIO_Mode_Out_PP

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

//=========================================================
//使用GPIO控制
//=========================================================

	/* 具体需要编写lcd_drive.c里面的LCD_GPIO_Init()这个函数完成IO的初始化处理 */

	/* 根据实际连接进行改写下面宏定义 */
	//                                      GPIO时钟        GPIO组   引脚号     翻转速度       输入输出模式
	#define LCD_RSInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_8,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_CSInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_9,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_RDInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_6,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_WRInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_7,GPIO_Speed_50MHz,GPIO_Mode_Out_PP

	/*******硬件相关的宏定义*******
	LCD_RST---> PC10  复位
	LCD_CS ---> PC9   片选
	LCD_RS ---> PC8   数据/命令
	LCD_WR ---> PC7   写数据
	LCD_RD ---> PC6   读数据
	**********/
	#define	LCD_RST_SET	GPIO_SetBits(GPIOC,GPIO_Pin_10)		//读数据			PC10
	#define	LCD_CS_SET  GPIO_SetBits(GPIOC,GPIO_Pin_9)		//片选端口  		PC9
	#define	LCD_RS_SET	GPIO_SetBits(GPIOC,GPIO_Pin_8)		//数据/命令 		PC8	   
	#define	LCD_WR_SET	GPIO_SetBits(GPIOC,GPIO_Pin_7)		//写数据			PC7
	#define	LCD_RD_SET	GPIO_SetBits(GPIOC,GPIO_Pin_6)		//读数据			PC6
	
	#define	LCD_RST_CLR  GPIO_ResetBits(GPIOC,GPIO_Pin_10)	//片选端口  		PC10
	#define	LCD_CS_CLR  GPIO_ResetBits(GPIOC,GPIO_Pin_9)	//片选端口  		PC9
	#define	LCD_RS_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_8)	//数据/命令			PC8	   
	#define	LCD_WR_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_7)	//写数据			PC7
	#define	LCD_RD_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_6)	//读数据			PC6  

	//PB0~15,作为数据线
	#define DATAOUT(x)	GPIO_Write(GPIOB,x);
	#define DATAIN		GPIO_ReadInputData(GPIOB)
	
	/* 读取数据时设置数据总线方向为输入 */
	#define LCD_BUS_IN16()		{GPIOB->CRL = 0x88888888;GPIOB->CRH = 0x88888888;GPIOB->ODR=0xffff;}
	
	/* 读取数据时设置数据总线方向为输入 */
	#define LCD_GRAM_IN16()		{GPIOB->CRL = 0x88888888;GPIOB->CRH = 0x88888888;GPIOB->ODR=0xffff;}

	/* 写入数据时设置数据总线方向为输出 */
	#define LCD_BUS_OUT16()		{GPIOB->CRL = 0x33333333;GPIOB->CRH = 0x33333333;GPIOB->ODR=0Xffff;}


/**********************************************************
                       外部功能函数                      
**********************************************************/
	
/**********************************************************
* 宏功能 ---> 写数据到LCD RAM
* 入口参数 ---> dat：要写入的数据
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
#define LCD_WR_Data(val)	{\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(val);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
}

void LCD_GPIO_Init(void);     //通讯、控制端口初始化

/* 下面通用外调函数 */
void LCD_RST_BL_Init(void);   //背光、复位控制端口初始化     
void LCD_Write_Register(u16 reg_val);     //写数据到LCD寄存器（主要写命令或寄存器地址）
void LCD_Write_Data(u16 dat); //写数据到LCD RAM
u16 LCD_Read_Data(void);      //读取LCD数据
void LCD_WriteRegisterData(u16 reg, u16 dat);   //向LCD某个寄存器写入数据
u16 LCD_ReadRegisterData(u16 reg);  //读取LCD某个寄存器数值

u16 ili9341_readPoint(u16 x, u16 y);

void LCD_Delay(u8 i);

#endif

