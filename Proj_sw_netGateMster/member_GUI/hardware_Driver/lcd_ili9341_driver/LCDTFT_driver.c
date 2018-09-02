#include "LCDTFT_driver.h"
#include "delay.h"

/**********************************************************
* �������� ---> STM32 GPIO��������ʼ��
* ��ڲ��� ---> *STMCapa�������ַ���ʾ����
* ������ֵ ---> none
* ����˵�� ---> ���մ˺����βε�˳����к궨�弴��
**********************************************************/
void STM32_GPIOx_Init(
                        uint32_t RCC_APB2Periph,		/* GPIO��Ӧʱ�� */
						GPIO_TypeDef* GPIOx,			/* ����Ӧ��GPIO�� */
						uint16_t GPIO_Pinx,				/* GPIO��Ӧ�Ĺܽź� */                       
						GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO��ת�ٶ� */
						GPIOMode_TypeDef GPIO_MODE		/* GPIO����ģʽ */
					   )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//����������
	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	assert_param(IS_GPIO_PIN(GPIO_Pinx));
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_SPEED(GPIO_SPEED));
	assert_param(IS_GPIO_MODE(GPIO_MODE)); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);	//��������ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE;

	GPIO_Init(GPIOx, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��
}

/**********************************************************
* �������� ---> ͨѶ�����ƶ˿ڳ�ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/* GPIO����LCD�����ߵȳ�ʼ������ */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //ʹ��PORTB,Cʱ�Ӻ�AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//����SWD��ʧ��JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTC6~10�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIO����LCD�������߹ܽų�ʼ������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  PORTB�������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
 
	GPIO_SetBits(GPIOB,GPIO_Pin_All);
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

/**********************************************************
* �������� ---> ��ȡLCD��Ҫ����ʱ
* ��ڲ��� ---> i����Ҫ��ʱ��ֵ
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Delay(u8 i)
{
	while(i--);
}

/**********************************************************
* �������� ---> д���ݵ�����
* ��ڲ��� ---> val��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Bus(u16 val)
{
/* ʹ��16bits�������߿��ʱ���߼�����ʱ�� */

//	LCD_BUS_OUT16();	//��������Ϊ�������
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(val);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}
/**********************************************************
* �������� ---> ��ȡLCD����
* ��ڲ��� ---> none
* ������ֵ ---> ��ȡ��������
* ����˵�� ---> none
**********************************************************/
u16 LCD_Read_Data(void)
{
	u16 temp=0;	//�������ݻ���

/* ʹ��16bits�������߿��ʱ���߼�����ʱ�� */
	LCD_BUS_IN16();

	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	
//	LCD_RD_SET;
	LCD_RD_CLR;
	
	LCD_Delay(2);
	
	temp=DATAIN;  
	
	LCD_RD_SET;
	LCD_CS_SET; 

	LCD_BUS_OUT16();
	
	return temp;	//���ض�ȡ��������     
}

u16 ili9341_readPoint(u16 x, u16 y){

	u16 r,g,b;
	
	LCD_Write_Register(0x2A); 
	LCD_WR_Data(x>>8);LCD_WR_Data(x&0XFF); 			 
	LCD_Write_Register(0x2B); 
	LCD_WR_Data(y>>8);LCD_WR_Data(y&0XFF); 
	
	LCD_Write_Register(0X2E);

/* ʹ��16bits�������߿��ʱ���߼�����ʱ�� */
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
	
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));;	//���ض�ȡ��������     
}

/**********************************************************
* �������� ---> ���⡢��λ���ƶ˿ڳ�ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_RST_BL_Init(void)
{
	/******����ܽ�******/

	STM32_GPIOx_Init(LCD_BLInit);
}

/**********************************************************
        ��������ص�Ӧ�õ���API�������Ǳ�Ҫ�ɲ��޸�
**********************************************************/


//=========================================================
//����ʹ����ͨGPIO����LCD                  
//=========================================================

/**********************************************************
* �������� ---> д���ݵ�LCD�Ĵ�������Ҫд�����Ĵ�����ַ��
* ��ڲ��� ---> reg_val��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Register(u16 reg_val)
{
	LCD_RS_CLR;		//д��ַ  
 	LCD_CS_CLR; 
	DATAOUT(reg_val);
	LCD_WR_CLR;
	LCD_WR_SET;
 	LCD_CS_SET;	
}
/**********************************************************
* �������� ---> д���ݵ�LCD RAM
* ��ڲ��� ---> dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Data(u16 dat)
{
	LCD_WR_Data(dat);
}
/**********************************************************
* �������� ---> ��LCDĳ���Ĵ���д������
* ��ڲ��� ---> reg��Ҫд�����ݵļĴ�����ַ
*               dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_WriteRegisterData(u16 reg, u16 dat)
{
	LCD_Write_Register(reg);
	LCD_WR_Data(dat);
}
/**********************************************************
* �������� ---> ��ȡLCDĳ���Ĵ�����ֵ
* ��ڲ��� ---> reg��Ҫ��ȡ���ݵļĴ�����ַ
* ������ֵ ---> �Ĵ�����ֵ
* ����˵�� ---> none
**********************************************************/
u16 LCD_ReadRegisterData(u16 reg)
{
	LCD_Write_Register(reg);
	return LCD_Read_Data();
}
