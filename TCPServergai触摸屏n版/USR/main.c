#include "pbdata.h"
#include "key.h"
#include "lcd.h"
#include "sys.h"
#include "led.h"
#include "beep.h"
#include "dht11.h" 	
#include "adc.h"
#include "timer.h"
#include "stmflash.h"
#include "oled.h"
#include "touch.h"
#include "24cxx.h" 
extern u8 CNT,CNT2,flag;                //TIM3��TIM2��ʱ  TIM2��־λ
void Temp_Humi_Coal(void);              //��ʪ��ú�����
void Input_password(void);              //��������
void Modify_password(void);             //�޸�����
void Out_door(void);                    //�����������
extern  u16 adcx;                       //    
extern  float temp;	 
extern  u8 t,a,b; 
extern	u8 temperature;  	              //�¶�
extern  u8 humidity;                    //ʪ��
extern  u8 coal;                        //ú��Ũ����ֵ
    char line,line2,code2,first_code,second_code,third_code,fourth_code=10,
			   first_code2,second_code2,third_code2,fourth_code2=10;          //���������������
		int code;
	  char  buf_code[4]={0};               //�趨����
		u16   total=0,total2=0,all=0;        //�����������
		#define SIZE sizeof(buf_code)	 			  	//���鳤��
    #define FLASH_SAVE_ADDR_START  0X08070000//0X08008000 FLASH�洢��ʼ��ַ
		#define FLASH_SAVE_ADDR_END    0X080703F0//0X0800C000
		u8 datatemp[SIZE];
//		u8 quit_flag;
int main(void)
{
	system_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD  
	OLED_Init();			  //��ʼ��OLED 
	Adc_Init();		  		//ADC��ʼ��
	BEEP_Init();        //��������ʼ��
  TIM3_Int_Init(9999,7199);
	TIM2_Int_Init(9999,7199);
	TIM4_Int_Init(9999,7199);
	KEY_Init();         //������ʼ��
	tp_dev.init();
 	POINT_COLOR=RED;		//��������Ϊ��ɫ
  LED1=LED2=0;
	    	
 	while(DHT11_Init())	//DHT11��ʼ��	
	{
		LCD_ShowString(30,130,200,16,16,"DHT11 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}	
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
 	LCD_ShowString(30,150,200,16,16,"Temp:  C");	 
 	LCD_ShowString(30,170,200,16,16,"Humi:  %");
  LCD_ShowString(30,190,200,16,16,"Coal:  %");
	OLED_ShowString(0,20,"Input password:"); 
	OLED_Refresh_Gram();
	BEEP=0;
		
	while(1)
	{	 
		a=KEY_Scan(0);
		b=Keyboard_Scan();
			if(a==KEY_RIGHT||b==14)          //�����Ž�
		{
		LED0=0;
		LED1=0;
		LCD_ShowString(20,80,220,16,16,"Access control has started");
		OLED_ShowString(10,5,"Code started"); 
		OLED_Refresh_Gram();	
		while(1)
		{
		a=KEY_Scan(0);                        //�Ŵ�
		if(a==KEY_MC)
		{	
			TIM_Cmd(TIM3, ENABLE);
			Input_password();                  //�������
      break;			
			}
		Temp_Humi_Coal();
	  }
  	}
		if(a==KEY_DOWN)                       //����wifi
		{   
			  LED2=0;
		  	LCD_ShowString(20,60,200,16,16,"wifi mode started ");
				printf("ϵͳ��ʼ�����\r\n");
	      printf("�Ƿ����WIFI�������ó���\r\n");
	      printf("����Y+�ո񣬵�����ͽ���������á�����N+�ո񣬵�����ͣ�����\r\n");
			
			while(1)
			{ 
				LED2=0;
				ESP8266_test();                      //wifi����
				LCD_ShowString(20,60,200,16,16,"wifi mode closed ");
				LCD_ShowString(30,210,200,16,16,"Connection disconnect!");
				LED1=0;
				LED2=0;
				break;
			   }
		  }
		  CNT=20;
			LCD_ShowNum(200,120,CNT,2,16);
		  TIM_Cmd(TIM3, DISABLE);
			BEEP=0;
		  BEEP2=0;
	    LED0=0;
		  LCD_ShowString(20,80,200,16,16,"Access control is closed");
			OLED_ShowString(10,5,"Code  closed");
			OLED_Refresh_Gram();	
		 Temp_Humi_Coal();
	 	 delay_ms(10);
	}

}

u8 KEY_Scan(u8 mode)                   //���ڰ�������
{	  	  
	if((KEY0==0||KEY1==0||KEY2==0||KEY4==1||KEY3==1||KEY6==0||KEY7==0||KEY8==0||KEY9==0))
{
		delay_ms(10);//ȥ���� 
		if(KEY0==0)  {while(KEY0==0);                  //E4
		              return KEY_RIGHT;}               
		else if(KEY1==0){while(KEY1==0);               //E3    
			               return KEY_DOWN;}
		else if(KEY2==0){while(KEY2==0);               //E2
			               return KEY_LEFT;}
		else if(KEY6==0){while(KEY6==0)                //E1
                      {TIM_Cmd(TIM2, ENABLE);
											 }		
                     TIM_Cmd(TIM2, DISABLE);											
			               return KEY_RE;}
//		else if(KEY5==1)return KEY_CLOSE;  
		else if(KEY7==0){while(KEY7==0)                 //E0  ȷ�ϰ���
			               {TIM_Cmd(TIM2, ENABLE);
											 }
										  TIM_Cmd(TIM2, DISABLE);
											 CNT2=0;
			                return KEY_CO;}
		else if(KEY8==0){while(KEY8==0);               //F0
			               return KEY_LED2;}
		else if(KEY9==0){while(KEY9==0);               //F0
			               return KEY_LED3;}
		else if(KEY4==1)  return KEY_MC;                //E6   �Ŵ�
	  else if(KEY3==1)  return KEY_UP;                //A0   
}
 	return 0;// �ް�������

	}
void Temp_Humi_Coal(void)              //��ʪ��ú��������
{   
	  coal=17;
	  adcx=Get_Adc_Average(ADC_Channel_1,10);
		temp=(float)adcx*(3.3/4096);
		adcx=temp;
		temp-=adcx;
		temp*=20;
    if(temp>coal) 
		{ BEEP2=1;
		}
    if(temp<=coal) 
		{ BEEP2=0;
		}	
 		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ					    
			LCD_ShowNum(30+40,150,temperature,2,16);	//��ʾ�¶�	   		   
			LCD_ShowNum(30+40,170,humidity,2,16);		  //��ʾʪ��	
      LCD_ShowNum(30+40,190,temp,2,16);	 			  //��ʾȼ����
		}
}
void Input_password(void)               //�������
{ 
	    line=140;
	    line2=40;
       if(line==148)
			LCD_ShowString(20,120,200,16,16,"Input password:");
			LCD_ShowString(line,105,200,16,16,"_");
	    STMFLASH_Read(FLASH_SAVE_ADDR_START,(u16*)buf_code,SIZE);      //��ȡ����
			while(1)
			{
			 
			 Modify_password();                    //�޸�����
       if(line==140)
			 {
				 first_code=code;
				 LCD_ShowNum(line,120,first_code,1,16);
				 LCD_ShowString(140,105,200,16,16,"_   ");
			  }
			 {
				 second_code=code;
				 LCD_ShowNum(line,120,second_code,1,16);
				  LCD_ShowString(140,105,200,16,16," _  ");
			  }	
       if(line==156)
			 {
				 third_code=code;
				 LCD_ShowNum(line,120,third_code,1,16);
				 LCD_ShowString(140,105,200,16,16,"  _ ");
			  }
       if(line==164)
			 {
				 fourth_code=code;
				 LCD_ShowNum(line,120,fourth_code,1,16);
				 LCD_ShowString(140,105,200,16,16,"   _");
			  }
			 if(a==KEY_CO||b==15)
			{
				
			total=first_code*1000+second_code*100+third_code*10+fourth_code;
			total2=first_code2*1000+second_code2*100+third_code2*10+fourth_code2;
			all=buf_code[0]*1000+buf_code[1]*100+buf_code[2]*10+buf_code[3];
//				printf("%d\r\n",all);
//				printf("%d\r\n",total);
			   if(flag&&(total==all))
				{ 
					while(1)
					{
					 Modify_password();
					 LCD_ShowString(20,120,200,16,16,"Modif password:");
					 if(line==140)
			    {
				   buf_code[0]=code;
				   LCD_ShowNum(line,120,buf_code[0],1,16);
					 LCD_ShowString(140,105,200,16,16,"_   ");
			    }
           if(line==148)
			   {
				   buf_code[1]=code;
				   LCD_ShowNum(line,120,buf_code[1],1,16);
           LCD_ShowString(140,105,200,16,16," _  ");
			    }	
           if(line==156)
			   {
				   buf_code[2]=code;
				   LCD_ShowNum(line,120,buf_code[2],1,16);
					 LCD_ShowString(140,105,200,16,16,"  _ ");
   			    }
           if(line==164)
			   {
				   buf_code[3]=code;
				   LCD_ShowNum(line,120,buf_code[3],1,16);
					 LCD_ShowString(140,105,200,16,16,"   _");
			    }
				   flag=0;
				   if(a==KEY_CO)  
					 {
						 code=0;
						 all=buf_code[0]*1000+buf_code[1]*100+buf_code[2]*10+buf_code[3];
						 STMFLASH_Write(FLASH_SAVE_ADDR_START,(u16*)buf_code,SIZE);         //��ȡ����
						 LED1=0;
						 break;
					 }
					 }
					LCD_ShowString(20,120,200,16,16,"Input password:");
					LCD_ShowString(140,120,200,16,16,"0000");
				 }
			if((total==all)||(total2==all))
			{
		 	LCD_ShowString(140,120,200,16,16,"0000");
			OLED_ShowString(40,49,"****");
			OLED_Refresh_Gram();	
				code=0;
				fourth_code=fourth_code2=10;
			 break;
			}
	   	}	
OLED_Refresh_Gram();			
			}	
}
void Modify_password(void)
{
			Temp_Humi_Coal();
      Out_door();	
			a=KEY_Scan(0);
			if(a==KEY_RIGHT) 
			{ 
				LCD_Fill(line,105,line+50,105+15,WHITE);
				line+=8;
				code=0;
				if(line>164)
					line=140;
			 }
			if(a==KEY_DOWN) 
			{
				LCD_Fill(line,105,line+50,105+15,WHITE);
				line-=8;
				if(line<140)
					line=164;
			 }
			if(a==KEY_LEFT)
			{
				code++;
				if(code>9)
					code=0;
			 }
			if(a==KEY_RE)
			{
				code--;
				if(code<0)
					code=9;
			 } 
}
void Out_door(void)          //���ⰴ��
{   
    b=Keyboard_Scan();
	 if(b==10)
		{
			line2-=7;
			if(line2<40)
			line2=40;
		  }
		if(b>0&&b<10)
		{ 
			code2=b;
			switch(line2)
			{
				case 40:
				first_code2=code2;  OLED_ShowNum(40,49,first_code2,1,16);  line2+=7; code2=0; break;
				case 47:
				second_code2=code2;  OLED_ShowNum(47,49,second_code2,1,16);  line2+=7; code2=0; break;
				case 54:
				third_code2=code2;  OLED_ShowNum(54,49,third_code2,1,16);  line2+=7; code2=0; break;
				case 61:
				fourth_code2=code2;  OLED_ShowNum(61,49,fourth_code2,1,16);  code2=0; break;
		}
	}
		if(b==16)
		{
		 code2=0;
			switch(line2)
			{
				case 40:
				first_code2=code2;  OLED_ShowNum(40,49,first_code2,1,16);  line2+=7; code2=0; break;
				case 47:
				second_code2=code2;  OLED_ShowNum(47,49,second_code2,1,16);  line2+=7; code2=0; break;
				case 54:
				third_code2=code2;  OLED_ShowNum(54,49,third_code2,1,16);  line2+=7; code2=0; break;
				case 61:
				fourth_code2=code2;  OLED_ShowNum(61 ,49,fourth_code2,1,16);  code2=0; break;
		}
		}
			 OLED_ShowString(line2,35,"_");
		   OLED_ShowString(line2-7,35," ");
		   OLED_ShowString(line2-14,35," ");
	     OLED_ShowString(line2-21,35," ");
		   OLED_ShowString(line2+7,35," ");
		   OLED_ShowString(line2+14,35," ");
	     OLED_ShowString(line2+21,35," ");
			 OLED_Refresh_Gram();
 }



