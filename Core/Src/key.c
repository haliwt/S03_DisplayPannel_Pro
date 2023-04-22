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

			 if(++key_t.on_time>40 ){ //120 

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
				if(++key_t.on_time> 800 && run_t.gPower_On==1)// 500 long key be down
				{
					
					key_t.value = key_t.value|0x80; //key.value(power_on) = 0x01 | 0x80  =0x81  
					key_t.on_time = 0;               //key.vaule(model_key) = 0x01 | 0x80 = 0x82
					key_t.state   = finish; 
                   
				}
			}
			else if(key_t.read == _KEY_ALL_OFF)  // loose hand 
				{
					if(++key_t.off_time>0) //8 //30 don't holding key dithering
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
            if(INTERRUPT_KEY()==0){
            
                   run_t.touch_key_enable=0;
            }
			break;
		}
		case end:
		{
			if(key_t.read == _KEY_ALL_OFF)
			{
				if(++key_t.off_time>0)//50 //100
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

//°´¼ü´¦Àíº¯Êý
//·µ»Ø°´¼üÖµ
//mode:0,don't has continuce key ;1-> continuce be pressed 
//0 -> none key be pressed 
//1 
//×¢Òâ´Ëº¯ÊýÓÐÏìÓ¦ÓÅÏÈ¼¶,KEY0>KEY1>KEY2>WK_UP!!

uint8_t  Key_BePressed_Fun(uint8_t mode)
{

    static uint8_t key_up=1;     // key release the button 
   
    if(mode==1)key_up=1;    //continue be pressed button
    
    if(key_up&&(POWER_KEY() ==1||MODE_KEY()==1||ADD_KEY()==1||DEC_KEY()==1))
    {
       delay_tick_us(1000); //delay_ms(5);
        key_up=0;
        if(POWER_KEY() ==1){
			run_t.touch_key_enable=0;
			return power_key;
        }
        else if(MODE_KEY()==1){
			run_t.touch_key_enable=0;
			return model_key;
        }
        else if(ADD_KEY()==1){
			run_t.touch_key_enable=0;
			return add_key;
        }
        else if(DEC_KEY()==1){
			run_t.touch_key_enable=0;
			return dec_key;  
        }
    }
//	else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0){
//		key_up=1;
//	}
    run_t.touch_key_enable=0;
    return 0;   //ÎÞ°´¼ü°´ÏÂ

}








