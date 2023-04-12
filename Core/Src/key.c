#include "key.h"
#include "gpio.h"
#include "run.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "cmd_link.h"
#include "single_mode.h"


key_types key_t;



uint8_t KEY_Scan(void)
{

   uint8_t  reval = 0;
 
  key_t.read = _KEY_ALL_OFF; //0xFF 
  
   if(POWER_KEY_VALUE() ==KEY_DOWN )// high level
	{
		key_t.read &= ~0x01; // 0xff & 0xfe =  0xFE
	}
    else if(MODE_KEY_VALUE() ==KEY_DOWN )
	{
		key_t.read &= ~0x02; // 0xFf & 0xfd =  0xFD
	}
    else if(DEC_KEY_VALUE()==KEY_DOWN )
	{
		  key_t.read &= ~0x04; // 0xFf & 0xfB =  0xFB
	}
    else if(ADD_KEY_VALUE() ==KEY_DOWN )
	{
		  key_t.read &= ~0x08; // 0x1f & 0xf7 =  0xF7
	 }

   

    switch(key_t.state )
	{
		case start:
		{
			if(key_t.read != _KEY_ALL_OFF)
			{
				key_t.buffer   = key_t.read; //??:key.buffer = 0xFE  POWER KEY 
				key_t.state    = first;
				key_t.on_time  = 0;
				key_t.off_time = 0;
                
			}
			break;
		}
		case first:
		{
			if(key_t.read == key_t.buffer) // adjust key be down ->continunce be pressed key
			{

				if(++key_t.on_time> 5000 &&  run_t.power_times==1) //1000  0.5us
				{
					
					run_t.power_times++ ;

					key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0xFE ^ 0xFF = 0x01
					key_t.on_time = 0;                        //key .value = 0xEF ^ 0XFF = 0X10
                   
					key_t.state   = second;
				
					
                   
                 }
				 else if(++key_t.on_time>200 && ++key_t.on_time <500){

					key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0xFE ^ 0xFF = 0x01
					key_t.on_time = 0;                        //key .value = 0xEF ^ 0XFF = 0X10
                   
					key_t.state   = second;


				 }
			}
			else
			{
				key_t.on_time = 0; 
				key_t.state   = start;
			}
			break;
		}
		case second:
		{
			if(key_t.read == key_t.buffer) //again adjust key if be pressed down 
			{
				if(++key_t.on_time> 600 && run_t.gPower_On==1)// 500 long key be down
				{
					
					key_t.value = key_t.value|0x80; //key.value = 0x01 | 0x80  =0x81  
					key_t.on_time = 0;
					key_t.state   = finish;
                   
				}
			}
			else if(key_t.read == _KEY_ALL_OFF)  // loose hand 
				{
					if(++key_t.off_time>4) //8 //30 don't holding key dithering
					{
						key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
						
						key_t.state   = finish; // loose hand
					}
					
				}
		   
			break;
		}
		case finish:
		{
			
			reval = key_t.value; // is short time  TIMER_KEY = 0x01  2. long times TIMER_KEY = 0X81
			key_t.state   = end;
         
			break;
		}
		case end:
		{
			if(key_t.read == _KEY_ALL_OFF)
			{
				if(++key_t.off_time>2)//50 //100
				{
					key_t.state   = start;
                  
				}
				
			}
			break;
		}
		default:
		{
			key_t.state   = start;
         
			break;
		}
	}
	return  reval;


}



void Normal_PressKey(void)
{
    static uint8_t power_on_flag_times,power_on_fisrt_flag;
    static uint8_t set_timer_flag,temp_bit_1_hours,temp_bit_2_hours,temp_bit_1_minute,temp_bit_2_minute;
    uint8_t decade_hour;
uint8_t unit_hour;
uint8_t decade_temp;
uint8_t decade_minute;
uint8_t unit_minute;
uint8_t decade_second;
uint8_t unit_second;
uint8_t unit_temp ;
    
    
    if(MODE_KEY_VALUE() ==KEY_DOWN ){
                  HAL_Delay(10);
				while(MODE_KEY_VALUE() ==KEY_DOWN);

			     if(run_t.gPower_On ==1){
					SendData_Buzzer();
				
					run_t.temp_set_timer_timing_flag=1;//run_t.gModel =2;
				
					run_t.gTimer_key_timing=0;
			
				
		        }

     }
     else if(DEC_KEY_VALUE()==KEY_DOWN )
	{
		HAL_Delay(10);
        while(DEC_KEY_VALUE()==KEY_DOWN);  
        if(run_t.gPower_On ==1){
	   	SendData_Buzzer();
	     switch(run_t.temp_set_timer_timing_flag){

		   case 0: 
	
	        run_t.wifi_set_temperature_value_flag =0;
			run_t.wifi_set_temperature--;
			if(run_t.wifi_set_temperature<20) run_t.wifi_set_temperature=40;
	        if(run_t.wifi_set_temperature >40)run_t.wifi_set_temperature=40;

			if(power_on_fisrt_flag ==0){
				power_on_fisrt_flag ++;
			  run_t.wifi_set_temperature =40;


			}

	        decade_temp =  run_t.wifi_set_temperature / 10;
			unit_temp =  run_t.wifi_set_temperature % 10; //
             HAL_Delay(5);
			lcd_t.number1_low=decade_temp;
			lcd_t.number1_high =decade_temp;

			lcd_t.number2_low = unit_temp;
			lcd_t.number2_high = unit_temp;
			
			run_t.panel_key_setup_timer_flag = 1;
	    	

		    break;

			case 1:
	    
			
				run_t.gTimer_key_timing =0;
                set_timer_flag=0;
				run_t.timer_time_hours -- ;//run_t.dispTime_minutes = run_t.dispTime_minutes - 1;
				if(run_t.timer_time_hours < 0){//if(run_t.dispTime_minutes < 0){

				    run_t.timer_time_hours =24;//run_t.dispTime_hours --;
					
					
				}
				    temp_bit_2_minute = run_t.timer_time_hours /10 ;
					temp_bit_1_minute = run_t.timer_time_hours %10;
               
					temp_bit_2_hours = run_t.timer_time_hours /10 ;
					temp_bit_1_hours = run_t.timer_time_hours  %10;
					run_t.timer_time_minutes  =0;

					temp_bit_2_minute=0;
					temp_bit_1_minute=0;
                 

					lcd_t.number5_low=temp_bit_2_hours;
					lcd_t.number5_high =temp_bit_2_hours;

					lcd_t.number6_low = temp_bit_1_hours;
					lcd_t.number6_high = temp_bit_1_hours;

					lcd_t.number7_low=temp_bit_2_minute;
					lcd_t.number7_high =temp_bit_2_minute;

					lcd_t.number8_low = temp_bit_1_minute;
					lcd_t.number8_high = temp_bit_1_minute;

             break;

	    	}
		}        
	}
    else if(ADD_KEY_VALUE() ==KEY_DOWN )
	{
		HAL_Delay(10);
        while(ADD_KEY_VALUE() ==KEY_DOWN);
        if(run_t.gPower_On ==1){
		
			SendData_Buzzer();
		

		    switch(run_t.temp_set_timer_timing_flag){

			case 0: //set temperature value add number
      
				run_t.wifi_set_temperature_value_flag =0;
				run_t.wifi_set_temperature ++;
	            if(run_t.wifi_set_temperature < 20){
				    run_t.wifi_set_temperature=20;
				}
				
				if(run_t.wifi_set_temperature > 40)run_t.wifi_set_temperature= 20;

				if(power_on_fisrt_flag ==0){
				     power_on_fisrt_flag ++;
			     	run_t.wifi_set_temperature =40;


			      }
            
			    decade_temp =  run_t.wifi_set_temperature / 10 ;
				unit_temp =  run_t.wifi_set_temperature % 10; //
                
				lcd_t.number1_low=decade_temp;
				lcd_t.number1_high =decade_temp;

				lcd_t.number2_low = unit_temp;
				lcd_t.number2_high = unit_temp;

				run_t.panel_key_setup_timer_flag = 1;
                
             
					
			
			   break;

			   case 1:
				 
					run_t.gTimer_key_timing =0;
                    set_timer_flag=0;
					run_t.timer_time_hours++ ;//run_t.dispTime_minutes = run_t.dispTime_minutes + 60;
				    if(run_t.timer_time_hours > 24){ //if(run_t.dispTime_minutes > 59){

		                 run_t.timer_time_hours=0;//run_t.dispTime_hours =0;
		                

					}
				
                  
					temp_bit_2_hours = run_t.timer_time_hours /10 ;
					temp_bit_1_hours = run_t.timer_time_hours %10;
					run_t.timer_time_minutes  =0;

					temp_bit_2_minute =0;
					temp_bit_1_minute =0;
                    
					lcd_t.number5_low=temp_bit_2_hours;
					lcd_t.number5_high =temp_bit_2_hours;

					lcd_t.number6_low = temp_bit_1_hours;
					lcd_t.number6_high = temp_bit_1_hours;

					lcd_t.number7_low=temp_bit_2_minute;
					lcd_t.number7_high =temp_bit_2_minute;

					lcd_t.number8_low = temp_bit_1_minute;
					lcd_t.number8_high = temp_bit_1_minute;

				
				break;
				}	
			
				
            }
        
	 }
    else if(run_t.gTimer_key_timing > 4 && run_t.temp_set_timer_timing_flag==1 && set_timer_flag ==0 && run_t.gPower_On==1){
				run_t.gTimer_digital5678_ms=0;
			   
				set_timer_flag++;
			   run_t.gTimer_key_timing =0;
			   if(run_t.timer_time_hours  ==0){
				   run_t.Timer_mode_flag = 0;
				   run_t.temp_set_timer_timing_flag=0;
			       run_t.timer_timing_define_flag = timing_not_definition;
	
			   }
			   else{
				   run_t.Timer_mode_flag = 1;
					SendData_Time_Data(run_t.dispTime_hours);
                     HAL_Delay(300);
			   }
	
		}


}






