#ifndef __CMD_LINK_H_
#define __CMD_LINK_H_

#include "main.h"
#define  MAX_BUFFER_SIZE   8

enum Signal{
	PANEL_DATA=1,
	WIFI_INFO,
	WIFI_TIME,
	WIFI_TEMP,
	WIFI_CMD,
	WIFI_WIND_SPEED,
	WIFI_BEIJING_TIME
};

extern uint8_t inputBuf[MAX_BUFFER_SIZE];
extern uint8_t outputBuf[8];


void SendData_PowerOff(uint8_t index);

void SendData_Set_Wifi(uint8_t hdata);




void SendData_Buzzer(void);





#endif 




