#include "delay.h"
#include "ft6236_driver.h"

#include "GUI.h"

#include "Driver_USART.h"

#include "stdio.h"
#include "string.h"

#include "cmsis_os.h"

extern ARM_DRIVER_USART Driver_USART1;

TouchPointRefTypeDef TPR_Structure; 

void touchReleas_Thread(const void *argument);
osThreadId tid_touchReleas_Thread;
osThreadDef(touchReleas_Thread, osPriorityNormal, 1, 512);

void gui_touchStart(void){

	tid_touchReleas_Thread = osThreadCreate(osThread(touchReleas_Thread), NULL);
}

void touchReleas_Thread(const void *argument){

	GUI_PID_STATE State; 
	
	for(;;){
		
		FT6236_Scan();
	
		State.x = TPR_Structure.y[0];
		State.y = 240 - TPR_Structure.x[0];
		
		if(TPR_Structure.TouchSta & TP_PRES_DOWN)State.Pressed = 1;
		else State.Pressed = 0;
		
		GUI_PID_StoreState(&State); 

		osDelay(9);
	}
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ��������ʼ�ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void FT6236_Start(void)					
{
	SDA_OUT();     		//sda�����
	delay_us(3);
	FT6236_SDA = 1;									
	FT6236_SCL = 1;		//SCL��С�ߵ�ƽ����:0.6us
	delay_us(4);		//��ʼ�źŵ���С����ʱ��:0.6us
	FT6236_SDA = 0;		//SCL�ߵ�ƽ�ڼ䣬SDA��һ���½��ر�ʾ��ʼ�ź�
	delay_us(4);		//��ʼ�źŵ���С����ʱ��:0.6us
	FT6236_SCL = 0;		//��ס����,Ϊ����������ַ��׼��;
	delay_us(2);		//SCL��С�͵�ƽ����:1.2us,��RETʵ��
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ������ֹͣ�ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void FT6236_Stop(void)							
{
	SDA_OUT();     		//sda�����	
	delay_us(3);	
	FT6236_SCL = 1;		//SCL��С�ߵ�ƽ����:0.6us		
	delay_us(4);		//ֹͣ�źŵ���С����ʱ��:0.6us
	FT6236_SDA = 0;	
	delay_us(4);
	FT6236_SDA = 1;		//SCL�ߵ�ƽ�ڼ䣬SDA��һ�������ر�ʾֹͣ�ź�
	delay_us(2);						
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ������Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ����Ƭ����1B���ݺ���һ��Ӧ���ź�
*****************************************************/
void FT6236_McuACK(void)							
{
	FT6236_SCL = 0;	
	SDA_OUT();     		//sda�����	
	delay_us(3);
	FT6236_SDA = 0;	
	delay_us(2);																	
	FT6236_SCL = 1;		//SCL��С�ߵ�ƽ����:0.6us
	delay_us(2);
	FT6236_SCL = 0;		//SCL��С�͵�ƽ����:1.2us
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ�����ͷ�Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ����Ƭ��������ֹͣǰ����һ����Ӧ���ź�
*****************************************************/
void FT6236_McuNACK(void)
{
	FT6236_SCL = 0;	
	SDA_OUT();     				//sda�����	
	delay_us(3);
	FT6236_SDA = 1;	
	delay_us(2);																	
	FT6236_SCL = 1;				//SCL��С�ߵ�ƽ����:0.6us
	delay_us(2);
	FT6236_SCL = 0;				//SCL��С�͵�ƽ����:1.2us
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ�����FT6236������Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ��1������Ӧ��ʧ��
			 0������Ӧ��ɹ�
* ע������ ����Ƭ��д1����ַ/���ݺ���
			 ȫ�ֱ���RevAckF:�յ�FT6236Ӧ���źŵı�־λ,Ϊ0��ʾ�յ�
*****************************************************/
u8 FT6236_CheckAck(void)							
{
	u8 ucErrTime=0;
	SDA_IN();      				//SDA����Ϊ����
	FT6236_SDA = 1;
	FT6236_SCL = 1;				//ʹSDA��������Ч;SCL��С�ߵ�ƽ����:0.6us
	delay_us(3);
	while(FT6236_SDA_Read)
	{	
		ucErrTime++;
		if(ucErrTime>250)		//��Ӧ��
		{
			FT6236_Stop();	
			return 1;
		}
		delay_us(2);
	}
	FT6236_SCL = 0;
	return 0;
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ����IIC���߷���1B�ĵ�ַ/����
* ��ڲ��� �������͵�1B��ַ/����
* ���ز��� ����
* ע������ ������һ�����������ݷ��͹���;������˳���ǴӸߵ���
*****************************************************/
void FT6236_WrOneByte(u8 dat)						
{
	u8 i;						
	SDA_OUT();     				//sda�����	
	FT6236_SCL = 0;				//����ʱ�ӿ�ʼ���ݴ���
	delay_us(3);
	for(i = 8; i > 0; i--)		//8λ1B��ַ/���ݵĳ���
	{
		if(dat & 0x80) 		
			FT6236_SDA = 1;		//����"1"		
		else
			FT6236_SDA = 0;		//����"0"
		FT6236_SCL = 1;			//ʹSDA�ϵ�������Ч
		delay_us(2);			//SCL��С�ߵ�ƽ����:0.6us							
		FT6236_SCL = 0;			//SCL��С�͵�ƽ����:1.2us
		delay_us(2);
		dat <<= 1;				//������������1λ,Ϊ��λ����׼��	
	}		
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ����IIC���߽���1B������
* ��ڲ��� ����
* ���ز��� ���յ���1B����
* ע������ ������һ�����������ݽ��չ���;�Ӹߵ��͵�˳���������
*****************************************************/
u8 FT6236_RdOneByte(void)						
{
	u8 i,dat = 0;				//��������λ���������ݴ浥Ԫ
	SDA_IN();						//SDA����Ϊ����
	delay_us(2);	
	FT6236_SDA = 1;			//ʹ������,׼����ȡ����
	delay_us(2);
	for(i = 8;i > 0;i--)
	{
		FT6236_SCL = 0;
		delay_us(2);
		FT6236_SCL = 1;
		dat <<= 1;
		if(FT6236_SDA_Read)
			dat++;
		delay_us(2);			//SCL��С�͵�ƽ����:1.2us
	}
	FT6236_SDA = 1;		
	return(dat);				//����1B������
}

//��FT6236д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 FT6236_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	FT6236_Start();	 
	FT6236_WrOneByte(FT_CMD_WR);	//����д���� 	 
	FT6236_CheckAck(); 	 										  		   
	FT6236_WrOneByte(reg&0XFF);   	//���͵�8λ��ַ
	FT6236_CheckAck();  
	for(i=0;i<len;i++)
	{	   
    	FT6236_WrOneByte(buf[i]);  	//������
		ret=FT6236_CheckAck();
		if(ret)break;  
	}
    FT6236_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
//��FT6236����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void FT6236_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	FT6236_Start();	
 	FT6236_WrOneByte(FT_CMD_WR);   	//����д���� 	 
	FT6236_CheckAck(); 	 										  		   
 	FT6236_WrOneByte(reg&0XFF);   	//���͵�8λ��ַ
	FT6236_CheckAck();  
 	FT6236_Start();  	 	   
	FT6236_WrOneByte(FT_CMD_RD);   	//���Ͷ�����		   
	FT6236_CheckAck();	  
	for(i=0;i<len;i++)
	{	   
		*buf++ = FT6236_RdOneByte();		//����1B���ݵ��������ݻ�������
		FT6236_McuACK();					//����Ӧ��λ	  
	} 
	FT6236_McuNACK();						//n���ֽڶ���,���ͷ�Ӧ��λ
    FT6236_Stop();					//����һ��ֹͣ����	  
} 
/* 
**��������FT6236_Init
**�����������
**����ֵ����
**���ܣ���ʼ��FT6236����
*/  
void FT6236_Init(void)
{
	u8 temp; 
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO_InitTypeDef���͵Ľṹ��
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��PG�˿�ʱ��

	/******  PC0:ʱ��SCL-----PC2:����SDA  ***********************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;	//PG7,PG8	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//����������50MHz	
	GPIO_Init(GPIOC,&GPIO_InitStructure);					//��ʼ����ӦGPIOG	
	GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_2);			//PG.7.8 �����	
	/******  PC3:ƬѡRST  ***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//PC3	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz	
	GPIO_Init(GPIOC,&GPIO_InitStructure);					//��ʼ����ӦGPIOC		
	GPIO_SetBits(GPIOC,GPIO_Pin_3);							//PC.7 �����
	/**********************PC6���ж�INT*********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//PC6	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//���óɸ�������	
	GPIO_Init(GPIOC,&GPIO_InitStructure);					//��ʼ����ӦGPIOC	
	GPIO_SetBits(GPIOC,GPIO_Pin_1);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��

	//�ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);

	EXTI_InitStructure.EXTI_Line= 	EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = 	EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	EXTI_ClearITPendingBit(EXTI_Line1);    //���LINE1�ϵ��жϱ�־λ 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn ;	//ʹ��TOUCH_INT���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�0�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  

	FT6236_RST = 0;
	delay_ms(50);
	FT6236_RST = 1;
	delay_ms(100);
	FT6236_SDA=1;
	FT6236_SCL=1;
	delay_ms(10);
	temp=0;
	FT6236_WR_Reg(FT_DEVIDE_MODE,&temp,1);	//������������ģʽ 
 	temp=22;								//������Чֵ��22��ԽСԽ����	
 	FT6236_WR_Reg(FT_ID_G_THGROUP,&temp,1);	//���ô�����Чֵ
 	temp=12;								//�������ڣ�����С��12�����14
 	FT6236_WR_Reg(FT_ID_G_PERIODACTIVE,&temp,1); 
/******************************************************/
	
	gui_touchStart();	//ʵʱ�����̼߳���
}
const u16 FT6236_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};

void FT6236_Scan(void)
{
	u8 i=0;
	u8 sta = 0;
	u8 buf[4] = {0};    
	
	FT6236_RD_Reg(0x02,&sta,1);//��ȡ�������״̬  	   
 	if(sta & 0x0f)	//�ж��Ƿ��д����㰴�£�0x02�Ĵ����ĵ�4λ��ʾ��Ч�������
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))����ĸ���ת��Ϊ�����㰴����Ч��־
 		for(i=0;i<5;i++)	                                //�ֱ��жϴ�����1-5�Ƿ񱻰���
 		{
 			if(TPR_Structure.TouchSta & (1<<i))			    //��ȡ����������
 			{											    //���������ȡ��Ӧ��������������
 				FT6236_RD_Reg(FT6236_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
				TPR_Structure.x[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
				TPR_Structure.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
 				if((buf[0]&0XC0)!=0X80)
 				{
					TPR_Structure.x[i]=TPR_Structure.y[i]=0;//������contact�¼�������Ϊ��Ч	
					return;
				}
 			}
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;     //�������±��
 	}
 	else
 	{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN) 	//֮ǰ�Ǳ����µ�
 			TPR_Structure.TouchSta &= ~0x80;        //�����ɿ����	
 		else
 		{
 			TPR_Structure.x[0] = TPR_Structure.x[0];
 			TPR_Structure.y[0] = TPR_Structure.y[0];
 			TPR_Structure.TouchSta &=0xe0;	//�����������Ч���
 		}
 	}
}

/*
�ж��ⲿ�ж�״̬�Լ�����ⲿ״̬��־λ�ĺ��� EXTI_GetFlagStatus �� EXTI_ClearFlag��
�� EXTI_GetITStatus �����л����ж������ж��Ƿ�ʹ�ܣ�ʹ���˲�ȥ�ж��жϱ�־λ����
EXTI_GetFlagStatus ֱ�������ж�״̬��־λ��
*/
void EXTI1_IRQHandler(void)		//�ж���������ļ���startup_stm32f10x_hd.s 
{
	
	delay_us(10);   //����	
	
	if(FT6236_INT == 0)	   	
		TPR_Structure.TouchSta |= TP_COORD_UD;	//���������и���
	
    EXTI_ClearITPendingBit(EXTI_Line1); //��� LINE6 �ϵ��жϱ�־λ
}

void ft6236_Test(void){
		
	FT6236_Init();

	for(;;){
		
		delay_ms(50);
	
		if(TPR_Structure.TouchSta &TP_COORD_UD)		//�����а���
		{
			
			TPR_Structure.TouchSta &= ~TP_COORD_UD;	//����
		
			{
			
				char logo_info[40] = {0};
				
				sprintf(logo_info, "coordinates_X : %d\ncoordinates_Y : %d\n", (int)TPR_Structure.x[0], (int)TPR_Structure.y[0]);
				Driver_USART1.Send(logo_info, strlen(logo_info));
			}
		}
	}
}
