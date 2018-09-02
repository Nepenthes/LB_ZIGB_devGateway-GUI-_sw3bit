/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions

#include "UART_dataTransfer.h"
#include "ili9341_driver.h"
#include "ft6236_driver.h"

#include "bussiness_userInterface.h"

#include "GUI.h"
#include "GUIDEMO.h"

#include "delay.h"

void myTest_Thread(const void *argument);
osThreadId tid_myTest_Thread;
osThreadDef(myTest_Thread, osPriorityNormal, 1, 2048);

void testThread_app(void){
	
	GUI_PID_STATE touch_State;
	
	
	for(;;){
	
//		GUI_TOUCH_GetState(&touch_State);
		
		osDelay(500);
		
//		{
//		
//			char logo_info[40] = {0};
//			u16 color =  LCD_ReadPointColor(50, 50);
//			
//			sprintf(logo_info, "testData is %04X\n", (int)color);
//			Driver_USART1.Send(logo_info, strlen(logo_info));
//		}
	}
}

void myTest_Thread(const void *argument){

	bsp_delayinit();
	testThread_app();
}


/*
 * main: initialize and start the system
 */
int main (void) {
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here

  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	
	bsp_delayinit();
	
	osMemoryInit();
	msgQueueInit();
	
	communicationActive(comObj_DbugP1, ftOBJ_DEBUG);
	communicationActive(comObj_DataTransP1, ftOBJ_ZIGB);
	communicationActive(comObj_DataTransP2, ftOBJ_WIFI);
	
	bussinessUI_Active();
	
//	tid_myTest_Thread = osThreadCreate(osThread(myTest_Thread), NULL);

  osKernelStart ();                         // start thread execution 
}
