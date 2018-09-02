#include "bussiness_userInterface.h"

#include "GUI.h"

osThreadId tid_bussinessUI_Thread;
osThreadDef(bussinessUI_Thread, osPriorityNormal, 1, 2048);

osPoolId  mpid_bussinessUI_pool;								 
osPoolDef(bussinessUIMS_pool, 10, stt_bussinessUIMS_MEAS);	// 内存池定义
osMessageQId  qid_UI2DT_messageQ;
osMessageQDef(MsgBox_UI2DTMS, 4, &stt_bussinessUIMS_MEAS);	// 消息队列定义，用于模块线程向无线通讯线程
osMessageQId  qid_DT2UI_messageQ;
osMessageQDef(MsgBox_DT2UIMS, 4, &stt_bussinessUIMS_MEAS);	// 消息队列定义，用于模块线程向无线通讯线程

void bussinessUI_Thread(const void *argument){

	GUI_Init();

	GUI_Clear();

	GUI_SetFont(&GUI_Font20_ASCII);
	GUI_CURSOR_Show();
	GUI_CURSOR_Select(&GUI_CursorCrossL);
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);

//	GUIDEMO_Main();
	
	for(;;){
	
		osDelay(10);
	}
}

void bussinessUI_Active(void){
	
	mpid_bussinessUI_pool   = osPoolCreate(osPool(bussinessUIMS_pool));		//创建内存池
	qid_UI2DT_messageQ 		= osMessageCreate(osMessageQ(MsgBox_UI2DTMS), NULL);	//创建消息队列
	qid_DT2UI_messageQ		= osMessageCreate(osMessageQ(MsgBox_DT2UIMS), NULL);	//创建消息队列

	tid_bussinessUI_Thread = osThreadCreate(osThread(bussinessUI_Thread), NULL);
}

