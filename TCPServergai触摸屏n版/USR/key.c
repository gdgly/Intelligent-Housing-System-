#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "pbdata.h"
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
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	//��ʼ��KEY0-->GPIOA.13,KEY1-->GPIOA.15  ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6;//PE2~4.6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
 //�������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//PE0~3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;//PE0~3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
}

u8 Keyboard_Scan()
{	 	
	 Line0_0;
	 Line1_1;
	 Line2_1;
	 Line3_1;
	 if(Column0==1||Column1==1||Column2==1||Column3==1){
    delay_ms(10);//ȥ���� 
		if(Column0==0)return 1;
		else if(Column1==0)return 2;
		else if(Column2==0)return 3;
		else if(Column3==0)return 10;
	 }
 
		Line0_1;
	  Line1_0;
	  Line2_1;
	  Line3_1;
	  if(Column0==1||Column1==1||Column2==1||Column3==1){
    delay_ms(10);//ȥ���� 
		if(Column0==0)return 4;
		else if(Column1==0)return 5;
		else if(Column2==0)return 6;
		else if(Column3==0)return 11;
	 }
		Line0_1;
	  Line1_1;
	  Line2_0;
	  Line3_1;
	 if(Column0==1||Column1==1||Column2==1||Column3==1){
    delay_ms(10);//ȥ���� 
		if(Column0==0)return 7;
		else if(Column1==0)return 8;
		else if(Column2==0)return 9;
		else if(Column3==0)return 12;
	 }
	  Line0_1;
	  Line1_1;
	  Line2_1;
	  Line3_0;
	 if(Column0==1||Column1==1||Column2==1||Column3==1){
    delay_ms(10);//ȥ���� 
		if(Column0==0)return 14;
		else if(Column1==0)return 16;
		else if(Column2==0)return 15;
		else if(Column3==0)return 13;
	 }
 	return 0;// �ް�������
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!

