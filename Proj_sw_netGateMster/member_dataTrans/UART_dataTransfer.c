#include "UART_dataTransfer.h"

osThreadId tid_com1DbugP1_Thread;
osThreadId tid_com2DataTransP1_Thread;
osThreadId tid_com5DataTransP2_Thread;

osThreadDef(com1DbugP1_Thread,osPriorityNormal,	1,	1024);
osThreadDef(com2DataTransP1_Thread,osPriorityNormal,	1,	4096);
osThreadDef(com5DataTransP2_Thread,osPriorityNormal,	1,	4096);

osTimerDef(TimerZigbDevManage, TimerZigbDevManage_Callback);

// Memory pool ID
osPoolId  (ZigbTrans_poolAttr_id);  
// Declare memory pool	//ZIGB���ݴ��� ���ݽṹ��
osPoolDef (datsZigbTrans_pool, 5, datsAttr_ZigbTrans); 
// Memory pool ID
osPoolId  (ZigbnwkState_poolAttr_id);   
// Declare memory pool	//ZIGB����ڵ���Ϣ ���ݽṹ��              
osPoolDef (nwkZigbDevStateAttr_pool, 32, nwkStateAttr_Zigb);  
// Memory pool ID
osPoolId  (WIFITrans_poolAttr_id);     
// Declare memory pool	//WIFI���������ݴ��� ���ݽṹ��             
osPoolDef (datsWIFITrans_pool, 5, datsAttr_WIFITrans); 
// Memory pool ID
osPoolId  (dttAct_poolAttr_id); 
// Declare memory pool	//���ݴ�����̳�ʼ����Ϣ ���ݽṹ��
osPoolDef (dtThreadActInitParm_pool, 5, type_ftOBJ);  

// messageQ ID
osPoolId  (threadDP_poolAttr_id); 
// Declare memory pool	//���ݴ�����̼���Ϣ���� ���ݽṹ��
osPoolDef (threadDatsPass_pool, 10, stt_threadDatsPass);  
// messageQ ID
osMessageQId  mqID_threadDP_Z2W;                  	
// Define message queue	//ZigB��WIFI��Ϣ����
osMessageQDef(MsgBox_threadDP_Z2W, 5, &stt_threadDatsPass);   
// messageQ ID
osMessageQId  mqID_threadDP_W2Z;
// Define message queue	//WIFI��ZigB��Ϣ����
osMessageQDef(MsgBox_threadDP_W2Z, 5, &stt_threadDatsPass);  

//typedef int32_t (*fun_UARTRecv)(void *data, uint32_t num);

void USART1_callback(uint32_t event){

	;
}

void USART2_callback(uint32_t event){

	;
}

void USART5_callback(uint32_t event){

	;
}

void USART1_DbugP1_Init(void){

    /*Initialize the USART driver */
    Driver_USART1.Initialize(USART1_callback);
    /*Power up the USART peripheral */
    Driver_USART1.PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 115200 Bits/sec */
    Driver_USART1.Control(ARM_USART_MODE_ASYNCHRONOUS |
                          ARM_USART_DATA_BITS_8 |
                          ARM_USART_PARITY_NONE |
                          ARM_USART_STOP_BITS_1 |
                          ARM_USART_FLOW_CONTROL_NONE, 115200);

    /* Enable Receiver and Transmitter lines */
    Driver_USART1.Control (ARM_USART_CONTROL_TX, 1);
    Driver_USART1.Control (ARM_USART_CONTROL_RX, 1);
	
    Driver_USART1.Send("i'm usart1 for debugP1\r\n", 24);
}

void USART2_DataTransP1_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*Initialize the USART driver */
    Driver_USART2.Initialize(USART2_callback);
    /*Power up the USART peripheral */
    Driver_USART2.PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 115200 Bits/sec */
    Driver_USART2.Control(ARM_USART_MODE_ASYNCHRONOUS |
                          ARM_USART_DATA_BITS_8 |
                          ARM_USART_PARITY_NONE |
                          ARM_USART_STOP_BITS_1 |
                          ARM_USART_FLOW_CONTROL_NONE, 115200);

    /* Enable Receiver and Transmitter lines */
    Driver_USART2.Control (ARM_USART_CONTROL_TX, 1);
    Driver_USART2.Control (ARM_USART_CONTROL_RX, 1);
	
    Driver_USART2.Send("i'm usart5 for dataTransP1\r\n", 28);
}

void USART5_DataTransP2_Init(void){
	
    GPIO_InitTypeDef GPIO_InitStructure;

    /*Initialize the USART driver */
    Driver_USART5.Initialize(USART5_callback);
    /*Power up the USART peripheral */
    Driver_USART5.PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 115200 Bits/sec */
    Driver_USART5.Control(ARM_USART_MODE_ASYNCHRONOUS |
                          ARM_USART_DATA_BITS_8 |
                          ARM_USART_PARITY_NONE |
                          ARM_USART_STOP_BITS_1 |
                          ARM_USART_FLOW_CONTROL_NONE, 115200);

    /* Enable Receiver and Transmitter lines */
    Driver_USART5.Control (ARM_USART_CONTROL_TX, 1);
    Driver_USART5.Control (ARM_USART_CONTROL_RX, 1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	PAout(12) = 1;
	
    Driver_USART5.Send("i'm usart5 for dataTransP2\r\n", 28);
}

void *memmem(void *start, unsigned int s_len, void *find,unsigned int f_len){
	
	char *p, *q;
	unsigned int len;
	p = start, q = find;
	len = 0;
	while((p - (char *)start + f_len) <= s_len){
			while(*p++ == *q++){
					len++;
					if(len == f_len)
							return(p - f_len);
			};
			q = find;
			len = 0;
	};
	return(NULL);
}

int memloc(u8 str2[],u8 num_s2,u8 str1[],u8 num_s1)
{
	int la = num_s1;
	int i, j;
	int lb = num_s2;
	for(i = 0; i < lb; i ++)
	{
		for(j = 0; j < la && i + j < lb && str1[j] == str2[i + j]; j ++);
		if(j == la)return i;
	}
	return -1;
}

int strloc(char *str2,char *str1)
{
	int la = strlen(str1);
	int i, j;
	int lb = strlen(str2);
	for(i = 0; i < lb; i ++)
	{
		for(j = 0; j < la && i + j < lb && str1[j] == str2[i + j]; j ++);
		if(j == la)return i;
	}
	return -1;
}

/*zigbee�ظ��ڵ���Ϣ�Ż�ȥ��*/ 
void zigbDev_delSame(nwkStateAttr_Zigb *head)	
{
    nwkStateAttr_Zigb *p,*q,*r;
    p = head->next; 
    while(p != NULL)    
    {
        q = p;
        while(q->next != NULL) 
        {
            if(q->next->nwkAddr == p->nwkAddr || (!memcmp(q->next->macAddr, p->macAddr, DEVMAC_LEN) && (q->next->devType == p->devType))) 
            {
                r = q->next; 
                q->next = r->next;   
                osPoolFree(ZigbnwkState_poolAttr_id,r);
            }
            else
                q = q->next;
        }

        p = p->next;
    }
}

/*zigbee�豸��Ϣ�ڵ������ڵ�*/ 
u8 zigbDev_eptCreat(nwkStateAttr_Zigb *pHead,nwkStateAttr_Zigb pNew){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	u8 nCount = 0;
	
	nwkStateAttr_Zigb *pNew_temp = (nwkStateAttr_Zigb *) osPoolAlloc(ZigbnwkState_poolAttr_id);
	pNew_temp->nwkAddr 				= pNew.nwkAddr;
	memcpy(pNew_temp->macAddr, pNew.macAddr, DEVMAC_LEN);
	pNew_temp->devType 				= pNew.devType;
	pNew_temp->onlineDectect_LCount = pNew.onlineDectect_LCount;
	pNew_temp->next	   = NULL;
	
	while(pAbove->next != NULL){
		
		nCount ++;
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	pAbove->next = pNew_temp;
	return ++nCount;
}

/*zigbee�豸��Ϣ����ڵ���������������ַ		����������ʹ����ȡ���Ľڵ���Ϣ�ڴ��Ҫ�ͷţ�����*/ 
nwkStateAttr_Zigb *zigbDev_eptPutout_BYnwk(nwkStateAttr_Zigb *pHead,u16 nwkAddr,bool method){	//method = 1,Դ�ڵ��ַ���أ�method = 0,Դ�ڵ���Ϣӳ���ַ���أ���ַ��������Ӱ��Դ�ڵ�
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp = (nwkStateAttr_Zigb *) osPoolAlloc(ZigbnwkState_poolAttr_id);
	pTemp->next = NULL;
	
	while(!(pAbove->nwkAddr == nwkAddr) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(pAbove->nwkAddr == nwkAddr){
		
		if(!method){
			
			pTemp->nwkAddr 				= pAbove->nwkAddr;
			memcpy(pTemp->macAddr, pAbove->macAddr, DEVMAC_LEN);
			pTemp->devType 				= pAbove->devType;
			pTemp->onlineDectect_LCount = pAbove->onlineDectect_LCount;
		}else{
			
			pTemp = pAbove;	
		}
		
		return pTemp;
	}else{
		
		osPoolFree(ZigbnwkState_poolAttr_id,pTemp);
		return NULL;
	}	
} 

/*zigbee�豸��Ϣ����ڵ�����������豸MAC��ַ���豸����		����������ʹ����ȡ���Ľڵ���Ϣ�ڴ��Ҫ�ͷţ�����*/ 
nwkStateAttr_Zigb *zigbDev_eptPutout_BYpsy(nwkStateAttr_Zigb *pHead,u8 macAddr[DEVMAC_LEN],u8 devType,bool method){		//method = 1,Դ�ڵ��ַ���أ�method = 0,Դ�ڵ���Ϣӳ���ַ���أ���ַ��������Ӱ��Դ�ڵ�
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp = (nwkStateAttr_Zigb *) osPoolAlloc(ZigbnwkState_poolAttr_id);
	pTemp->next = NULL;
	
	while(!(!memcmp(pAbove->macAddr, macAddr, DEVMAC_LEN) && pAbove->devType == devType) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->macAddr, macAddr, DEVMAC_LEN) && pAbove->devType == devType){
		
		if(!method){
			
			pTemp->nwkAddr 				= pAbove->nwkAddr;
			memcpy(pTemp->macAddr, pAbove->macAddr, DEVMAC_LEN);
			pTemp->devType 				= pAbove->devType;
			pTemp->onlineDectect_LCount = pAbove->onlineDectect_LCount;
		}else{
			
			pTemp = pAbove;	
		}
		
		return pTemp;
	}else{
		
		osPoolFree(ZigbnwkState_poolAttr_id,pTemp);
		return NULL;
	}	
}

/*zigbee�豸��Ϣ����ڵ�ɾ�������������ַ*/ 
bool zigbDev_eptRemove_BYnwk(nwkStateAttr_Zigb *pHead,u16 nwkAddr){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp;
	
	while(!(pAbove->nwkAddr == nwkAddr) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(pAbove->nwkAddr == nwkAddr){
		
		pTemp = pAbove;
		pFollow->next = pAbove->next;
		osPoolFree(ZigbnwkState_poolAttr_id,pTemp);
		return true;
	}else{
		
		return false;
	}
}

/*zigbee�豸��Ϣ����ڵ�ɾ���������豸MAC��ַ���豸����*/ 
bool zigbDev_eptRemove_BYpsy(nwkStateAttr_Zigb *pHead,u8 macAddr[DEVMAC_LEN],u8 devType){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp;
	
	while(!(!memcmp(pAbove->macAddr, macAddr, DEVMAC_LEN) && pAbove->devType == devType) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->macAddr, macAddr, DEVMAC_LEN) && pAbove->devType == devType){
		
		 pTemp = pAbove;
		 pFollow->next = pAbove->next;
		 osPoolFree(ZigbnwkState_poolAttr_id,pTemp);
		 return true;
	}else{
		
		return false;
	}
}

/*zigbee�豸��Ϣ�����Ȳ���*/
u8 zigbDev_chatLenDectect(nwkStateAttr_Zigb *pHead){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	u8 loop;
	
	while(pAbove->next != NULL){
		
		loop ++;
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}

	return loop;
}

/*zigbee�豸��Ϣ�������  ���� �豸MAC��ַ �� �豸���� ���������*/
u8 ZigBdevDispList(nwkStateAttr_Zigb *pHead,u8 DevInform[]){
	
	nwkStateAttr_Zigb *Disp = pHead;
	u8 loop = 0;
	
	if(0 == zigbDev_chatLenDectect(pHead)){
		
		return 0;
	}

	while(Disp->next != NULL){
	
		Disp = Disp->next;
		
		memcpy(&DevInform[loop * (DEVMAC_LEN + 1)], Disp->macAddr, DEVMAC_LEN);
		DevInform[loop * (DEVMAC_LEN + 1) + 5] = Disp->devType;
		loop ++;
	}
	
	return loop;
}

/*zigbee���������У��*/
u8 XOR_CHECK(u8 buf[],u8 length){

	u8 loop = 0;
	u8 valXOR = buf[0];
	
	for(loop = 1;loop < length;loop ++)valXOR ^= buf[loop];
	
	return valXOR;
}

/*zigbee����֡����*/
u8 ZigB_TXFrameLoad(u8 frame[],u8 cmd[],u8 cmdLen,u8 dats[],u8 datsLen){		

	const u8 frameHead = ZIGB_FRAME_HEAD;	//ZNP,SOF֡ͷ
	u8 xor_check = datsLen;					//���У�飬֡β
	u8 loop = 0;
	u8 ptr = 0;
	
	frame[ptr ++] = frameHead;
	frame[ptr ++] = datsLen;
	
	memcpy(&frame[ptr],cmd,cmdLen);
	ptr += cmdLen;
	for(loop = 0;loop < cmdLen;loop ++)xor_check ^= cmd[loop];

	memcpy(&frame[ptr],dats,datsLen);
	ptr += datsLen;
	for(loop = 0;loop < datsLen;loop ++)xor_check ^= dats[loop];	
	
	frame[ptr ++] = xor_check;
	
	return ptr;
}

/*zigbee��ָ���·�����Ӧ��֤*/
bool zigb_VALIDA_INPUT(type_uart_OBJ UART_OBJ,	//���ڶ���
					   uint8_t REQ_CMD[2],		//ָ��
					   uint8_t REQ_DATS[],		//����
					   uint8_t REQdatsLen,		//���ݳ���
					   uint8_t ANSR_frame[],	//��Ӧ֡
					   uint8_t ANSRdatsLen,		//��Ӧ֡����
					   u8 times,u16 timeDelay){	//ѭ�����������εȴ�ʱ��
					   
	const u8 dataLen = 150;
	u8 	dataRXBUF[dataLen] = {0};
	u8 	dataTXBUF[dataLen] = {0};
	u8 	loop = 0;
	u8 	datsTX_Len;
	u16 local_timeDelay = timeDelay;
	
	ARM_DRIVER_USART *USARTdrv;
	
	switch(UART_OBJ){
	
		case comObj_DbugP1:			{USARTdrv = &DbugP1;}break;	
		
		case comObj_DataTransP1:	{USARTdrv = &DataTransP1;}break;		
		
		case comObj_DataTransP2:	{USARTdrv = &DataTransP2;}break;	

		default:break;
	}
	
	datsTX_Len = ZigB_TXFrameLoad(dataTXBUF,REQ_CMD,2,REQ_DATS,REQdatsLen);

	for(loop = 0;loop < times;loop ++){
	
		memset(dataRXBUF,0,dataLen * sizeof(u8));
		
		USARTdrv->Send(dataTXBUF,datsTX_Len);
		USARTdrv->Receive(dataRXBUF,dataLen);
		osDelay(20);
		
		while(local_timeDelay --){
		
			if(memmem(dataRXBUF,dataLen,ANSR_frame,ANSRdatsLen)){
			
//					DbugP1TX("123",3);	/*�������*/
//					osDelay(20);
					USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
					return true;
				}else{
					
					osDelay(1);		//������ʱ
					
//					DbugP1TX(dataRXBUF,ANSRdatsLen);	/*�������*/
//					osDelay(20);
//					
//					DbugP1TX(ANSR_frame,ANSRdatsLen);	/*�������*/
//					osDelay(20);
				}
			}local_timeDelay = timeDelay;
		
		USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
//		DbugP1TX(ANSR_frame,ANSRdatsLen);	/*�������*/
//		osDelay(20);
	}
	
	return false;
}

/*zigbeeͨ�Ŵ�����*/
bool zigb_clusterSet(type_uart_OBJ UART_OBJ,u16 deviveID,u8 endPoint){

	const datsAttr_ZigbInit default_param = {{0x24,0x00},{0x0E,0x0D,0x00,0x0D,0x00,0x0D,0x00,0x01,0x00,0x00,0x01,0x00,0x00},0x0D,{0xFE,0x01,0x64,0x00,0x00,0x65},0x06,500};	//���ݴ�ע��,Ĭ�ϲ���
	
	const u8 paramLen = 100;
	u8 paramTX_temp[paramLen] = {0};
	
	memcpy(paramTX_temp,default_param.zigbInit_reqDAT,default_param.reqDAT_num);
	paramTX_temp[0] = endPoint;
	paramTX_temp[3] = (uint8_t)((deviveID & 0x00ff) >> 0);
	paramTX_temp[4] = (uint8_t)((deviveID & 0xff00) >> 8);
	
	return zigb_VALIDA_INPUT(UART_OBJ,
							 (u8 *)default_param.zigbInit_reqCMD,
							 (u8 *)paramTX_temp,
							 default_param.reqDAT_num,
							 (u8 *)default_param.zigbInit_REPLY,
							 default_param.REPLY_num,
							 3,		//3������û����ȷ��Ӧ��ʧ��
							 default_param.timeTab_waitAnsr);
}

/*zigbee���翪�ż��رղ���*/
bool zigbNetwork_OpenIF(type_uart_OBJ UART_OBJ,bool opreat_Act){

	const datsAttr_ZigbInit default_param = {{0x26,0x08}, {0xFF,0xFF,0x00}, 0x03, {0xFE,0x01,0x66,0x08,0x00,0x6F}, 0x06, 500};	//���ݴ�ע��,Ĭ�ϲ���
	
	const u8 paramLen = 100;
	u8 paramTX_temp[paramLen] = {0};
	
	memcpy(paramTX_temp,default_param.zigbInit_reqDAT,default_param.reqDAT_num);
	if(true == opreat_Act)paramTX_temp[2] = 0xFF;
	else paramTX_temp[2] = 0x00;
	
	return zigb_VALIDA_INPUT(UART_OBJ,
							 (u8 *)default_param.zigbInit_reqCMD,
							 (u8 *)paramTX_temp,
							 default_param.reqDAT_num,
							 (u8 *)default_param.zigbInit_REPLY,
							 default_param.REPLY_num,
							 3,		//3������û����ȷ��Ӧ��ʧ��
							 default_param.timeTab_waitAnsr);	
}

/*zigbee��ʼ��*/
bool ZigB_Init(type_uart_OBJ UART_OBJ,uint16_t PANID,uint8_t CHANNELS){		

	const u8 InitCMDLen = 7;	
	
	const u8 loop_PANID		= 3;
	const u8 loop_CHANNELS	= 4;

	const datsAttr_ZigbInit ZigbInit_dats[InitCMDLen] = {
		
		{	{0x41,0x00},	{0x00},					0x01,	{0xFE,0x06,0x41,0x80,0x02,0x02,0x00,0x02,0x06,0x03,0xC0},	0x0B,	4000},	//��λ
		{	{0x26,0x05},	{0x03,0x01,0x03},		0x03,	{0xFE,0x01,0x66,0x05,0x00,0x62},							0x06,	500	},	//�Ĵ�����ʼ����ȫ�����
		{	{0x41,0x00},	{0x00},					0x01,	{0xFE,0x06,0x41,0x80,0x02,0x02,0x00,0x02,0x06,0x03,0xC0},	0x0B,	4000},	//���θ�λ
		{	{0x27,0x02},	{0x34,0x12},			0x02,	{0xFE,0x01,0x67,0x02,0x00,0x64},							0x06,	500	},	//PAN_ID�Ĵ�������
		{	{0x27,0x03},	{0x00,0x80,0x00,0x00},	0x04,	{0xFE,0x01,0x67,0x03,0x00,0x65},							0x06,	500	},	//�ŵ��Ĵ�������
		{	{0x26,0x05},	{0x87,0x01,0x00},		0x03,	{0xFE,0x01,0x66,0x05,0x00,0x62},							0x06,	500	},	//��ɫ���ã�Э������
		{	{0x26,0x00},	{0},					0x00,	{0xFE,0x01,0x45,0xC0,0x09,0x8D},							0x06,	8000},	//��ʼ�������Լȶ���ɫЭ����
	};
	
	const u8 paramLen = 100;
	u8 paramTX_temp[paramLen] = {0};
	
	u8  loop;
	u32 chnl_temp = 0x00000800 << CHANNELS;
	
	for(loop = 0;loop < InitCMDLen;loop ++){
		
		memset(paramTX_temp,0,paramLen * sizeof(uint8_t));
		
		switch(loop){	//��ѡ����&Ĭ�ϲ���
		
			case loop_PANID:{
			
				paramTX_temp[0] = (uint8_t)((PANID & 0x00ff) >> 0);
				paramTX_temp[1] = (uint8_t)((PANID & 0xff00) >> 8);
			}break;
			
			case loop_CHANNELS:{
			
				paramTX_temp[0] = (uint8_t)((chnl_temp & 0x000000ff) >>  0);
				paramTX_temp[1] = (uint8_t)((chnl_temp & 0x0000ff00) >>  8);
				paramTX_temp[2] = (uint8_t)((chnl_temp & 0x00ff0000) >> 16);
				paramTX_temp[3] = (uint8_t)((chnl_temp & 0xff000000) >> 24);
			}break;
			
			default:{
			
				memcpy(paramTX_temp,ZigbInit_dats[loop].zigbInit_reqDAT,ZigbInit_dats[loop].reqDAT_num);
			}break;
		}
	
		if(false == zigb_VALIDA_INPUT(UART_OBJ,
									  (u8 *)ZigbInit_dats[loop].zigbInit_reqCMD,
									  (u8 *)paramTX_temp,
									  ZigbInit_dats[loop].reqDAT_num,
									  (u8 *)ZigbInit_dats[loop].zigbInit_REPLY,
									  ZigbInit_dats[loop].REPLY_num,
									  3,
									  ZigbInit_dats[loop].timeTab_waitAnsr)
									 )loop = 0;
	}
	
	return zigb_clusterSet(UART_OBJ,13,13);	//�豸ID 13���ն˵� 13��
}

/*zigbee���ݽ���*/
bool ZigB_datsRX(u8 UART_OBJ,datsAttr_ZigbTrans *datsRX,u32 timeWait){

	const u8 datsRXLen = 100;
	u8 datsRX_BUF[datsRXLen] = {0};
	
	const u8 cmdRX_Len = 2;
	const u8 cmdRX[cmdRX_Len][2] = {
	
		{0x44,0x81},	//���յ���������
		{0x45,0xCA},	//���߽ڵ�����֪ͨ
	};
	
	static ARM_DRIVER_USART *USARTdrv;
	
	u8 *ptr = NULL;
	u8 loop = 0;
	
	switch(UART_OBJ){
	
		case comObj_DbugP1:			{USARTdrv = &DbugP1;}break;	
		
		case comObj_DataTransP1:	{USARTdrv = &DataTransP1;}break;		
		
		case comObj_DataTransP2:	{USARTdrv = &DataTransP2;}break;	

		default:return false;
	}
	
	USARTdrv->Receive(datsRX_BUF,datsRXLen);
	osDelay(timeWait);
	USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
	
	datsRX->datsType = zigbTP_NULL;
	
	for(loop = 0;loop < cmdRX_Len;loop ++){
	
		ptr = memmem(datsRX_BUF,datsRXLen,(u8*)cmdRX[loop],2);
		
		if(ptr != NULL){
		
			switch(loop){
			
				case 0:{
				
					if(ZIGB_FRAME_HEAD == *(ptr - 2) &&			//��Ϣ��ʽУ��	(���ʽ�������֡ͷ�������ַ)
					   *(ptr + 6) != 0 &&
					   *(ptr + 7) != 0 &&
					   *(ptr + 6) == *((ptr + *(ptr + 18)) + 19) && 	//�����ַ��λ��
					   *(ptr + 7) == *((ptr + *(ptr + 18)) + 20)){		//�����ַ��λ��
					   
						/*��֤ͨ������Ϣ��װ*/
						datsRX->datsSTT.stt_MSG.ifBroadcast = *(ptr + 10);
						datsRX->datsSTT.stt_MSG.Addr_from	= (((u16)*(ptr + 6) & 0x00FF) << 0) + (((u16)*(ptr + 7) & 0x00FF) << 8);
						datsRX->datsSTT.stt_MSG.srcEP		= *(ptr + 8);
						datsRX->datsSTT.stt_MSG.dstEP		= *(ptr + 9);
						datsRX->datsSTT.stt_MSG.ClusterID	= (((u16)*(ptr + 4) & 0x00FF) << 0) + (((u16)*(ptr + 5) & 0x00FF) << 8);
						datsRX->datsSTT.stt_MSG.LQI			= *(ptr + 11);
						datsRX->datsSTT.stt_MSG.datsLen		= *(ptr + 18);
						memset(datsRX->datsSTT.stt_MSG.dats,0,datsRX->datsSTT.stt_MSG.datsLen * sizeof(u8));
						memcpy(datsRX->datsSTT.stt_MSG.dats,(ptr + 19),*(ptr + 18));
						
						datsRX->datsType = zigbTP_MSGCOMMING;
						return true;
					}else{
					   
						datsRX_BUF[memloc((u8 *)datsRX_BUF,datsRXLen,(u8 *)cmdRX[loop],2)] = 0xFF;  //��ָ����ʽ��������Ⱦ����ʹ�䲻ͬ��������ѯ���κ�
						loop --;	//ԭ����Ϣ��󸴲�
					}
				}break;
				
				case 1:{
				
					if(ZIGB_FRAME_HEAD == *(ptr - 2)){
					
						/*��֤ͨ������Ϣ��װ*/
						datsRX->datsSTT.stt_ONLINE.nwkAddr_fresh = (((u16)*(ptr + 2) & 0x00FF) << 0) + (((u16)*(ptr + 3) & 0x00FF) << 8);
						
						datsRX->datsType = zigbTP_ntCONNECT;
						return true;
					}else{
					   
						datsRX_BUF[memloc((u8 *)datsRX_BUF,datsRXLen,(u8 *)cmdRX[loop],2)] = 0xFF;  //��ָ����ʽ��������Ⱦ����ʹ�䲻ͬ��������ѯ���κ�
						loop --;	//ԭ����Ϣ��󸴲�
					}
				}break;
				
				default:break;
			}
		}
	}
	
	return false;
}

/*zigbee���ݷ���*/
bool ZigB_datsTX(type_uart_OBJ  UART_OBJ,
				 uint16_t DstAddr,
				 uint8_t  SrcPoint,
				 uint8_t  DstPoint,
				 uint8_t  ClustID,
				 uint8_t  dats[],
				 uint8_t  datsLen,
					bool  responseIF){
					 
	const datsAttr_ZigbInit default_param = {{0x24,0x01},{0},0,{0},0,200};	//���ݷ���ָ�Ĭ��
	
	const u8 TransID = 13;
	const u8 Option	 = 0;
	const u8 Radius	 = 7;
					 
	const u8 ASR_datsLen = 3;
		  u8 ASR_dats[ASR_datsLen] = {0};
	const u8 ASR_cmd[2] = {0x44,0x80};	//����ZNPЭ���ȷ�Ϸ�����Ӧ
	
	const u8 ASR_datsDstLen = 3;
	const u8 ASR_datsDst[ASR_datsDstLen] = {0x03,0x02,0x03};	//Զ�˽���ȷ�ϣ���Ӧ��������

	const uint8_t datsTransLen = 100;
	uint8_t buf_datsTX[datsTransLen] = {0};
	uint8_t buf_datsRX[datsTransLen] = {0};
	uint8_t datsRX_Len = 0;
	
	bool TXCMP_FLG = false;
	
	datsAttr_ZigbTrans *local_datsRX = (datsAttr_ZigbTrans *) osPoolAlloc(ZigbTrans_poolAttr_id);
	
	//����֡��װ_����
	ASR_dats[0] = 0x00;
	ASR_dats[1] = SrcPoint;
	ASR_dats[2] = TransID;
	datsRX_Len = ZigB_TXFrameLoad(buf_datsRX,(u8 *)ASR_cmd,2,ASR_dats,ASR_datsLen);
	
	//����֡��װ
	buf_datsTX[0] = (uint8_t)((DstAddr & 0x00ff) >> 0);
	buf_datsTX[1] = (uint8_t)((DstAddr & 0xff00) >> 8);
	buf_datsTX[2] = DstPoint;
	buf_datsTX[3] = SrcPoint;
	buf_datsTX[4] = ClustID;
	buf_datsTX[6] = TransID;
	buf_datsTX[7] = Option;
	buf_datsTX[8] = Radius;
	buf_datsTX[9] = datsLen;
	memcpy(&buf_datsTX[10],dats,datsLen);
	
	/*���ط�����Ӧȷ��*/
	TXCMP_FLG = zigb_VALIDA_INPUT(UART_OBJ,			
								  (u8 *)default_param.zigbInit_reqCMD,
								  (u8 *)buf_datsTX,
								  datsLen + 10,
								  (u8 *)buf_datsRX,
								  datsRX_Len,
								  1,
								  default_param.timeTab_waitAnsr);
	
	/*����ѡ�� Զ�ˣ������նˣ���Ӧȷ��*/
	if(true == responseIF){
	
		if(TXCMP_FLG && ZigB_datsRX(UART_OBJ,local_datsRX,200)){	/**ע����Ժ��ʵ���Ӧʱ��**/
		
			if(local_datsRX->datsSTT.stt_MSG.Addr_from == DstAddr && !memcmp(local_datsRX->datsSTT.stt_MSG.dats,ASR_datsDst,ASR_datsDstLen)){
			
				TXCMP_FLG = true;
			}else{
			
				TXCMP_FLG = false;
			}
		}else{
		
			TXCMP_FLG = false;
		}
	}
	
	osPoolFree(ZigbTrans_poolAttr_id,local_datsRX);
	return TXCMP_FLG;
}

/*zigbee���߳�*/
void ZigB_mainThread(type_uart_OBJ UART_OBJ){

	osEvent  evt;
	
	const u8 dispLen = 150;
	char disp[dispLen];
	const u8 datsMSG_Len = 100;
	char dats_MSG[datsMSG_Len];
	
	const u8 dats_kernelTXBUFLen = 80;
	u8 dats_kernelTXBUF[dats_kernelTXBUFLen] = {0};

	osTimerId tid_ZigbDevDetect;
	const u16 ZigbDevDetect_Period = 10;	//Zigbee�ڵ��豸�����Ż�����ʱ��ֵ����λ���룩���ȶ�ʱ����δ���յ��ڵ��豸���ݣ��ýڵ㽫�������б��Ż��Ƴ�
	
	u8 loop = 0;
	
	const u8 respond_bufLen = 100;			
	u8 respond_buf[respond_bufLen] = {0};	
	bool TXCMP_FLG = false;
	
	const u8 datsTrans_cmdNum = 2;
	const sttDatsTrans_CMD datsTransCMD[datsTrans_cmdNum] = {
				
		{{0x0A,0x00},{0x0B,0x00}},		//ָ���ѯzigbee�豸�ڵ��
		{{0x0A,0x0A},{0x0B,0x0A}},		//ָ��������ݷ���
	};
	u8 cmdLoop = datsTrans_cmdNum;
	
	datsAttr_ZigbTrans *local_datsRX = (datsAttr_ZigbTrans *) osPoolAlloc(ZigbTrans_poolAttr_id);	//���ݽ��ջ���
	
	nwkStateAttr_Zigb *zigbDevList_Head = (nwkStateAttr_Zigb *) osPoolAlloc(ZigbnwkState_poolAttr_id);	//zigbee�豸�ڵ���Ϣ����
	nwkStateAttr_Zigb *ZigbDevNew_temp;
	nwkStateAttr_Zigb ZigbDevNew_tempInsert;
	
	ZigB_Init(UART_OBJ,0x1234,4);
	memset(disp,0,dispLen * sizeof(char));
	sprintf(disp,"Tips_ZigB:��ʼ�����.\r\n");	/*���Դ����*/
	DbugP1TX(disp,strlen((char *)disp));	
	osDelay(20);
	
	/*zigbee�����豸�ڵ��ⶨʱ����ʼ��*/
	tid_ZigbDevDetect = osTimerCreate(osTimer(TimerZigbDevManage), osTimerPeriodic, zigbDevList_Head);
	osTimerStart(tid_ZigbDevDetect, 1000UL);   
	
	for(;;){
		
		/*W2Z��Ϣ�������ݽ���*/
		evt = osMessageGet(mqID_threadDP_W2Z, 1);  // wait for message
		if(evt.status == osEventMessage){
		
			stt_threadDatsPass *rptr_W2Z = evt.value.p;
			
			switch(rptr_W2Z->msgType){
					
				case listDev_query:{
				
					u8 devNum = 0;
					stt_threadDatsPass *mptr_Z2W  = (stt_threadDatsPass *)osPoolAlloc(threadDP_poolAttr_id);

					mptr_Z2W->msgType = listDev_query;
					
					memset(respond_buf, 0, respond_bufLen * sizeof(u8));
					devNum = ZigBdevDispList(zigbDevList_Head,&respond_buf[1]);
					respond_buf[0] = devNum;
					memcpy(mptr_Z2W->dats.dats_devQuery.infoDevList, respond_buf, devNum * (DEVMAC_LEN + 1) + 1);		//��5�ֽ�MAC��ַ + 1�ֽ��豸���ͣ�*devNum + 1�ֽ��豸����
					mptr_Z2W->dats.dats_devQuery.infoLen = devNum * (DEVMAC_LEN + 1) + 1;
				
					osMessagePut(mqID_threadDP_Z2W, (uint32_t)mptr_Z2W, 100);
				}break;
				
				case conventional:{
					
					nwkStateAttr_Zigb *infoZigbDevRet_temp = zigbDev_eptPutout_BYpsy(zigbDevList_Head,rptr_W2Z->dats.dats_conv.macAddr,rptr_W2Z->dats.dats_conv.devType,false);
					
					if(NULL != infoZigbDevRet_temp){
					
						memset(dats_kernelTXBUF,0,dats_kernelTXBUFLen * sizeof(u8));
						
						memcpy(&dats_kernelTXBUF[0], rptr_W2Z->dats.dats_conv.macAddr, DEVMAC_LEN);
						dats_kernelTXBUF[DEVMAC_LEN] = rptr_W2Z->dats.dats_conv.devType;
						memcpy(&dats_kernelTXBUF[DEVMAC_LEN + 1],rptr_W2Z->dats.dats_conv.dats,rptr_W2Z->dats.dats_conv.datsLen);
						TXCMP_FLG = ZigB_datsTX(UART_OBJ,
												infoZigbDevRet_temp->nwkAddr,	
												13,
												13,
												13,
												(u8 *)dats_kernelTXBUF,
												rptr_W2Z->dats.dats_conv.datsLen + (DEVMAC_LEN + 1),	//5�ֽ�MAC��ַ + 1�ֽ��豸����
												true);
						
						osPoolFree(ZigbnwkState_poolAttr_id,infoZigbDevRet_temp);    
					}		
				}break;
				
				default:break;
			}
			
			osPoolFree(threadDP_poolAttr_id,rptr_W2Z);    
		}
		
		/*���ݽ��ռ�����*/
		if(true == ZigB_datsRX(UART_OBJ,local_datsRX,50)){
			
			memset(disp,0,dispLen * sizeof(char));
			memset(dats_MSG,0,datsMSG_Len * sizeof(char));
		
			switch(local_datsRX->datsType){
			
				case zigbTP_MSGCOMMING:{
					
					ZigbDevNew_temp = zigbDev_eptPutout_BYnwk(zigbDevList_Head,local_datsRX->datsSTT.stt_MSG.Addr_from,true);
					
					if(NULL == ZigbDevNew_temp){	//�豸��δ���ҵ���Ӧ�����ַ�豸�������ڵ㣬�����Ӧ�ڵ��豸���߼����������
					
						if(local_datsRX->datsSTT.stt_MSG.Addr_from != 0 && local_datsRX->datsSTT.stt_MSG.datsLen >= (datsTransCMD_length + DEVMAC_LEN + 1)){	//���ݰ������ֽ�ָ�� �� MAC��ַ���豸���ͣ��������ݳ��ȱ������8��ͬʱ�ڵ������ַ����Ϊ0
							
							DbugP1TX("node new add\n",13);
						
							ZigbDevNew_tempInsert.nwkAddr = local_datsRX->datsSTT.stt_MSG.Addr_from;
							memcpy(ZigbDevNew_tempInsert.macAddr, &(local_datsRX->datsSTT.stt_MSG.dats[datsTransCMD_length]), DEVMAC_LEN);	//�����������ֽ��豸MAC��ַ
							ZigbDevNew_tempInsert.devType = local_datsRX->datsSTT.stt_MSG.dats[datsTransCMD_length + DEVMAC_LEN];	//�ڰ��ֽ��豸����
							ZigbDevNew_tempInsert.onlineDectect_LCount = ZigbDevDetect_Period;
							ZigbDevNew_tempInsert.next = NULL;
							
							zigbDev_eptCreat(zigbDevList_Head, ZigbDevNew_tempInsert);	//zigbee�豸�������ڵ�
							zigbDev_delSame(zigbDevList_Head);	//�豸���Ż� ȥ��
						}
					}else{
					
						ZigbDevNew_temp->onlineDectect_LCount = ZigbDevDetect_Period;
						ZigbDevNew_temp = NULL;
						osPoolFree(ZigbnwkState_poolAttr_id,ZigbDevNew_temp);
					}
					
					for(loop = 0;loop < local_datsRX->datsSTT.stt_MSG.datsLen;loop ++){
					
						sprintf((char *)&dats_MSG[loop * 3],"%02X ",local_datsRX->datsSTT.stt_MSG.dats[loop]);
					}

					memset(disp,0,dispLen * sizeof(char));
					sprintf(disp,"Tips_ZigB:���յ����Խڵ������ַ��0x%04X �����ݣ�%s\r\n",local_datsRX->datsSTT.stt_MSG.Addr_from,dats_MSG);	/*���Դ����*/
					DbugP1TX(disp,strlen((char *)disp));	
					osDelay(20);
					
					for(cmdLoop = 0;cmdLoop < datsTrans_cmdNum;cmdLoop ++){
					
						if(!memcmp(local_datsRX->datsSTT.stt_MSG.dats,datsTransCMD[cmdLoop].datsCMDRX,datsTransCMD_length))break;	//��һ�ֽں͵ڶ��ֽ�Ϊָ�����Ϊ��datsTransCMD_length
					}
					
					if(cmdLoop < datsTrans_cmdNum){
					
						switch(cmdLoop){
						
							case 0:{	//ָ��0��Ӧ�����������豸������Ϣ
							
								u8 devNum = 0;
								
								memset(respond_buf, 0, respond_bufLen * sizeof(u8));
								devNum = ZigBdevDispList(zigbDevList_Head, &respond_buf[1]);
								respond_buf[0] = devNum;
								
								/*��Ӧ�ظ�*/
								TXCMP_FLG = ZigB_datsTX(UART_OBJ,
														local_datsRX->datsSTT.stt_MSG.Addr_from,	/*���Դ����*/
														13,
														13,
														13,
														(u8 *)respond_buf,
														devNum * (DEVMAC_LEN + 1) + 1,		//��5�ֽ�MAC��ַ + 1�ֽ��豸���ͣ�*devNum + ����
														true);
								
								memset(disp,0,dispLen * sizeof(char));
								sprintf(disp,"Tips_ZigB:���յ�����ָ�Zigbee�ڵ��豸�����ݻط������Ѵ���\r\n��������Զ����Ӧ�ظ��Ƿ�ɹ���%d\r\n",TXCMP_FLG);	/*���Դ����*/
								DbugP1TX(disp,strlen((char *)disp));	
								osDelay(20);
							}break;
							
							case 1:{	//ָ��1��Ӧ����������Զ�����ݴ���
							
								u8 dataTrans_offsetNum = datsTransCMD_length + DEVMAC_LEN + 1;	//֡ͷƫ�Ƴ��ȣ�2�ֽ�ָ��� + 5�ֽ�MAC��ַ + 1�ֽ��豸���ͣ�
								stt_threadDatsPass *mptr_Z2W  = (stt_threadDatsPass *)osPoolAlloc(threadDP_poolAttr_id);
								
								memset(respond_buf, 0, respond_bufLen * sizeof(u8));
								memcpy(respond_buf, local_datsRX->datsSTT.stt_MSG.dats, datsTransCMD_length);
								
								/*��Ӧ�ظ�*/
								TXCMP_FLG = ZigB_datsTX(UART_OBJ,
														local_datsRX->datsSTT.stt_MSG.Addr_from,	
														13,
														13,
														13,
														(u8 *)respond_buf,
														datsTransCMD_length,	//�������ݴ���ָ�����Ӧ�ظ�ָ������
														false);		//���������Ӧ��������Ҫ����Ӧ
								
								/*���ݷ��� ��Ϣ������WIFI*/
								mptr_Z2W->msgType = conventional;
								memcpy(mptr_Z2W->dats.dats_conv.macAddr, &(local_datsRX->datsSTT.stt_MSG.dats[datsTransCMD_length]), DEVMAC_LEN);	//�����������ֽ�MAC��ַ
								mptr_Z2W->dats.dats_conv.devType = local_datsRX->datsSTT.stt_MSG.dats[datsTransCMD_length + DEVMAC_LEN];	//�ڰ��ֽ��豸����
								memcpy(mptr_Z2W->dats.dats_conv.dats, 
									   &(local_datsRX->datsSTT.stt_MSG.dats[dataTrans_offsetNum]), 
									   local_datsRX->datsSTT.stt_MSG.datsLen - dataTrans_offsetNum
								      );
								mptr_Z2W->dats.dats_conv.datsLen = local_datsRX->datsSTT.stt_MSG.datsLen - dataTrans_offsetNum;
								
								osMessagePut(mqID_threadDP_Z2W, (uint32_t)mptr_Z2W, 100);
							}break;
							
							default:break;
						}cmdLoop = datsTrans_cmdNum;
					}
				}break;
					
				case zigbTP_ntCONNECT:{
				
					sprintf((char *)disp,"Tips_ZigB:��⵽�½ڵ����ߣ��ڵ��ַ��0x%04X\r\n",local_datsRX->datsSTT.stt_ONLINE.nwkAddr_fresh);	/*���Դ����*/
					DbugP1TX(disp,strlen((char *)disp));	/*�������*/
					osDelay(20);
				}break;
				
				default:break;
			}
		}
	}
}

/*WIFI�������Զ���У��*/
u8 frame_Check(u8 frame_temp[], u8 check_num){
  
	u8 loop 	 = 0;
	u8 val_Check = 0;
	
	for(loop = 0; loop < check_num; loop ++){
	
		val_Check += frame_temp[loop];
	}
	
	val_Check  = ~val_Check;
	val_Check ^= 0xa7;
	
	return val_Check;
}

/*WIFI����֡����*///����
u8 WIFI_TXFrameLoad(u8 frame[],u8 cmd[],u8 cmdLen,u8 dats[],u8 datsLen){

	const u8 frameHead = WIFI_FRAME_HEAD;	//SOF֡ͷ
	u8 xor_check = datsLen;					//���У�飬֡β
	u8 loop = 0;
	u8 ptr = 0;
	
	frame[ptr ++] = frameHead;
	frame[ptr ++] = datsLen;
	
	memcpy(&frame[ptr],cmd,cmdLen);
	ptr += cmdLen;
	for(loop = 0;loop < cmdLen;loop ++)xor_check ^= cmd[loop];

	memcpy(&frame[ptr],dats,datsLen);
	ptr += datsLen;
	for(loop = 0;loop < datsLen;loop ++)xor_check ^= dats[loop];	
	
	frame[ptr ++] = xor_check;
	
	return ptr;
}

/*WIFI��ָ���·�����Ӧ��֤*/
bool wifi_ATCMD_INPUT(type_uart_OBJ UART_OBJ,char *CMD,char *REPLY[2],u8 REPLY_LEN[2],u8 times,u16 timeDelay){
	
	const u8 dataLen = 100;
	u8 dataRXBUF[dataLen] = {0};
	u8 loop = 0;
	u8 loopa = 0;
	
	static ARM_DRIVER_USART *USARTdrv;
	
	switch(UART_OBJ){
	
		case comObj_DbugP1:			{USARTdrv = &DbugP1;}break;		
		
		case comObj_DataTransP1:	{USARTdrv = &DataTransP1;}break;		
		
		case comObj_DataTransP2:	{USARTdrv = &DataTransP2;}break;	

		default:break;
	}
	
	for(loop = 0;loop < times;loop ++){
	
		USARTdrv->Send(CMD,strlen(CMD));
		USARTdrv->Receive(dataRXBUF,dataLen);
		osDelay(timeDelay);
		osDelay(20);
		USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
		
		for(loopa = 0;loopa < 2;loopa ++)if(memmem(dataRXBUF,dataLen,REPLY[loopa],REPLY_LEN[loopa]))return true;
	}return false;
}

/*WIFI��ʼ��*/
void WIFI_Init(type_uart_OBJ UART_OBJ){

	const u8 InitCMDLen = 10;
	const datsAttr_wifiInit wifiInit_dats[InitCMDLen] = {
	
		{"ATE0\r\n",															{"OK","OK"},	{2,2},	100},
		{"ATE0\r\n",															{"OK","OK"},	{2,2},	100},
		{"AT\r\n",																{"OK","OK"},	{2,2},	100},
		{"AT+CWMODE_DEF=3\r\n",													{"OK","OK"},	{2,2},	100},
		{"AT+CWSAP_DEF=\"Zigbee&WIFI_NG\",\"gta114605\",5,3\r\n",				{"OK","OK"},	{2,2},	100},
		{"AT+CWDHCP_DEF=2,0\r\n",												{"OK","OK"},	{2,2},	100},
		{"AT+CIPAP=\"192.168.6.1\",\"192.168.6.1\",\"255.255.255.0\"\r\n",		{"OK","OK"},	{2,2},	100},
		{"AT+CIPSTA=\"192.168.7.1\",\"192.168.7.1\",\"255.255.255.0\"\r\n",		{"OK","OK"},	{2,2},	100},
		{"AT+CIPMUX=1\r\n",														{"OK","OK"},	{2,2},	100},
		{"AT+CIPSERVER=1,8088\r\n",												{"OK","OK"},	{2,2},	100},
	};
	
	u8 loop;
	
	static ARM_DRIVER_USART *USARTdrv;
	
	switch(UART_OBJ){
	
		case comObj_DbugP1:			{USARTdrv = &DbugP1;}break;	
		
		case comObj_DataTransP1:	{USARTdrv = &DataTransP1;}break;		
		
		case comObj_DataTransP2:	{USARTdrv = &DataTransP2;}break;	

		default:return;
	}
	
	USARTdrv->Send("+++",3);osDelay(100);
	USARTdrv->Send("+++\r\n",5);osDelay(100);
	USARTdrv->Send("+++",3);osDelay(100);
	USARTdrv->Send("+++\r\n",5);osDelay(100);
	
	for(loop = 0;loop < InitCMDLen;loop ++)
		if(false == wifi_ATCMD_INPUT(UART_OBJ,
									 (char *)wifiInit_dats[loop].wifiInit_reqCMD,
									 (char **)wifiInit_dats[loop].wifiInit_REPLY,
									 (u8 *)wifiInit_dats[loop].REPLY_DATLENTAB,
									 3,
									 wifiInit_dats[loop].timeTab_waitAnsr)
									 )loop = 0;
									 
	USARTdrv->Send("AT\r\n",4); //���ڻ������
	osDelay(10);
}

/*WIFI�������Ӽ��*/
u8 WIFI_CONNECT_DETECT(u8 UART_OBJ){

	const u16 dataLen = 500;
	char dataRXBUF[dataLen] = {0};
	char *ptr;
	char port;
	char CONNECT_HOOK = 0;
	u8 buff_num = 0;
	
	static ARM_DRIVER_USART *USARTdrv;
	
	switch(UART_OBJ){
	
		case comObj_DbugP1:			{USARTdrv = &DbugP1;}break;
		
		case comObj_DataTransP1:	{USARTdrv = &DataTransP1;}break;		
		
		case comObj_DataTransP2:	{USARTdrv = &DataTransP2;}break;

		default:return false;
	}
	
	memset(dataRXBUF,0,dataLen * sizeof(u8));
	
	USARTdrv->Send("AT+CIPSTATUS\r\n",14);
	USARTdrv->Receive(dataRXBUF,dataLen);
	delay_ms(30);	//30ms Ϊ���Լ���ֵ��������С�ˣ�
//	DbugP2TX(dataRXBUF,dataLen);	/*�������*/
//	delay_ms(30);					/*�������*/
	USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
	
	for(;;){
	
		ptr = memmem(dataRXBUF,dataLen,"+CIPSTATUS:",11);
		if(ptr){
		
			ptr += 11;
			port = *ptr - '0';
			CONNECT_HOOK |= 1 << port; //��Ӧ����״̬λ��λ
			
//			DbugP2TX(&CONNECT_HOOK,1);	/*�������*/
//			delay_ms(30);				/*�������*/
			
			buff_num = strloc(dataRXBUF,"+CIPSTATUS:") + 13;
			memcpy(dataRXBUF,&dataRXBUF[buff_num],strlen(&dataRXBUF[buff_num]));	//��������ݽ�ǰ���ѽ������ݸ��ǵ�
		}else return CONNECT_HOOK;
	}
}

/*WIFI���ݽ���*/
bool WIFI_datsRX(u8 UART_OBJ,datsAttr_WIFITrans *datsRX,u32 timeWait){

	const u8 datsRXLen = 100;
	u8 datsRX_BUF[datsRXLen] = {0};
	
	const u8 cmdRX_Len = 3;
	const char cmdRX[cmdRX_Len][15] = {
	
		"+IPD,",
		",CONNECT",
		",CLOSED",
	};
	
	u8 *ptr = NULL;
	u8 xorLocation = 0;
	u8 loop = 0;
	
	static ARM_DRIVER_USART *USARTdrv;
	
	switch(UART_OBJ){
	
		case comObj_DbugP1:			{USARTdrv = &DbugP1;}break;
		
		case comObj_DataTransP1:	{USARTdrv = &DataTransP1;}break;		
		
		case comObj_DataTransP2:	{USARTdrv = &DataTransP2;}break;
		
		default:return false;
	}
	
	USARTdrv->Receive(datsRX_BUF,datsRXLen);
	osDelay(timeWait);
	USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
	
	datsRX->datsType = wifiTP_NULL;
	
	for(loop = 0;loop < cmdRX_Len;loop ++){
	
		ptr = memmem(datsRX_BUF,datsRXLen,(u8*)cmdRX[loop],strlen(cmdRX[loop]));
		
		if(ptr != NULL){
		
			switch(loop){
			
				case 0:{
					
					u8 *ptr_numCfr = ptr;		//��ѯ���ݳ������Ȩλ  ���ȨλΪ����ʮ����ʱascii��ռλ����ͬ
					u8 datsKernelLen = 0;		//�������ݳ���
					u8 loop_a = 0;				//ռλͳ��
					u8 Aweight = 1;				//Ȩλֵ
//					char disp[100] = {0};		/*���Դ����*/
					
					while(*(ptr_numCfr ++) != ':');
					ptr_numCfr -= 2; //�ؼ�2 ����ָ��':'ǰ��λ�±�
					
//					sprintf(disp,"���ǲ��Ե�1���ݣ�%c \r\n",*ptr_numCfr);
//					DbugP1TX(disp,strlen(disp));		/*���Դ����*/
//					osDelay(20);
					
					while(*ptr_numCfr != ','){
					
						loop_a ++;			//ռλͳ��
						datsKernelLen += Aweight * (*ptr_numCfr - '0');
						Aweight *= 10;		//ʮ����Ȩֵ����
						ptr_numCfr --;
					}
					
//					sprintf(disp,"���ǲ��Ե�2���ݣ�%d \r\n",datsKernelLen);
//					DbugP1TX(disp,strlen(disp));		/*���Դ����*/
//					osDelay(20);
					
					xorLocation = datsKernelLen + 7 + loop_a;
					if(*(ptr + 8 + loop_a) == WIFI_FRAME_HEAD && *(ptr + xorLocation) == XOR_CHECK((ptr + 9 + loop_a),datsKernelLen - 2)){		//֡ͷУ�鼰֡β���У��
						
						datsRX->linkObj = *(ptr + 5) - '0';
						memcpy(datsRX->CMD,ptr + 10 + loop_a,2);
						memcpy(datsRX->dats,ptr + 12 + loop_a,*(ptr + 9 + loop_a));
						datsRX->datsLen = *(ptr + 9 + loop_a);
						
						datsRX->datsType = wifiTP_MSGCOMMING;
						return true;
					}
				}break;

				case 1:{
					
//					DbugP1TX("���ǲ��Ե�\r\n",12);		/*���Դ����*/
//					osDelay(20);
				
					datsRX->linkObj = *(-- ptr) - '0';
					
					datsRX->datsType = wifiTP_ntCONNECT;
					return true;
				}
				
				case 2:{
				
					datsRX->linkObj = *(-- ptr) - '0';
					
					datsRX->datsType = wifiTP_ntDISCONNECT;
					return true;
				}
			}
		}
	}
	
	return false;
}

/*WIFI���ݷ���*/
bool WIFI_datsTX(type_uart_OBJ UART_OBJ,
				 uint8_t linkObj,
				 uint8_t CMD[],
				 uint8_t CMDLen,
				 uint8_t dats[],
				 uint8_t datsLen,
					bool responseIF){
				
	const u16 dataLen = 100;
	u8 dataRXBUF[dataLen] = {0};
	u8 dataTXBUF[dataLen] = {0};
	const u16 datsSdyLen = 40;
	u8 datsSdy[datsSdyLen] = {0};
	u8 lengthTX = 0;
	
	const u8 ASR_CMDDstLen = 2;
	const u8 ASR_CMDDst[ASR_CMDDstLen] = {0x0A,0xFF};	//�����˽���ȷ�ϣ���Ӧ����ָ������

	u16	timeOut_cnt  = 0;
	const u16 tOut_period = 1000; //��ʱ���ã���ֹ���������˹�С
	
	bool TXCMP_FLG = false;
	
	datsAttr_WIFITrans *local_datsRX = (datsAttr_WIFITrans *) osPoolAlloc(WIFITrans_poolAttr_id);
	
	static ARM_DRIVER_USART *USARTdrv;
	
	switch(UART_OBJ){
	
		case comObj_DbugP1:			{USARTdrv = &DbugP1;}break;	
		
		case comObj_DataTransP1:	{USARTdrv = &DataTransP1;}break;		
		
		case comObj_DataTransP2:	{USARTdrv = &DataTransP2;}break;	

		default:return false;
	}
	
	lengthTX = WIFI_TXFrameLoad(dataTXBUF,CMD,CMDLen,dats,datsLen);
	sprintf((char *)datsSdy,"AT+CIPSEND=%d,%d\r\n",linkObj,lengthTX);
	USARTdrv->Send(datsSdy,strlen((const char*)datsSdy));
	USARTdrv->Receive(dataRXBUF,dataLen);
	delay_ms(10);
	while(!memmem(dataRXBUF,dataLen,">",1) && timeOut_cnt < tOut_period){
	
		delay_ms(5); 
		timeOut_cnt ++;
		if(memmem(dataRXBUF,dataLen,"link is not valid",17)){
		
			timeOut_cnt = tOut_period + 1;	//���Ӳ����ڣ���������
			break;
		}
	}USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
	
	if(timeOut_cnt >= tOut_period){
	
		return false;
	}else{
	
		timeOut_cnt = 0;
		
		memset(dataRXBUF,0,dataLen * sizeof(u8));
		USARTdrv->Send(dataTXBUF,lengthTX);
		USARTdrv->Receive(dataRXBUF,dataLen);
		delay_ms(10);
		
		while(!memmem(dataRXBUF,dataLen,"SEND OK",7) && timeOut_cnt < tOut_period){delay_ms(5); timeOut_cnt ++;}
		USARTdrv->Control(ARM_USART_ABORT_RECEIVE,0);
		if(timeOut_cnt >= tOut_period)return false;
		delay_ms(10);
		
		TXCMP_FLG = true;
	}
	
	/*����ѡ�� Զ�ˣ������նˣ���Ӧȷ��*/
	if(true == responseIF){
	
		if(TXCMP_FLG && true == WIFI_datsRX(UART_OBJ,local_datsRX,200)){	/**ע����Ժ��ʵ���Ӧʱ��**/
		
			if(wifiTP_MSGCOMMING == local_datsRX->datsType && !memcmp(local_datsRX->CMD,ASR_CMDDst,ASR_CMDDstLen)){
			
				TXCMP_FLG = true;
			}else{
			
				TXCMP_FLG = false;
			}
		}else{
		
			TXCMP_FLG = false;
		}
	}
	
	osPoolFree(WIFITrans_poolAttr_id,local_datsRX);
	return TXCMP_FLG;
}

/*WIFI���߳�*/
void WIFI_mainThread(type_uart_OBJ UART_OBJ){
	
	osEvent  evt;
	
	const u8 dispLen = 150;
	char disp[dispLen];
	
	const u8 dats_kernelTXBUFLen = 80;
	u8 dats_kernelTXBUF[dats_kernelTXBUFLen] = {0};
	
	const u8 datsTrans_cmdNum = 2;
	const sttDatsTrans_CMD datsTransCMD[datsTrans_cmdNum] = {	//��������ָ�� �� ��Ӧ����������ָ��
	
		{{0x0A,0x00},{0x0B,0x00}},		//ָ���ѯzigbee�豸�ڵ��
		{{0x0A,0x0A},{0x0B,0x0A}},		//ָ��������ݷ���
	};
	u8 cmdLoop = datsTrans_cmdNum;
	
	u8 loop;
	
	u8	wifiContDec_cnt = 0;
	const u8 wifiCDec_period = 100; //wifi�ѽ���������Ŀ������ڣ�������⣩
	char CONNECT_HOOK = 0; //������������bit0��������0��bit1��������1���Դ����ƣ�ͬһʱ�����5������;
	
	datsAttr_WIFITrans *local_datsRX = (datsAttr_WIFITrans *) osPoolAlloc(WIFITrans_poolAttr_id);
	
	WIFI_Init(UART_OBJ);	
	memset(disp,0,dispLen * sizeof(char));
	snprintf(disp,dispLen,"Tips_WIFI:WIFI��ʼ�����.\r\n");
	DbugP1TX(disp,strlen((char *)disp));	
	osDelay(20);
	
	for(;;){
		
		/*�������Ӽ��*/
		if(wifiContDec_cnt < wifiCDec_period)wifiContDec_cnt ++;
		else{
		
			wifiContDec_cnt = 0;
			CONNECT_HOOK = WIFI_CONNECT_DETECT(UART_OBJ); //������Ŀ���
//			CONNECT_HOOK = 0x1f; //������Ŀ����ֵ/*�������*/
		}
		
		/*Z2W��Ϣ�������ݽ���*/
		evt = osMessageGet(mqID_threadDP_Z2W, 1);  // wait for message
		if(evt.status == osEventMessage){
		
			stt_threadDatsPass *rptr_Z2W = evt.value.p;
			switch(rptr_Z2W->msgType){
			
				case listDev_query:{		//��ZigB���̻߳�ȡ�ڵ��豸������Ϣ�����λش�
				
					if(CONNECT_HOOK){
					
						memset(dats_kernelTXBUF,0,dats_kernelTXBUFLen * sizeof(u8));
						memcpy(dats_kernelTXBUF,rptr_Z2W->dats.dats_devQuery.infoDevList,rptr_Z2W->dats.dats_devQuery.infoLen);
						for(loop = 0;loop < 5;loop ++){
						
							if(CONNECT_HOOK & (1 << loop)){
								
								bool TEST_TXCMPFLAG = false;
										
								TEST_TXCMPFLAG = WIFI_datsTX(UART_OBJ,loop,(u8 *)datsTransCMD[0].datsCMDTX,datsTransCMD_length,(u8 *)dats_kernelTXBUF,rptr_Z2W->dats.dats_devQuery.infoLen,true);
								
								memset(disp,0,dispLen * sizeof(char));
								snprintf(disp,dispLen,"��⵽����ָ�����ݣ�����ZigB�ڵ��豸������Ϣ���ݻط�����\r\n��������Զ����Ӧ�ظ��Ƿ�ɹ���%d\r\n",TEST_TXCMPFLAG);	/*���Դ����*/
								DbugP1TX(disp,strlen((char *)disp));	
								osDelay(20);
							}
						}
					}
				}break;
				
				case conventional:{
				
					if(CONNECT_HOOK){
					
						memset(dats_kernelTXBUF,0,dats_kernelTXBUFLen * sizeof(u8));
						memcpy(&dats_kernelTXBUF[0], rptr_Z2W->dats.dats_conv.macAddr, DEVMAC_LEN);
						dats_kernelTXBUF[DEVMAC_LEN] = rptr_Z2W->dats.dats_conv.devType;
						memcpy(&dats_kernelTXBUF[DEVMAC_LEN + 1],rptr_Z2W->dats.dats_conv.dats,rptr_Z2W->dats.dats_conv.datsLen);
						for(loop = 0;loop < 5;loop ++){
						
							if(CONNECT_HOOK & (1 << loop)){
							
								WIFI_datsTX(UART_OBJ,loop,(u8 *)datsTransCMD[1].datsCMDTX,datsTransCMD_length,(u8 *)dats_kernelTXBUF,rptr_Z2W->dats.dats_conv.datsLen + datsTransCMD_length,true);
							}
						}
					}
				}break;
				
				default:break;
			}
			
			osPoolFree(threadDP_poolAttr_id,rptr_Z2W);    
		}
		
		/*���ƶ�ָ�����ݽ���*/
		if(true == WIFI_datsRX(UART_OBJ,local_datsRX,50)){
		
			memset(disp,0,dispLen * sizeof(char));
		
			switch(local_datsRX->datsType){
				
				case wifiTP_MSGCOMMING:{
				
					const u8 datsMSG_Len = 100;
					char dats_MSG[datsMSG_Len] = {0};
					
					memset(dats_MSG,0,datsMSG_Len * sizeof(char));
					
					for(loop = 0;loop < local_datsRX->datsLen;loop ++){
					
						sprintf((char *)&dats_MSG[loop * 3],"%02X ",local_datsRX->dats[loop]);
					}
					
					memset(disp,0,dispLen * sizeof(char));
					snprintf(disp,dispLen,"Tips_WIFI:���յ��������ӵ�Ԫ��%d ��ָ�0x%02X%02X  ���������������ݣ�%s \r\n",local_datsRX->linkObj,local_datsRX->CMD[0],local_datsRX->CMD[1],dats_MSG);
					DbugP1TX(disp,strlen((char *)disp));	
					osDelay(20);
					
					for(cmdLoop = 0;cmdLoop < datsTrans_cmdNum;cmdLoop ++){
					
						if(!memcmp(local_datsRX->CMD,datsTransCMD[cmdLoop].datsCMDRX,datsTransCMD_length))break;	//��һ�ֽں͵ڶ��ֽ�Ϊָ�����Ϊ��datsTransCMD_length
					}
					
					if(cmdLoop < datsTrans_cmdNum){
					
						switch(cmdLoop){
						
							case 0:{	//ָ��0��Ӧ����	zigbee�ڵ��豸����Ϣ��ȡ����
								
								stt_threadDatsPass *mptr_W2Z  = (stt_threadDatsPass *)osPoolAlloc(threadDP_poolAttr_id);
								
								mptr_W2Z->msgType = listDev_query;
								
								osMessagePut(mqID_threadDP_W2Z, (uint32_t)mptr_W2Z, 100);
							}break;
							
							case 1:{	//ָ��1��Ӧ���� �����������ݴ�������
							
								stt_threadDatsPass *mptr_W2Z  = (stt_threadDatsPass *)osPoolAlloc(threadDP_poolAttr_id);
								
								mptr_W2Z->msgType = conventional;
								
								memcpy(mptr_W2Z->dats.dats_conv.macAddr, &local_datsRX->dats[0], DEVMAC_LEN);
								mptr_W2Z->dats.dats_conv.devType = local_datsRX->dats[DEVMAC_LEN];
								memcpy(mptr_W2Z->dats.dats_conv.dats, &local_datsRX->dats[DEVMAC_LEN + 1], local_datsRX->datsLen - (DEVMAC_LEN + 1));
								mptr_W2Z->dats.dats_conv.datsLen = local_datsRX->datsLen - (DEVMAC_LEN + 1);
								osMessagePut(mqID_threadDP_W2Z, (uint32_t)mptr_W2Z, 100);
							}break;
							
							default:break;
						}cmdLoop = datsTrans_cmdNum;
					}
				}break;
				
				case wifiTP_ntCONNECT:{
				
					CONNECT_HOOK |= 1 << local_datsRX->linkObj;
					snprintf(disp,dispLen,"Tips_WIFI:��Ԫ��%d �����Ѵ���\r\n",local_datsRX->linkObj);
					DbugP1TX(disp,strlen((char *)disp));	
					osDelay(20);
				}break;
				
				case wifiTP_ntDISCONNECT:{
				
					snprintf(disp,dispLen,"Tips_WIFI:��Ԫ��%d �����ѶϿ�\r\n",local_datsRX->linkObj);
					CONNECT_HOOK &= 0 << local_datsRX->linkObj;
					DbugP1TX(disp,strlen((char *)disp));	
					osDelay(20);
				}break;
				
				default:break;
			}
		}
	}
}

/*WIFI���̼߳���*/
void dataTransThread_Active(const void *argument){

	paramLaunch_OBJ *datsTransActOBJ = (paramLaunch_OBJ *)argument;
	
	switch(datsTransActOBJ->funTrans_OBJ){
	
		case ftOBJ_ZIGB:{
		
			ZigB_mainThread(datsTransActOBJ->uart_OBJ);
		}break;
		
		case ftOBJ_WIFI:{
		
			WIFI_mainThread(datsTransActOBJ->uart_OBJ);
		}break;
		
		case ftOBJ_DEBUG:{
		
			
		}break;
		
		default:break;
	}
	
	osPoolFree(dttAct_poolAttr_id,datsTransActOBJ);
}

void com1DbugP1_Thread(const void *argument){
	
	dataTransThread_Active(argument);

	for(;;){
	
		osDelay(100);
	}
}

void com2DataTransP1_Thread(const void *argument){
	
	dataTransThread_Active(argument);
}

void com5DataTransP2_Thread(const void *argument){

	dataTransThread_Active(argument);
}

/*�����Ż���ʱ���ص�����*/
void TimerZigbDevManage_Callback(void const *arg){
	
	const u8 dispLen = 150;
	char disp[dispLen];

	nwkStateAttr_Zigb *pHead_listDevInfo = (nwkStateAttr_Zigb *)arg;
	
	zigbDev_delSame(pHead_listDevInfo);		//�����Ż���ȥ��
	
//	DbugP1TX("ZigBdev Online detect counting tips\r\n",37);	
//	osDelay(20);
	
	if(0 == zigbDev_chatLenDectect(pHead_listDevInfo)){	//�����޽ڵ㣬ֱ�ӷ���
		
		osDelay(10);
		return;
	}

	while(pHead_listDevInfo->next != NULL){
	
		pHead_listDevInfo = pHead_listDevInfo->next;
		
		if(pHead_listDevInfo->onlineDectect_LCount > 0)pHead_listDevInfo->onlineDectect_LCount --;
		else{
			
			u16 p_nwkRemove = pHead_listDevInfo->nwkAddr;
			
			while(false == zigbDev_eptRemove_BYnwk((nwkStateAttr_Zigb *)arg,p_nwkRemove));
		
			memset(disp,0,dispLen * sizeof(char));
			snprintf(disp,dispLen,"Tips_ZigB:���ڽڵ��豸�����������ַΪ:0x%04X �Ľڵ��豸�ڼȶ�ʱ���������ݴ����ѱ������Ż�.\r\n",p_nwkRemove);
			DbugP1TX(disp,strlen((char *)disp));	
			osDelay(20);
		}
	}
}

void osMemoryInit(void){

	ZigbTrans_poolAttr_id 	 = osPoolCreate(osPool(datsZigbTrans_pool));		//zigbee���ݴ��� ���ݽṹ���ڴ�س�ʼ��
	ZigbnwkState_poolAttr_id = osPoolCreate(osPool(nwkZigbDevStateAttr_pool));	//zigbee�ڵ��豸��Ϣ ���ݽṹ���ڴ�س�ʼ��
	WIFITrans_poolAttr_id	 = osPoolCreate(osPool(datsWIFITrans_pool));		//wifi���ݴ��� ���ݽṹ���ڴ�س�ʼ��
	dttAct_poolAttr_id	 	 = osPoolCreate(osPool(dtThreadActInitParm_pool));	//���ݴ�����̳�ʼ����Ϣ ���ݽṹ���ڴ�س�ʼ��
}

void msgQueueInit(void){

	threadDP_poolAttr_id	= osPoolCreate(osPool(threadDatsPass_pool));		//zigbee��wifi���߳����ݴ�����Ϣ���� ���ݽṹ���ڴ�س�ʼ��
	
	mqID_threadDP_Z2W = osMessageCreate(osMessageQ(MsgBox_threadDP_Z2W), NULL);	//zigbee��wifi��Ϣ���г�ʼ��
	mqID_threadDP_W2Z = osMessageCreate(osMessageQ(MsgBox_threadDP_W2Z), NULL);	//wifi��zigbee��Ϣ���г�ʼ��
}

void communicationActive(type_uart_OBJ comObj,type_ftOBJ ftTransObj){
	
	paramLaunch_OBJ *param_DatsTransAct = (paramLaunch_OBJ *)osPoolAlloc(dttAct_poolAttr_id);
	
	param_DatsTransAct->uart_OBJ 		= comObj;
	param_DatsTransAct->funTrans_OBJ 	= ftTransObj;

	switch(param_DatsTransAct->uart_OBJ){
	
		case comObj_DbugP1:		USART1_DbugP1_Init();
								tid_com1DbugP1_Thread = osThreadCreate(osThread(com1DbugP1_Thread),param_DatsTransAct);
								break;
				
		case comObj_DataTransP1:USART2_DataTransP1_Init();
								tid_com2DataTransP1_Thread = osThreadCreate(osThread(com2DataTransP1_Thread),param_DatsTransAct);
								break;
				
		case comObj_DataTransP2:USART5_DataTransP2_Init();
								tid_com5DataTransP2_Thread = osThreadCreate(osThread(com5DataTransP2_Thread),param_DatsTransAct);
								break;
		
				   default:		break;
	}	
}
