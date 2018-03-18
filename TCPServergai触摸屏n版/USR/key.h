#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP
//#define KEY4 PEin(6)	//PE6
//#define KEY5 PAin(3)	//PA3

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����3(WK_UP) 
#define KEY4  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)//��ȡ����4
//#define KEY5  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ����5
#define KEY6  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//��ȡ����6
#define KEY7  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//��ȡ����7
#define KEY8  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����8
#define KEY9  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����9

#define KEY_LED3  10
#define KEY_LED2  9
#define KEY_CO    8
#define KEY_RE    7
//#define KEY_CLOSE 6
#define KEY_MC	  5
#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1

#define Line0_1  GPIO_SetBits  (GPIOA, GPIO_Pin_12 ) 		//enable port
#define Line0_0 GPIO_ResetBits  (GPIOA, GPIO_Pin_12 ) 
#define Line1_1  GPIO_SetBits  (GPIOA, GPIO_Pin_11) 		//enable port
#define Line1_0 GPIO_ResetBits  (GPIOA, GPIO_Pin_11 ) 
#define Line2_1  GPIO_SetBits  (GPIOA, GPIO_Pin_8) 		//enable port
#define Line2_0 GPIO_ResetBits  (GPIOA, GPIO_Pin_8 )
#define Line3_1  GPIO_SetBits  (GPIOA, GPIO_Pin_7) 		//enable port
#define Line3_0 GPIO_ResetBits  (GPIOA, GPIO_Pin_7 )


#define Column0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)//��ȡ����2
#define Column1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//��ȡ����2
#define Column2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//��ȡ����2 
#define Column3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//��ȡ����3 

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��	
u8 Keyboard_Scan(void);  	//����ɨ�躯��
#endif

