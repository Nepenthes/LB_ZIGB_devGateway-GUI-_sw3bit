#include "delay.h"

/*
*********************************************************************************************************
*                                             �Ĵ���
*********************************************************************************************************
*/
#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR   *(volatile unsigned int *)0xE0042004

#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitDWT
*	����˵��: ��ʼ��DWT. �ú����� bsp_Init() ���á�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_delayinit(void)
{
	DEM_CR         |= (unsigned int)DEM_CR_TRCENA;   
	DWT_CYCCNT      = (unsigned int)0u;
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

void delay_us(uint32_t _ulDelayTime)
{
	uint32_t tCnt, tDelayCnt;
	uint32_t tStart;

	tStart = DWT_CYCCNT;                                     /* �ս���ʱ�ļ�����ֵ */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		      

	while(tCnt < tDelayCnt)
			tCnt = DWT_CYCCNT - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
}
