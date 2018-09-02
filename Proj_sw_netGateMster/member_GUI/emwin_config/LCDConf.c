/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "LCDTFT_driver.h"
#include "ili9341_driver.h"
#include "ft6236_driver.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size	//这里定义所用的LCD的分辨率
//
#define XSIZE_PHYS  240 // To be adapted to x-screen size
#define YSIZE_PHYS  320 // To be adapted to y-screen size

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteReg(U16 Data) {
// Bula => MiniSTM32 V3.0 不支持FSMC，采用GPIO模式往LCD寄存器写入数据
//	LCD_Write_Register(Data);	
	
	LCD_CS_CLR;
	LCD_RD_SET;
	LCD_RS_CLR;

	LCD_WR_CLR;
	DATAOUT(Data);
	LCD_WR_SET;
	
	LCD_CS_SET;
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
// Bula => MiniSTM32 V3.0 不支持FSMC，采用GPIO模式往LCD RAM中写入数据
//	LCD_Write_Data(Data);		
	
	LCD_CS_CLR;
	LCD_RD_SET;
	LCD_RS_SET;

	LCD_WR_CLR;
	DATAOUT(Data);
	LCD_WR_SET;
	
	LCD_CS_SET;
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
//  while (NumItems--) {
//// Bula => MiniSTM32 V3.0 不支持FSMC，采用GPIO模式往LCD RAM中连续写入NumItems组数据
//	  LCD_Write_Data(*pData++);		
//  }
	
	LCD_CS_CLR;
	LCD_RD_SET;
	LCD_RS_SET;
	
	while (NumItems--) {

		LCD_WR_CLR;
		DATAOUT(*pData++);
		LCD_WR_SET;
	}
	
	LCD_CS_SET;
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
	
//  while (NumItems--) {
//// Bula => MiniSTM32 V3.0 不支持FSMC，采用GPIO模式从LCD RAM中连续读取NumItems组数据
//	  *pData++ = LCD_ReadRegisterData(Write_Data_to_GRAM);		
//  }
	
	LCD_BUS_IN16();

	LCD_CS_CLR;
	LCD_WR_SET;
	LCD_RS_SET;
	
	while (NumItems--) {
	  
		LCD_RD_CLR;
		*pData++ = DATAIN;
		LCD_RD_SET;
	}
	
	LCD_CS_SET; 
	
	LCD_BUS_OUT16();
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/

void LCD_X_Config(void) {
	
	GUI_DEVICE * pDevice;
	CONFIG_FLEXCOLOR Config = {0};
	GUI_PORT_API PortAPI = {0};
	//
	// Set display driver and color conversion
	//
	pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);	//设置颜色格式
	//GUIDRV_FLEXCOLOR：这个形参区别不同的LCD控制器，详细见参考手册的第975页表格
	//GUICC_565：这个形参决定LCD颜色数据格式
	//
	// Display driver configuration, required for Lin-driver
	//
	LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
	LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
	
	GUI_TOUCH_Calibrate(GUI_COORD_X, 0, 240, 0, 240);
	GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, 320, 0, 320);
	
	Config.FirstCOM = 0;                                          //modify by fire
	Config.FirstSEG = 0;                                          //modify by fire  
	Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_X;
//	Config.Orientation = GUI_MIRROR_Y|GUI_MIRROR_X;				  //modify by fire
//	Config.Orientation = GUI_SWAP_XY |GUI_MIRROR_Y;			  	  //modify by fire	
	Config.NumDummyReads = 2;                                     //modify by fire
	//
	// Orientation
	//
	//  Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_Y;
	//LCD显示方向设置
	//Orientation：结构体（GUIDRV_FLEXCOLOR.h文件中）决定了LCD的显示方式，详细见参考手册第988页	
	GUIDRV_FlexColor_Config(pDevice, &Config);
	//
	// Set controller and operation mode
	//

	PortAPI.pfWrite16_A0  = LcdWriteReg;
	PortAPI.pfWrite16_A1  = LcdWriteData;
	PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
	PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
	GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
	//GUIDRV_FLEXCOLOR_F66709：这个形参就是LCD控制器型号支持的参数，详细见参考手册的第1002页表格
	//GUIDRV_FLEXCOLOR_M16C0B16：这个形参就是LCD控制器数据总线宽度，颜色等参数，详细见参考手册第1003页表格
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...

	LCDTFT_Init();	//调用LCD初始化函数，初始化接口等
	FT6236_Init();	//触摸屏初始化

    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

