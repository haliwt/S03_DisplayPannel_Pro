#ifndef __CMD_LINK_H_
#define __CMD_LINK_H_

#include "main.h"
#define  MAX_BUFFER_SIZE   8

enum Signal{
	PANEL_DATA=1,
	WIFI_INFO,
	WIFI_TIME,
	WIFI_REAL_TEMP,
	WIFI_CMD,
	WIFI_WIND_SPEED,
	WIFI_BEIJING_TIME,
	WIFI_SET_TIMING,
	WIFI_SET_TEMPERATURE
	
};



extern uint8_t inputBuf[MAX_BUFFER_SIZE];
extern uint8_t outputBuf[8];


void SendData_PowerOff(uint8_t index);

void SendData_Set_Wifi(uint8_t hdata);

void SendData_Set_Command(uint8_t hdata);
void SendData_Temp_Data(uint8_t tdata);
void SendData_Time_Data(uint8_t tdata);


void SendData_Buzzer(void);

void USART1_Cmd_Error_Handler(void);




#endif 




