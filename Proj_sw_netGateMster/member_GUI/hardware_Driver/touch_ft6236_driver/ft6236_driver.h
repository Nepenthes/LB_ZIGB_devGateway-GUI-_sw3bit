#ifndef __FT6236_H_
#define __FT6236_H_

#include "IO_Map.h" 
#include "stm32f10x.h"

#define SDA_IN()  {GPIOC->CRL&=0XFFFFF0FF;GPIOC->CRL |= 8 << 8;}	 //����ģʽ����������ģʽ
#define SDA_OUT() {GPIOC->CRL&=0XFFFFF0FF;GPIOC->CRL |= 3 << 8;}	 //ͨ���������������ٶ�50MHZ

#define TP_PRES_DOWN 0x80  //����������	
#define TP_COORD_UD  0x40  //����������±��

//������������ݽṹ�嶨��
typedef struct			
{
	u8 TouchSta;	//���������b7:����1/�ɿ�0; b6:0û�а�������/1�а�������;bit5:������bit4-bit0�����㰴����Ч��־����ЧΪ1���ֱ��Ӧ������5-1��
	u16 x[5];		//֧��5�㴥������Ҫʹ��5������洢����������
	u16 y[5];
	
}TouchPointRefTypeDef;

extern TouchPointRefTypeDef TPR_Structure;

/*********************IO��������*********************/									
#define	FT6236_RST			PCout(3)	//�������ģʽ						
#define FT6236_SCL 			PCout(0)	//�������ģʽ
#define FT6236_SDA 			PCout(2)	//�������ģʽ
#define FT6236_SDA_Read 	PCin(2)		//��������ģʽ									
#define FT6236_INT 			PCin(1)		//��������ģʽ
							
//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
//FT6236 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT6236ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  

#define	FT_ID_G_LIB_VERSION		0xA1		//�汾		
#define FT_ID_G_MODE 			0xA4   		//FT6236�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���  
#define Chip_Vendor_ID          0xA3        //оƬID(0x36)
#define ID_G_FT6236ID			0xA8		//0x11

u8 FT6236_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT6236_RD_Reg(u16 reg,u8 *buf,u8 len);
void FT6236_Init(void);
void FT6236_Scan(void);
u16 TP_Read_XOY(u8 xy);

void gui_touchStart(void);

void ft6236_Test(void);

#endif



