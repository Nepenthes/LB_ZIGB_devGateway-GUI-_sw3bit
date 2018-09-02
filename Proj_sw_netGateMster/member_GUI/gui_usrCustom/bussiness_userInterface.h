#ifndef __BUSSINESS_INTERFACE_H_
#define __BUSSINESS_INTERFACE_H_

#include "stm32f10x.h"
#include "cmsis_os.h"

#include "stdbool.h"

typedef struct{

	u8 dats;
}stt_bussinessUIMS_MEAS;

extern osPoolId  	 mpid_bussinessUI_pool;	
extern osMessageQId  qid_UI2DT_messageQ;
extern osMessageQId  qid_DT2UI_messageQ;

void bussinessUI_Thread(const void *argument);

void bussinessUI_Active(void);

#endif

