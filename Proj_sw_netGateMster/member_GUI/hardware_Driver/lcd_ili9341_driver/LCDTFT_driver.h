#ifndef __LCDTFT_DRIVER_H_
#define __LCDTFT_DRIVER_H_

/**********************************************************
                     �ⲿ����ͷ�ļ�
         Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
**********************************************************/

#include "stm32f10x.h"
#include "IO_Map.h"

/**********************************************************
                    ����LCD��صĺ궨��
**********************************************************/


/**********************************************************
                    ���屳��ӿ���Ϣ
               ��ͬ�Ŀ��ƽӿ����޸�����                       
**********************************************************/

#define LCD_BLControl               PAout(1)    //�������

//                                      GPIOʱ��        GPIO��   ���ź�     ��ת�ٶ�       �������ģʽ
#define LCD_BLInit					RCC_APB2Periph_GPIOA,GPIOB,GPIO_Pin_1,GPIO_Speed_50MHz,GPIO_Mode_Out_PP

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

//=========================================================
//ʹ��GPIO����
//=========================================================

	/* ������Ҫ��дlcd_drive.c�����LCD_GPIO_Init()����������IO�ĳ�ʼ������ */

	/* ����ʵ�����ӽ��и�д����궨�� */
	//                                      GPIOʱ��        GPIO��   ���ź�     ��ת�ٶ�       �������ģʽ
	#define LCD_RSInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_8,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_CSInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_9,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_RDInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_6,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_WRInit					RCC_APB2Periph_GPIOC,GPIOC,GPIO_Pin_7,GPIO_Speed_50MHz,GPIO_Mode_Out_PP

	/*******Ӳ����صĺ궨��*******
	LCD_RST---> PC10  ��λ
	LCD_CS ---> PC9   Ƭѡ
	LCD_RS ---> PC8   ����/����
	LCD_WR ---> PC7   д����
	LCD_RD ---> PC6   ������
	**********/
	#define	LCD_RST_SET	GPIO_SetBits(GPIOC,GPIO_Pin_10)		//������			PC10
	#define	LCD_CS_SET  GPIO_SetBits(GPIOC,GPIO_Pin_9)		//Ƭѡ�˿�  		PC9
	#define	LCD_RS_SET	GPIO_SetBits(GPIOC,GPIO_Pin_8)		//����/���� 		PC8	   
	#define	LCD_WR_SET	GPIO_SetBits(GPIOC,GPIO_Pin_7)		//д����			PC7
	#define	LCD_RD_SET	GPIO_SetBits(GPIOC,GPIO_Pin_6)		//������			PC6
	
	#define	LCD_RST_CLR  GPIO_ResetBits(GPIOC,GPIO_Pin_10)	//Ƭѡ�˿�  		PC10
	#define	LCD_CS_CLR  GPIO_ResetBits(GPIOC,GPIO_Pin_9)	//Ƭѡ�˿�  		PC9
	#define	LCD_RS_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_8)	//����/����			PC8	   
	#define	LCD_WR_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_7)	//д����			PC7
	#define	LCD_RD_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_6)	//������			PC6  

	//PB0~15,��Ϊ������
	#define DATAOUT(x)	GPIO_Write(GPIOB,x);
	#define DATAIN		GPIO_ReadInputData(GPIOB)
	
	/* ��ȡ����ʱ�����������߷���Ϊ���� */
	#define LCD_BUS_IN16()		{GPIOB->CRL = 0x88888888;GPIOB->CRH = 0x88888888;GPIOB->ODR=0xffff;}
	
	/* ��ȡ����ʱ�����������߷���Ϊ���� */
	#define LCD_GRAM_IN16()		{GPIOB->CRL = 0x88888888;GPIOB->CRH = 0x88888888;GPIOB->ODR=0xffff;}

	/* д������ʱ�����������߷���Ϊ��� */
	#define LCD_BUS_OUT16()		{GPIOB->CRL = 0x33333333;GPIOB->CRH = 0x33333333;GPIOB->ODR=0Xffff;}


/**********************************************************
                       �ⲿ���ܺ���                      
**********************************************************/
	
/**********************************************************
* �깦�� ---> д���ݵ�LCD RAM
* ��ڲ��� ---> dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
#define LCD_WR_Data(val)	{\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(val);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
}

void LCD_GPIO_Init(void);     //ͨѶ�����ƶ˿ڳ�ʼ��

/* ����ͨ��������� */
void LCD_RST_BL_Init(void);   //���⡢��λ���ƶ˿ڳ�ʼ��     
void LCD_Write_Register(u16 reg_val);     //д���ݵ�LCD�Ĵ�������Ҫд�����Ĵ�����ַ��
void LCD_Write_Data(u16 dat); //д���ݵ�LCD RAM
u16 LCD_Read_Data(void);      //��ȡLCD����
void LCD_WriteRegisterData(u16 reg, u16 dat);   //��LCDĳ���Ĵ���д������
u16 LCD_ReadRegisterData(u16 reg);  //��ȡLCDĳ���Ĵ�����ֵ

u16 ili9341_readPoint(u16 x, u16 y);

void LCD_Delay(u8 i);

#endif

