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
	u8 fudu2 =0;
	u16 you;
	u16 zuo;
	u8 state=0;	
	
	

	void xunguang()
	{
		adc4=Get_Adc_Average(4,20);//adc4是右边的传感器
		adc5=Get_Adc_Average(5,20);//adc5是左边的传感器
		
		
		if(adc4<=0x400||adc5<=0x400)
		{
      car_stop();
      continue;
		}
		else if(adc4>=0xdd0 && adc5>=0xdd0)
		{
			car_right();
		}
		else if(adc4>adc5&&adc4-adc5<0x08f||adc5>adc4&&adc5-adc4<0x8f)
		{
			 car_forward();
			delay_ms(100);
		}
		
      else if(adc4<adc5)//右边亮,左边暗
		{
        car_right();
		}
      else     //左边亮，右边暗
		{ 
        car_left();
			
			
		}
	}

	
	
 void xunji()
 {
		switch(KEY1<<2 | KEY0<<1 | KEY2)
		{
			case 5: car_forward(); last_state = 5; break;//101
			case 4: car_right_s(200); last_state = 4; break;//100
			case 6: car_right_s(150); last_state = 6; break;//110
			case 1: car_left_s(200); last_state = 1; break;//001
			case 3: car_left_s(150); last_state = 3; break;//011
			case 7: {
				out++;
				switch(last_state){
					case 4: 
					case 6: if(out>outfudu)
									{
										car_right_back();
										chixu++;
										if(chixu>chixufudu)
										{
											out=0;
											chixu=0;
										}
									}
									else car_right_s(250);break; 
					case 1:
					case 3: if(out>outfudu)
									{
										car_left_back();chixu++;
										if(chixu>chixufudu)
										{
											out=0;
											chixu=0;
										}
									}
									else car_left_s(250);break;
					default: break;
						
				}
			}
//			case 0: car_stop(); break;
			default: break;
		}
	}

u8 KEY_State(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up)//&&(WK_UP==1)
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(WK_UP==1)return 1;
	}else if(WK_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
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
/*		fudu2++;
			if(fudu2>1000)
			{
				if(KEY_State(1)==0)state=1-state;
				fudu2=0;
			}
*/
			if(WK_UP==0)state=1-state;
			while(WK_UP==0);
			if(state==0)xunji();
			else xunguang();
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
















