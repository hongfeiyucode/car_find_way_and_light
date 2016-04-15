#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"
#include "lcd.h"
#include "usmart.h"		
#include "hc05.h" 	 
#include "usart3.h" 	
#include "key.h" 	 
#include "string.h"	 
#include "motor.h"
#include "adc.h"
//ALIENTEK探索者STM32F407开发板扩展实验1
//ATK-HC05蓝牙串口模块实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 
extern u32 global_time_cnt;
extern u8 USART3_RX_BYTE;

	int ii=0;
	u8 last_state = 0xff;
	u8 la2_state = 0xff;
	u8 out = 0;
	u8 fudu =30;
	u16 you;
	u16 zuo;
	u8 state=0;	
 void xunji()
 {
	 		switch(KEY1<<2 | KEY0<<1 | KEY2)
		{
			case 5: car_forward();if(out<=fudu)for(ii=0;ii<10;ii++)car_forward(); la2_state=last_state;last_state = 5; break;//101
			case 4: car_right_slow();if(out<=fudu)car_forward(); la2_state=last_state;last_state = 4; break;//100
			case 6: car_right();if(out<=fudu)car_forward(); la2_state=last_state;last_state = 6; break;//110
			case 1: car_left_slow();if(out<=fudu)car_forward(); la2_state=last_state;last_state = 1; break;//001
			case 3: car_left(); if(out<=fudu)car_forward();la2_state=last_state;last_state = 3; break;//011
			case 7: {
				//la2_state=last_state;last_state = 7;
				out=out+1;
				switch(last_state){
					case 4: 
					case 6: if(out>fudu){car_right_back();out=0;}
									else car_right_slow();
									//if(la2_state==7&&last_state==7)car_right();
									//else car_right_slow();
									break; 
					case 1:
					case 3: if(out>fudu){car_left_back();out=0;}
									else car_left_slow();
									//if(la2_state==7&&last_state==7)car_left();
									//else car_left_slow();
									break; 
					default: break;
				}
			}
//			case 0: car_stop(); break;
			default: break;
		}
		delay_ms(1);
	}

	void xunguang()
	{
			you=Get_Adc_Average(ADC_CH5,20);
			zuo=Get_Adc_Average(ADC_CH4,20);
	}

	
int main(void)
{	 


	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz 
	delay_init(168);			//延时初始化  
	uart_init(84,115200);		//初始化串口波特率为115200 
	usmart_dev.init(84); 		//初始化USMART		
	KEY_Init();					//初始化按键
	LCD_Init();		 			//初始化LCD
	usmart_dev.init(72); 		//初始化USMART 	  
	POINT_COLOR=RED;
	delay_ms(1000);			//等待蓝牙模块上电稳定
	
	
	
	if(HC05_Init()) 		//初始化ATK-HC05模块  
	{
		LCD_ShowString(30,90,200,16,16,"ATK-HC05 Error!"); 
		delay_ms(500);
		LCD_ShowString(30,90,200,16,16,"Please Check!!!"); 
		delay_ms(100);
	}
	
	POINT_COLOR=BLUE;
	delay_ms(100);
	pwm_init();
	delay_ms(100);
	
	//TODO弹射
	//WK_UP==1;
//	while(1)
//	{
//		if(WK_UP==0)car_right();按下
//	}
	


	
	while(1)
	{
			if(WK_UP==0)state=1-state;
			if(state)car_right();//xunji();
			else car_left();//xunguang();
	}

	while(1) 
	{				
		switch(USART3_RX_BYTE)
		{
			//up, forward
			case 'w': car_forward(); break;
			//down, backward
			case 's': car_backward(); break;
			//left, turn left
			case 'a': car_left(); break;
			//right, turn right
			case 'd': car_right(); break;
			//default mode
			case 't': car_stop(); break;
			default: break;
		}
	}											    
}
















