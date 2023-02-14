#include "single_mode.h"
#include "run.h"
#include "display.h"
#include "gpio.h"
#include "cmd_link.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

uint16_t k;
uint8_t keyvalue;
uint8_t decade_hour;
uint8_t unit_hour;
uint8_t decade_temp;
uint8_t decade_minute;
uint8_t unit_minute;
uint8_t decade_second;
uint8_t unit_second;
uint8_t unit_temp ;
uint8_t power_on_off_flag;
uint32_t wifi_key_counter;
uint8_t keyvalue;


void (*single_ai_fun)(uint8_t cmd);
void (*single_add_fun)(void);
void (*single_buzzer_fun)(void);
void (*sendAi_usart_fun)(uint8_t senddat);

static void Receive_Wifi_Cmd(uint8_t cmd);

static void RunKeyOrder_Handler(void);
static void Timing_Handler(void);
//static void Power_Off_Fun(void);
static void Power_On_Fun(void);


/*******************************************************************************
	 * 
	 * Function Name :void Single_RunMode(void)
	 * Function :main in single run process
	 * Input Reference:No
	 * Return Reference :NO
	 *
*******************************************************************************/
void Scan_KeyModel(void)
{
  static uint8_t model_temp ;

	
   if(run_t.wifi_special_key ==1 && POWER_KEY_VALUE() ==KEY_DOWN && run_t.wifi_led_fast_blink_flag==0){
      
		 while(POWER_KEY_VALUE()  ==KEY_DOWN && run_t.wifi_led_fast_blink_flag==0){
             wifi_key_counter++;
         
		

		 if(wifi_key_counter > 0x1a0bdf){ //1a6bdf//0x1e6bdf
             wifi_key_counter=0;
		     run_t.link_wifi_key_flag = 1;
		     run_t.wifi_led_fast_blink_flag=1;
			 break;
           }

		 }
	     if(wifi_key_counter  < 0x1a0bdf && run_t.wifi_led_fast_blink_flag==0){
	        wifi_key_counter=0;
		 	Power_Off_Fun();

		 }

	}
   
    if(run_t.link_wifi_key_flag==1){
		run_t.link_wifi_key_flag++;
        run_t.wifi_connect_flag =0;
	    run_t.gTimer_wifi_connect_counter=0;
	    SendData_Set_Wifi(0x01);
		HAL_Delay(100);
    }

    if(run_t.link_wifi_key_flag==3){
       if(POWER_KEY_VALUE() ==KEY_DOWN ){ //power on KEY
	         HAL_Delay(20);
	    while(POWER_KEY_VALUE()  ==KEY_DOWN);
		Power_Off_Fun();
        }
    }
	
    if(run_t.wifi_special_key ==0){
	      if(POWER_KEY_VALUE() ==KEY_DOWN ){ //power on KEY
	          HAL_Delay(20);
			 while(POWER_KEY_VALUE()  ==KEY_DOWN);
	             
				  if(run_t.gPower_On == 0 || run_t.gPower_On == 0xff){
				  	  
             
					  Power_On_Fun();
					  
					  SendData_PowerOff(1);
		           
				  }
				  else{

				    Power_Off_Fun();
					
			           
	              }
	           
      
	     }
     }
     else if(MODE_KEY_VALUE()==KEY_DOWN){ //Mode key 
	 	    
	     	 HAL_Delay(20);
	         while(MODE_KEY_VALUE()==KEY_DOWN);
			
	 	  
			if(run_t.gPower_On ==1){
				
		
			model_temp = model_temp ^ 0x01;

			if(model_temp == 1){ //timing of function.
                
				run_t.gModel =2;
				SendData_Set_Wifi(0x14);
				
				
		    }
            else{ //temperature of function adjust ref .
             
			   run_t.gModel =1;
			   SendData_Set_Wifi(0x04);
			  
            }
		
           
	     }
		
		  
     }
     else if(DEC_KEY_VALUE()==KEY_DOWN){ //"-" KEY
	 	
		  HAL_Delay(20);
        while(DEC_KEY_VALUE()==KEY_DOWN);
		
		if(run_t.gPower_On ==1){
			single_buzzer_fun();
	
		//setup temperature of value,minimum 20,maximum 40
		run_t.wifi_set_temperature--;
		if(run_t.wifi_set_temperature<20) run_t.wifi_set_temperature=40;
        if(run_t.wifi_set_temperature >40)run_t.wifi_set_temperature=40;

        decade_temp =  run_t.wifi_set_temperature / 10 %10;
		unit_temp =  run_t.wifi_set_temperature % 10; //

		lcd_t.number1_low=decade_temp;
		lcd_t.number1_high =decade_temp;

		lcd_t.number2_low = unit_temp;
		lcd_t.number2_high = unit_temp;
		
		run_t.panel_key_setup_timer_flag = 1;
		}
	}	
	else if(ADD_KEY_VALUE()==KEY_DOWN){ //"+" KEY
	 	 HAL_Delay(20);

		  while(ADD_KEY_VALUE()==KEY_DOWN);
		 
		  if(run_t.gPower_On ==1){
			single_buzzer_fun();

			run_t.wifi_set_temperature ++;
            if(run_t.wifi_set_temperature < 20){
			    run_t.wifi_set_temperature=20;
			}
			
			if(run_t.wifi_set_temperature > 40)run_t.wifi_set_temperature= 20;
			
		    decade_temp =  run_t.wifi_set_temperature / 10 %10;
			unit_temp =  run_t.wifi_set_temperature % 10; //

			lcd_t.number1_low=decade_temp;
			lcd_t.number1_high =decade_temp;

			lcd_t.number2_low = unit_temp;
			lcd_t.number2_high = unit_temp;

			run_t.panel_key_setup_timer_flag = 1;
					
					
			}
				
     }
}	

/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
 void Power_Off_Fun(void)
{
		run_t.gModel =0; //WT.EDIT 2022.09.01
		run_t.gPlasma=0;
		run_t.gDry =0;
		run_t.gBug =0;
		run_t.wifi_special_key = 0;
		run_t.wifi_led_fast_blink_flag=0;
      

		run_t.gTiming_label=0;
		run_t.wifi_connect_flag =0;
		run_t.power_key =2;
		run_t.gFan_RunContinue=1;
		run_t.disp_wind_speed_grade =1;	
		run_t.gPower_On=0;
		run_t.fan_off_60s =0;
		power_on_off_flag=1;
		run_t.temperature_set_flag = 0; //WT.EDIT 2023.01.31
		run_t.wifi_set_temp_flag=0;
        run_t.link_wifi_key_flag=0;
	    if(run_t.wifi_power_flag == WIFI_POWER_OFF_ITEM){
			run_t.wifi_power_flag = WIFI_POWER_NULL;
	    }	
		else
	   	   SendData_PowerOff(0);
  
} 

static void Power_On_Fun(void)
{
                
	run_t.gPower_On=1;

	run_t.power_key =1;
	run_t.gFan_RunContinue=0;

	run_t.gModel =1; //WT.EDIT 2022.09.01
	run_t.gPlasma=1;
	run_t.gDry =1;
	run_t.gBug =1;
	run_t.wifi_special_key = 1;
	run_t.temperature_set_flag = 0; //WT.EDIT 2023.01.31
    run_t.wifi_set_temp_flag=0; // //WT.EDIT 2023.01.31
    run_t.disp_wind_speed_grade =3;
	run_t.gTiming_label=  time_normal;
	run_t.gTimer_minute_Counter =0;
	if(power_on_off_flag==0){
	     run_t.dispTime_hours=12;
		 
	}

	 lcd_t.number5_low=(run_t.dispTime_hours ) /10;
     lcd_t.number5_high =(run_t.dispTime_hours) /10;

	 lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
	 lcd_t.number6_high = (run_t.dispTime_hours ) %10;
	
	 


}
  
/************************************************************************
	*
	*Function Name: void Wifi_Key_Fun(void)
	*
	*
	*
	*
************************************************************************/  
static void Timing_Handler(void)
{
  
		 
		 if(run_t.gTimer_minute_Counter >0){ //minute

			  run_t.gTimer_minute_Counter=0;

			  run_t.dispTime_minutes ++;//run_t.dispTime_minute++;
			  if(run_t.dispTime_minutes > 59){
				  run_t.dispTime_minutes=0;
                  run_t.dispTime_hours ++;
				  if(run_t.dispTime_hours >23){
					run_t.dispTime_hours=0;

				  }

			  }

			 lcd_t.number5_low=(run_t.dispTime_hours ) /10;
             lcd_t.number5_high =(run_t.dispTime_hours) /10;

			 lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
			 lcd_t.number6_high = (run_t.dispTime_hours ) %10;
	   


			lcd_t.number7_low = (run_t.dispTime_minutes )/10;
			lcd_t.number7_high = (run_t.dispTime_minutes )/10;

			lcd_t.number8_low = (run_t.dispTime_minutes )%10;
			lcd_t.number8_high = (run_t.dispTime_minutes )%10;
					 
          }
		
}





/******************************************************************************
*
*Function Name:void RunPocess_Command_Handler(void)
*Funcion: handle of tall process 
*
*
******************************************************************************/
void RunPocess_Command_Handler(void)
{
   //key input run function
   static uint8_t key_set_temp_flag,temp1,temp2;
   if(run_t.gPower_On ==1 && run_t.decodeFlag ==0){
       RunKeyOrder_Handler();
	   if(run_t.panel_key_setup_timer_flag==1){
           run_t.panel_key_setup_timer_flag=0;
		   key_set_temp_flag =1;
		   run_t.wifi_set_temp_flag=1;
		   run_t.gTimer_numbers_one_two_blink=0;
	      
		  
	   }
	   if(run_t.wifi_set_temp_flag ==0 && key_set_temp_flag ==1){
	   	    key_set_temp_flag = 0;

	        temp1 = run_t.gReal_humtemp[1]/10 %10;  // temperature
            temp2 = run_t.gReal_humtemp[1]%10;

		    lcd_t.number1_low=temp1;
			lcd_t.number1_high =temp1;

			lcd_t.number2_low = temp2;
			lcd_t.number2_high = temp2;
			run_t.gTimer_temp_delay =0;
			run_t.temperature_set_flag = 1;
            
		}
	   if(run_t.gModel == 1){ //as is "Ai mode"

          if(run_t.temperature_set_flag ==1 && run_t.gTimer_temp_delay >59){
               run_t.gTimer_temp_delay =0;
		 
		  
		  if(run_t.wifi_set_temperature <= run_t.gReal_humtemp[1] || run_t.gReal_humtemp[1] >39){//envirment temperature
	  
				run_t.gDry = 0;

		        SendData_Set_Command(0x02); //PTC turn off
			    //sendAi_usart_fun(0x91);//dry turn off;//turn off PTC "heat"
			    
                
		  }
		  else if((run_t.wifi_set_temperature -3) > run_t.gReal_humtemp[1] ||  run_t.gReal_humtemp[1] < 37){
	  
		     run_t.gDry = 1;
	         SendData_Set_Command(0x12); //PTC turn On
				 
		  }
	  
	     }

	  }
	   
   }
   //receive from mainboard data 
   if(run_t.decodeFlag ==1){
       run_t.decodeFlag =0;
       Decode_Function();
    }

   if(run_t.gPower_On ==0 || run_t.gPower_On == 0xff ){
	 	
	      Breath_Led();
	      run_t.gPower_On =0xff;
         if(run_t.gFan_RunContinue == 1){
           if(run_t.fan_off_60s < 61){
		      LED_MODEL_OFF();
			  POWER_ON_LED();
		      LCD_Display_Wind_Icon_Handler();
           	}
		   else{
               run_t.gFan_RunContinue =0;
			   Lcd_PowerOff_Fun();

		   }

         }
    }

    if(run_t.gTimer_set_temp_times > 0){ // 1 minute
	     run_t.gTimer_set_temp_times=0;
         SendData_Temp_Data(run_t.wifi_set_temperature);
    }
 
}
/******************************************************************************
*
*Function Name:void Single_RunCmd(void)
*Funcion: handle of receive by usart data
*
*
******************************************************************************/
static void RunKeyOrder_Handler(void)
{
	
    if(run_t.gPower_On ==1 ){


	 Lcd_PowerOn_Fun();
	 Timing_Handler();
	 DisplayPanel_Ref_Handler();
     
    
    }
	 
}

/**********************************************************************************************************
**
*Function Name:static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum,uint8_t unionNum)
*Function : 
*Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
*Return Ref:NO
*
*********************************************************************************************************/
void Decode_Function(void)
{
   
   Receive_MainBoard_Data_Handler(run_t.single_data);
    
}

/**********************************************************************************************************
	**
	*Function Name:void Receive_MainBoard_Data_Handler(uint8_t cmd)
	*Function : mainboard of command 
	*Input Ref:
	*Return Ref:NO
	*
*********************************************************************************************************/
void Receive_MainBoard_Data_Handler(uint8_t cmd)
{
    static uint8_t temperature_decade, temperature_unit;
	static uint8_t hum1,hum2,temp1,temp2; 
   


	 switch(cmd){


	 case WIFI_CMD:
	 	 
	 	 Receive_Wifi_Cmd(run_t.wifiCmd[0]);
	 
	     cmd = 0xff;
	     run_t.single_data =0xff;
	     
   
	 break;

	 case  WIFI_WIND_SPEED:
	 	if(run_t.gPower_On ==1){
		  

		   if(run_t.wifi_set_wind_speed <40){
		   	 
                run_t.disp_wind_speed_grade = 1;
		   	}
			else if(run_t.wifi_set_wind_speed >39 && run_t.wifi_set_wind_speed < 67){
			    run_t.disp_wind_speed_grade = 2;


			}
			else if(run_t.wifi_set_wind_speed >66){

				run_t.disp_wind_speed_grade = 3;

			}



		}
       cmd = 0xff;
	   run_t.single_data =0xff;
	 break;

	 case WIFI_TEMP: //set temperature value
	       if(run_t.gPower_On ==1){
		   	   

		      run_t.wifi_set_temp_flag = 1;

		      temperature_decade= run_t.wifi_set_temperature /10 ;
			  temperature_unit = run_t.wifi_set_temperature %10;
		   
	         lcd_t.number1_high = temperature_decade;
			 lcd_t.number1_low = temperature_decade;

			 
		    lcd_t.number2_high =  temperature_unit;
			lcd_t.number2_low = temperature_unit;

			
			run_t.gTimer_numbers_one_two_blink=0;
	      }
		cmd = 0xff;
	    run_t.single_data =0xff;
	 break;

	 case PANEL_DATA:
	   
        if(run_t.gPower_On ==1){
        hum1 =  run_t.gReal_humtemp[0]/10 %10;  //Humidity 
        hum2 =  run_t.gReal_humtemp[0]%10;
        
        temp1 = run_t.gReal_humtemp[1]/10 %10;  // temperature
        temp2 = run_t.gReal_humtemp[1]%10;

         //temperature 
		 lcd_t.number1_high = temp1;
		 lcd_t.number1_low = temp1;

		  lcd_t.number2_high = temp2;
		 lcd_t.number2_low = temp2;

		 //humidity
		 
		 lcd_t.number3_high = hum1;
		 lcd_t.number3_low = hum1;
		 
		 lcd_t.number4_high = hum2;
		 lcd_t.number4_low = hum2;

		 DisplayPanel_Ref_Handler();
        }
		cmd = 0xff;
	    run_t.single_data =0xff;
      break;

       case WIFI_BEIJING_TIME: 
         if(run_t.wifi_connect_flag ==1 && run_t.gPower_On==1){
			 lcd_t.number5_low=(run_t.dispTime_hours ) /10;
	         lcd_t.number5_high =(run_t.dispTime_hours) /10;

			 lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
			 lcd_t.number6_high = (run_t.dispTime_hours ) %10;
	   


			lcd_t.number7_low = (run_t.dispTime_minutes )/10;
			lcd_t.number7_high = (run_t.dispTime_minutes )/10;

			lcd_t.number8_low = (run_t.dispTime_minutes )%10;
			lcd_t.number8_high = (run_t.dispTime_minutes )%10;

	        DisplayPanel_Ref_Handler();
		    
        } 
        cmd = 0xff;
	    run_t.single_data =0xff;
      break;

	}


}
/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Receive_Wifi_Cmd(uint8_t cmd)
{
	switch(cmd){


		   case WIFI_POWER_ON: //turn on 
		 	
            //  single_buzzer_fun();
              run_t.wifi_power_flag = WIFI_POWER_ON_ITEM;
              Power_On_Fun();
			  cmd=0xff;

	         break;

			 case WIFI_POWER_OFF: //turn off 
                
			    //single_buzzer_fun();
			   run_t.wifi_power_flag = WIFI_POWER_OFF_ITEM;
				
			    Power_Off_Fun();
				
              cmd=0xff;

			 break;

			case WIFI_MODE_1: //AI turn on -> AI icon display 
                if(run_t.gPower_On==1){
				     run_t.gModel =1; //0-> has ,1->no gModel
                	} 
			break;

			 case WIFI_MODE_2: //icon don't display 
                 if(run_t.gPower_On==1){
				   run_t.gModel =2; //turon off AI mode
			 	   
                 }
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


	         default :
                  cmd =0;
			 break;

			 
        }
   
}

/****************************************************************
 * 
 * Function Name:
 * Function :function of pointer 
 * 
 * 
****************************************************************/
void Single_Add_RunCmd(void(*addHandler)(void))
{
    single_add_fun = addHandler;   

}

void Single_SendBuzzer_RunCmd(void(*buzzerHandler)(void))
{
	single_buzzer_fun = buzzerHandler;

}
void Single_SendAi_Usart_RunCmd(void(*sendaiHandler)(uint8_t seddat))
{
    sendAi_usart_fun = sendaiHandler;

}



