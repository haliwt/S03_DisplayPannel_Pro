#include "run.h"
#include "gpio.h"
#include "cmd_link.h"
#include "led.h"
#include "display.h"
#include "lcd.h"

RUN_T run_t;

uint8_t tim3,tim14;



/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 10ms interrupt call back function call back function
	*
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t tm0,tm1;
    
   if(htim->Instance==TIM3){
    
    tm0++;  //10ms
	tm1++;
 
    lcd_t.gTimer_wifi_500ms++;
	lcd_t.gTimer_fan_10ms++;
	
    if(tm1>9){ //100ms 
       tm1=0;
       lcd_t.gTimer_colon_ms++ ;
	   run_t.gTimer_numbers_one_two_blink++;
    }

    if(tm0>99){ //100 *10ms = 1000ms = 1s
		tm0=0;
		
		run_t.fan_off_60s++;
        run_t.gTimer_wifi_connect_counter++;
		run_t.dispTime_seconds++;
		run_t.gTimer_temp_delay++ ;
	    if(run_t.dispTime_seconds >59){//60s ->1 minute 
	      run_t.dispTime_seconds=0;
		  run_t.gTimer_minute_Counter ++;
		 
		 }
     

   }
	
   }


}
	
