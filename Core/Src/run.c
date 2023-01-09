#include "run.h"
#include "gpio.h"
#include "cmd_link.h"
#include "led.h"
#include "display.h"
#include "lcd.h"

RUN_T run_t;

uint8_t tim3,tim14;
void Power_Off(void);

/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Receive_ManiBoard_Cmd(uint8_t cmd)
{
	switch(cmd){


		   case WIFI_POWER_ON: //turn on 
		 	
          
              run_t.gTimes_hours_temp=12;
              run_t.gPower_On=1;
          
			  run_t.power_key =1;
			  run_t.gFan_RunContinue=0;
			 
			  run_t.gModel =1; //WT.EDIT 2022.09.01
			  run_t.gWifi =1;

			  run_t.gDry =1;
			  run_t.gPlasma=1;
              run_t.gBug =1;
		      run_t.gPower_On =1;
			
               Display_Temperature_Humidity_Value();
               run_t.wifi_turn_off ++;
	     
              cmd=0xff;

	         break;

			 case WIFI_POWER_OFF: //turn off 
                
			
			    run_t.gFan_RunContinue=1;
	            run_t.gPower_On=0;
				run_t.fan_off_60s =0;
	            run_t.gFan_RunContinue=1; //WT.EDIT 2022.08.31
				run_t.fan_off_60s = 0;
                //Display_Temperature_Humidity_Value();
                 run_t.wifi_turn_on ++;
               cmd=0xff;

			 break;

			case WIFI_MODE_1: //AI turn on -> AI icon display 
                if(run_t.gPower_On==1)
				     run_t.gModel =1; //0-> has ,1->no gModel
                   
			break;

			 case WIFI_MODE_2: //icon don't display 
                 if(run_t.gPower_On==1)
				 run_t.gModel =2; //turon off AI mode
			 	   
                 
             break;

			 case WIFI_KILL_ON: //kill turn on plasma
			  if(run_t.gPower_On==1){
                    run_t.gPlasma = 1;
			        run_t.gFan_RunContinue =0;
                } 
			 break;

			 case WIFI_KILL_OFF: //kill turn off
                if(run_t.gPower_On==1){
			 	  run_t.gPlasma =0;
				  
		          run_t.gFan_RunContinue =0;
                }
			 break;

			 case WIFI_PTC_ON://dry turn on
                if(run_t.gPower_On==1){
			        run_t.gDry =1;
                    run_t.gFan_RunContinue =0;
                 
                }
			 break;

			 case WIFI_PTC_OFF: //dry turn off
               
			 	if(run_t.gPower_On==1){
					run_t.gDry=0;
                 
		            run_t.gFan_RunContinue =0;
			 	}

			 break;

			 case WIFI_SONIC_ON:  //drive bug
		
				 if(run_t.gPower_On==1){		   
				  run_t.gBug =1; //turn on 
			
				 run_t.gFan_RunContinue =0;
			    }

			 break;

			 case WIFI_SONIC_OFF: //drive bug turn off
			 	if(run_t.gPower_On==1){
				    run_t.gBug=0;
					run_t.gFan_RunContinue =0;
			   }
			 break;

			 case  WIFI_WIND_SPEED_ITEM :
                if(run_t.gPower_On==1){


                }

			 break;

			 case WIFI_TEMPERATURE:
 				if(run_t.gPower_On==1){


                }

			 break;


	         default :
                  cmd =0;
			 break;

			 
        }
   
}


/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Power_Off(void)
{
    	 if(run_t.gPower_On ==0){

			     run_t.gPower_On =0xff;
			 	 // Smg_AllOff();
				
		         run_t.gModel =0;
				
				 run_t.gPlasma=0;
				 run_t.gDry=0;
				
				 run_t.gTemperature =20;
				

				  run_t.gTimer_Cmd=0; //timer of command "1"->timer is start
				  run_t.dispTime_hours=0;
				  run_t.dispTime_minute=0;
				  run_t.gTemperature_timer_flag=0;
			
			
			
				  run_t.gDht11_flag=0; //the first power on display "00"
				  run_t.gTimes_hours_temp=12;
				  run_t.gTimes_minutes_temp=0;
			
				  run_t.gKeyTimer_mode =0;
				  run_t.gTimer_key_5s=0;
				  run_t.gTimer_key_4s=0;
	              run_t.gTimer_key_60s=0;
				  run_t.gTimer_fan_counter=0;
				  Lcd_PowerOff_Fun();
				
				//  SendData_PowerOff(0);//ShutDown_AllFunction();
				 // ALL_LED_Off();
				  
                  
		}
		Breath_Led();
		//SMG_POWER_OFF()	;
		
	      //power off display fan run times 60s
          if(run_t.gFan_RunContinue==1){

		      if(run_t.fan_off_60s > 60){
                run_t.gFan_RunContinue=0;
                
              }
			  else{

			   
							
           }

		 }
}

void DisplayTimer_Timing(void)
{
      static uint8_t m,n,p,q;
    
    if(run_t.gTimer_Cmd==1 &&  run_t.temperature_flag !=1){

                
				m = run_t. dispTime_hours /10 ;
			    n=	run_t. dispTime_hours %10; 
				p = run_t. dispTime_minute /10;
				q=  run_t. dispTime_minute %10;
			   // TM1639_Write_4Bit_Data(m,n,p,q,0) ; //timer is default 12 hours "12:00"
			   //hours
		 lcd_t.number5_high = m;
		 lcd_t.number5_low = m;

		  lcd_t.number6_high = n;
		 lcd_t.number6_low = n;

		 //minutes 
		 lcd_t.number7_high = p;
		 lcd_t.number7_low = p;

		  lcd_t.number8_high = q;
		 lcd_t.number8_low = q;

		 DisplayPanel_Ref_Handler();
		
			   

  }
}


/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 10ms interrupt call back function call back function
	*
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t tm0,tm2;
    
   if(htim->Instance==TIM3){
    
    tm0++;  //10ms
	
    tim3++;
    lcd_t.gTimer_wifi_500ms++;
	lcd_t.gTimer_fan_10ms++;

    if(tm0>99){ //100 *10ms = 1000ms = 1s
		tm0=0;
		tm2++;
		
		run_t.gTimer_key_4s ++;
		run_t.gTimer_key_60s++;
		run_t.fan_off_60s++;
		 run_t.gTimer_led_500ms =1;
		 run_t.gTimer_key_5s++;
	     run_t.gTimer_disp_temp++;
		 run_t.gTimer_set_temperature++;
		 run_t.beijing_time_seconds++;
	    if(tm2>59){//60s ->1 minute 
	      tm2=0;
		   run_t.gTimer_1_hour_counter++;
		   run_t.gTimer_minute_Counter ++;
		 
		  if(run_t.gTimer_1_hour_counter> 59){ // 1 hour
	        run_t.gTimer_1_hour_counter=0;
			
	         run_t.gTimer_1hour =1;

		  }
		
	    
		}
     

   }
	
   }


}
	
