#include "LCDTFT_driver.h"
#include "delay.h"

/**********************************************************
* 函数功能 ---> STM32 GPIO输出方向初始化
* 入口参数 ---> *STMCapa：容量字符显示缓存
* 返回数值 ---> none
* 功能说明 ---> 按照此函数形参的顺序进行宏定义即可
**********************************************************/
void STM32_GPIOx_Init(
                        uint32_t RCC_APB2Periph,		/* GPIO对应时钟 */
						GPIO_TypeDef* GPIOx,			/* 所对应的GPIO组 */
						uint16_t GPIO_Pinx,				/* GPIO对应的管脚号 */                       
						GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO翻转速度 */
						GPIOMode_TypeDef GPIO_MODE		/* GPIO设置模式 */
					   )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//检测输入参数
	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	assert_param(IS_GPIO_PIN(GPIO_Pinx));
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_SPEED(GPIO_SPEED));
	assert_param(IS_GPIO_MODE(GPIO_MODE)); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);	//开启外设时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE;

	GPIO_Init(GPIOx, &GPIO_InitStructure);	//初始化GPIO相关结构体
}

/**********************************************************
* 函数功能 ---> 通讯、控制端口初始化
* 入口参数 ---> none
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/* GPIO控制LCD控制线等初始化代码 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //使能PORTB,C时钟和AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTC6~10推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//背光
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIO连接LCD数据总线管脚初始化代码 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  PORTB推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
 
	GPIO_SetBits(GPIOB,GPIO_Pin_All);
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

/**********************************************************
* 函数功能 ---> 读取LCD需要的延时
* 入口参数 ---> i：需要延时数值
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_Delay(u8 i)
{
	while(i--);
}

/**********************************************************
* 函数功能 ---> 写数据到总线
* 入口参数 ---> val：要写入的数据
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_Write_Bus(u16 val)
{
/* 使用16bits数据总线宽度时的逻辑控制时序 */

//	LCD_BUS_OUT16();	//设置总线为输出方向
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(val);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}
/**********************************************************
* 函数功能 ---> 读取LCD数据
* 入口参数 ---> none
* 返回数值 ---> 读取到的数据
* 功能说明 ---> none
**********************************************************/
u16 LCD_Read_Data(void)
{
	u16 temp=0;	//接收数据缓存

/* 使用16bits数据总线宽度时的逻辑控制时序 */
	LCD_BUS_IN16();

	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	
//	LCD_RD_SET;
	LCD_RD_CLR;
	
	LCD_Delay(2);
	
	temp=DATAIN;  
	
	LCD_RD_SET;
	LCD_CS_SET; 

	LCD_BUS_OUT16();
	
	return temp;	//返回读取到的数据     
}

u16 ili9341_readPoint(u16 x, u16 y){

	u16 r,g,b;
	
	LCD_Write_Register(0x2A); 
	LCD_WR_Data(x>>8);LCD_WR_Data(x&0XFF); 			 
	LCD_Write_Register(0x2B); 
	LCD_WR_Data(y>>8);LCD_WR_Data(y&0XFF); 
	
	LCD_Write_Register(0X2E);

/* 使用16bits数据总线宽度时的逻辑控制时序 */
	LCD_BUS_IN16();	

    LCD_RS_SET;
    LCD_CS_CLR;     
 
    LCD_RD_CLR; 
    delay_us(1);                  
    LCD_RD_SET;
    //dummy READ
    LCD_RD_CLR;                    
    delay_us(1);                
    r=DATAIN;   
    LCD_RD_SET;
	
	LCD_RD_CLR;                    
	b=DATAIN;
	LCD_RD_SET;
	g=r&0XFF;
	g<<=8;

	LCD_BUS_OUT16();
	
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));;	//返回读取到的数据     
}

/**********************************************************
* 函数功能 ---> 背光、复位控制端口初始化
* 入口参数 ---> none
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_RST_BL_Init(void)
{
	/******背光管脚******/

	STM32_GPIOx_Init(LCD_BLInit);
}

/**********************************************************
        下面是相关的应用调用API函数，非必要可不修改
**********************************************************/


//=========================================================
//否则使用普通GPIO控制LCD                  
//=========================================================

/**********************************************************
* 函数功能 ---> 写数据到LCD寄存器（主要写命令或寄存器地址）
* 入口参数 ---> reg_val：要写入的数据
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_Write_Register(u16 reg_val)
{
	LCD_RS_CLR;		//写地址  
 	LCD_CS_CLR; 
	DATAOUT(reg_val);
	LCD_WR_CLR;
	LCD_WR_SET;
 	LCD_CS_SET;	
}
/**********************************************************
* 函数功能 ---> 写数据到LCD RAM
* 入口参数 ---> dat：要写入的数据
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_Write_Data(u16 dat)
{
	LCD_WR_Data(dat);
}
/**********************************************************
* 函数功能 ---> 向LCD某个寄存器写入数据
* 入口参数 ---> reg：要写入数据的寄存器地址
*               dat：要写入的数据
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_WriteRegisterData(u16 reg, u16 dat)
{
	LCD_Write_Register(reg);
	LCD_WR_Data(dat);
}
/**********************************************************
* 函数功能 ---> 读取LCD某个寄存器数值
* 入口参数 ---> reg：要读取数据的寄存器地址
* 返回数值 ---> 寄存器数值
* 功能说明 ---> none
**********************************************************/
u16 LCD_ReadRegisterData(u16 reg)
{
	LCD_Write_Register(reg);
	return LCD_Read_Data();
}
