#include "ESP8266.h"
#include "led.h"
//#include "delay.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "beep.h"
#include "dht11.h" 	
#include "adc.h"
#include "stmflash.h"
#include "touch.h"
#include "usart_config.h"
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	 //��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);                    //ָ��λ����ʾ�ַ���
void atk_8266_load_keyboard(u16 x,u16 y);                                //������
void atk_8266_key_staset(u16 x,u16 y,u8 keyx,u8 sta);                    //����״̬����
void rtp_test(void);                                                     //������
void ctp_test(void);                                                     //������
u8 atk_8266_get_keynum(u16 x,u16 y);                                     //�õ���������ֵ
u8* kbd_tbl[15]={"KEY","LED","DEL","1","2","3","4","5","6","7","8","9","SURE","0","QUIT"};  //�����ַ���
u16 key_code[30]={0};
//u8* key_led[4]={"LED1","LED2","LED3","ALL"}; 
//char* wifi_send[10]={"7","8","9","a","b","c","d","e","f","g"};           //������������
void ( * pNet_Test ) ( void );
void Temp_Humi_Coal(void);
struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
    extern u8 flag,sleep_flag;    //TIM2��־λ  �����߱�־λ
	  u16 adcx;                     //��ʪ��ú������
	  float temp;	 
	  u8 t=0,a,b,i,client_ID,client_IPD[6]; 
  	u8 temperature,humidity,coal=17;  
    u8 key_place=7,send_flag;               //��Ҫ���õİ���  
    int address,address_data;
extern u8 CNT3;
#define SIZE2 sizeof(key_code)	 			  	//���鳤��
#define FLASH_SAVE_ADDR_START2  0X08071000//0X08008000
   //ú��Ũ����ֵ
u8 KEY_Scan(u8); 
/*
 * ��������ESP8266_test
 * ����  ��ESP8266���Գ���
 * ����  ��
 *         
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_test(void)
{
	
	char cCh;
//	LED_Init();
//	LED0=LED1=LED2=0;
//	delay_init();	    	    //��ʱ������ʼ��	
	PC_Usart("\r\n WF-ESP8266 WiFiģ�����\r\n");                            //��ӡ����������ʾ��Ϣ
		
  PC_Usart ( "\r\n������ģ��Ĳ���ģʽ�ı�ţ�������£�\
              \r\n1.STAģʽ�� TCP Client\
              \r\n2.APģʽ�� AP Server\
              \r\n3.STA+APģʽ�� (STA)TCP Client + (AP)TCP Server\
              \r\n�����ʽΪ�����+�ո񣬵������\r\n" );
	STMFLASH_Read(FLASH_SAVE_ADDR_START2,(u16*)key_code,SIZE2);               //��ȡ��ƥ�䰴��
//  scanf ( "%c",  & cCh );
  cCh='2';
  switch ( cCh )
  {
		case '1':
			pNet_Test = ESP8266_STA_TCP_Client;
		  PC_Usart("\r\n�������еĲ����ǣ�1.STAģʽ�� TCP Client\r\n");
		  break;
		
		case '2':
			pNet_Test = ESP8266_AP_TCP_Server;
		  PC_Usart("\r\n�������еĲ����ǣ�2.APģʽ�� TCP Server\r\n");
		  break;

		case '3':
			pNet_Test = ESP8266_StaTcpClient_ApTcpServer;
		  PC_Usart("\r\n�������еĲ����ǣ�3.STA+APģʽ�� (STA)TCP Client + (AP)TCP Server\r\n");
		  break;

		default:
			pNet_Test = NULL;
		  PC_Usart("\r\n������󣬲����в��ԣ�\r\n");
		  break;		
			
	}
//PC_Usart ( "%d",pNet_Test);
	
	if ( pNet_Test )
		  pNet_Test();

}
/*
 * ��������ESP8266_Choose
 * ����  ��ʹ��/����WF-ESP8266ģ��
 * ����  ��enumChoose = ENABLE��ʹ��ģ��
 *         enumChoose = DISABLE������ģ��
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_Choose ( FunctionalState enumChoose )
{
	if ( enumChoose == ENABLE )
		ESP8266_CH_HIGH_LEVEL();
	
	else
		ESP8266_CH_LOW_LEVEL();
	
}


/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ����ESP8266_AT_Test����
 */
void ESP8266_Rst ( void )
{
	#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	
	#else
	 ESP8266_RST_LOW_LEVEL();
	 delay_ms ( 500 ); 
	 ESP8266_RST_HIGH_LEVEL();
	 
	#endif

}


/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����	
 */
void ESP8266_AT_Test ( void )
{
	ESP8266_RST_HIGH_LEVEL();
	
	delay_ms ( 1000 ); 
	
	while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 200 ) )
		
		ESP8266_Rst (); 
	
  
}


/*
 * ��������ESP8266_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	delay_ms ( waittime );                 //��ʱ
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}


/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		  return ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  return ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return false;
  }
	
}


/*
 * ��������ESP8266_JoinAP
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 7000 );
	
}


/*
 * ��������ESP8266_BuildAP
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, char * enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
}


/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d",( enumEnUnvarnishTx ? 1 : 0 ));
//	( enumEnUnvarnishTx ? 1 : 0 ) 
//	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	return ESP8266_Cmd ( "AT+CIPMUX=1", "OK", 0, 500 );
	
}


/*
 * ��������ESP8266_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 500 );
	
}


/*
 * ��������ESP8266_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}


/*
 * ��������ESP8266_UnvarnishSend
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_UnvarnishSend ( void )
{
	return (
	  ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) &&
	  ESP8266_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 ) );
	
}


/*
 * ��������ESP8266_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
		
	if ( enumEnUnvarnishTx )
		ESP8266_Usart ( "%s", pStr );

	
	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength  );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength  );
		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}


/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	delay_ms ( 300 ) ;
	if(strEsp8266_Fram_Record .InfBit .FramFinishFlag)
	{
//	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	if ( enumEnUnvarnishTx )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, ">" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
	
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
}
	return pRecStr;

}


/*
 * ��������ESP8266_STA_TCP_Client
 * ����  ��WF-ESP8266ģ�����STA TCP Clien����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_STA_TCP_Client ( void )
{
	char cStrInput [100] = { 0 };
//	char * cStrInput="ATK-ESP8266,12345678 ";
	char * pStrDelimiter [2], * pBuf, * pStr;
	u8 uc = 0;
  u32 ul = 0;

	ESP8266_Choose ( ENABLE );	
  
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );
  
	ESP8266_Cmd ( "AT+CWLAP", "OK", 0, 5000 );
		
  do
	{
		PC_Usart ( "\r\n������Ҫ���ӵ�WiFi���ƺ���Կ�������ʽΪ�������ַ�+Ӣ�Ķ���+��Կ�ַ�+�ո񣬵������\r\n" );

		scanf ( "%s", cStrInput );
//    cStrInput[]="ATK-ESP8266,12345678 ";
		PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

		pBuf = cStrInput;
		uc = 0;
		while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
		{
			pStrDelimiter [ uc ++ ] = pStr;
			pBuf = NULL;
		} 
		
  } while ( ! ESP8266_JoinAP ( pStrDelimiter [0], pStrDelimiter [1] ) );
	
	ESP8266_Enable_MultipleId ( ENABLE );
	
	do 
	{ 
		PC_Usart ( "\r\n���ڵ����Ͻ��������������TCP Server�������磬��������Ե�IP�Ͷ˿ںţ������ʽΪ������IP+Ӣ�Ķ���+�˿ں�+�ո񣬵������\r\n" );

		scanf ( "%s", cStrInput );

		PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

		pBuf = cStrInput;
		uc = 0;
		while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
		{
			pStrDelimiter [ uc ++ ] = pStr;
			pBuf = NULL;
		} 
		
  } while ( ! ( ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_0 ) &&
	              ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_1 ) &&
	              ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_2 ) &&
	              ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_3 ) &&
	              ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_4 ) ) );

  for ( uc = 0; uc < 5; uc ++ )
	{
		PC_Usart ( "\r\n������˿�ID%dҪ���͵��ַ����������ʽΪ���ַ����������ո�+�ո񣬵������\r\n", uc );

		scanf ( "%s", cStrInput );

		ul = strlen ( cStrInput );
		
		ESP8266_SendString ( DISABLE, cStrInput, ul, ( ENUM_ID_NO_TypeDef ) uc );
		
	}
	
	
	PC_Usart ( "\r\n��������������ַ����ַ���\r\n" );
	while (1)
	{
	  pStr = ESP8266_ReceiveString ( DISABLE );
		PC_Usart ( "%s", pStr );
	}

}


/*
 * ��������ESP8266_AP_TCP_Server
 * ����  ��WF-ESP8266ģ�����AP TCP Server����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_AP_TCP_Server ( void )
{
	char cStrInput [100] = { 0 }, * pStrDelimiter [3], * pBuf, * pStr,chose[1]={0}, str[10]={0};
	u8 uc = 0,quit=0,quit2=0;      //�˳���־λ
  u32 ul = 0;
//  KEY_Init();
  ESP8266_Choose ( ENABLE );

	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( AP );


	PC_Usart ( "\r\n������Ҫ������WiFi�����ơ����ܷ�ʽ����Կ�����ܷ�ʽ�ı��Ϊ��\
              \r\n0 = OPEN\
              \r\n1 = WEP\
              \r\n2 = WPA_PSK\
	            \r\n3 = WPA2_PSK\
              \r\n4 = WPA_WPA2_PSK\
							\r\n�����ʽΪ�������ַ�+Ӣ�Ķ���+���ܷ�ʽ���+Ӣ�Ķ���+��Կ�ַ�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );
  strcpy(cStrInput, "ATK-ESP8266,4,12345678");
	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

	pBuf = cStrInput;
	uc = 0;
	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
	{
		pStrDelimiter [ uc ++ ] = pStr;
		pBuf = NULL;
	} 
	
	ESP8266_BuildAP ( pStrDelimiter [0], pStrDelimiter [2], pStrDelimiter [1] );
	ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 ); //*
		

	ESP8266_Enable_MultipleId ( ENABLE );
		
	
	PC_Usart ( "\r\n�����������Ҫ�����Ķ˿ںźͳ�ʱʱ�䣨0~28800����λ���룩�������ʽΪ���˿ں��ַ�+Ӣ�Ķ���+��ʱʱ���ַ�+�ո񣬵������\r\n" );

//	scanf ( "%s", cStrInput );
  strcpy(cStrInput, "8086,10000 ");
	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

	pBuf = cStrInput;
	uc = 0;
	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
	{
		pStrDelimiter [ uc ++ ] = pStr;
		pBuf = NULL;
	} 

	ESP8266_StartOrShutServer ( ENABLE, pStrDelimiter [0], pStrDelimiter [1] );
	
	
	do
	{ 
		LED2=!LED2;
		PC_Usart ( "\r\n����ѯ��ģ��IP����\r\n" );
	  ESP8266_Cmd ( "AT+CIFSR", "OK", "Link", 500 );
	  Temp_Humi_Coal();
		PC_Usart ( "\r\n�����ֻ����ӸղŴ�����WiFi������ֻ����һ���ֻ�����ΪID0��Ȼ�����ֻ��������������TCP Client���Ӹղſ����ķ�������AP IP������\r\n" );
		delay_ms ( 60000 ) ;
		a=KEY_Scan(0);
		if(a==KEY_RE)                //ǿ���˳�
		{quit2=1;
			break;
		  }
	}	while ( ! ESP8266_Cmd ( "AT+CIPSTATUS", "+CIPSTATUS:0", 0, 500 ) );
	PC_Usart ( "������Ҫѡ���ģʽ-1�������ֻ���  2�ֻ��������� �����ʽΪ��ģʽ���+�ո�" );
	LED2=1;
	LCD_ShowString(30,210,200,16,16,"Connection    success!");
	chose[0]='2';
	while(1)
	{
		if(chose[0]=='1')
		{
	PC_Usart ( "\r\n������Ҫ��˿��ֻ���ID0�����͵��ַ����������ʽΪ���ַ����������ո�+�ո񣬵������\r\n" );
  
	while(1)
	{
		
//		LCD_ShowString(30,230,200,16,16,"Set up KEY1->");
//	  LCD_ShowString(30,250,200,16,16,"Set up KEY2->");
//	  LCD_ShowString(30,270,200,16,16,"Set up KEY3->");
//		LCD_ShowString(140,230,200,16,16,key_led[key_code[0]-7]);
//		LCD_ShowString(140,250,200,16,16,key_led[key_code[1]-7]);
//		LCD_ShowString(140,270,200,16,16,key_led[key_code[2]-7]);
		a=KEY_Scan(0);                   
		if(a)                            //����
		{
			switch(a)                      
			{
				case KEY_RIGHT:              //��������
				    cStrInput[0]='5';
//			     ul = strlen ( cStrInput[0] );	
	          ESP8266_SendString ( DISABLE, cStrInput, 1, Multiple_ID_0 );
				    break;
				case  KEY_DOWN:              //��������
				    cStrInput[0]='6';
//           ul = strlen ( cStrInput );	
	          ESP8266_SendString ( DISABLE, cStrInput, 1, Multiple_ID_1 );
				    break;
				case   KEY_LEFT:                //key1		
//			      ul = strlen ( cStrInput );	
				    cStrInput[0]='7'+key_code[0];
	          ESP8266_SendString ( DISABLE, cStrInput, 1, Multiple_ID_2 );
				    break;
				case   KEY_CO:                 //key2					
//			      ul = strlen ( cStrInput );	
				    cStrInput[0]='7'+key_code[1];
	          ESP8266_SendString ( DISABLE, cStrInput, 1, Multiple_ID_3 );
				    break;
				case   KEY_LED2:                //key3
//			      ul = strlen ( cStrInput );	
				    cStrInput[0]='7'+key_code[2];
	          ESP8266_SendString ( DISABLE, cStrInput, 1, Multiple_ID_4 );
				    break;
				case   KEY_RE:               //������������   �̰��˳�wifiģʽ
					 
					  if(flag)                 //��������
						{   
						   	LCD_Clear(WHITE);//����
							  if(tp_dev.touchtype!=0XFF)LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");//����������ʾ
							  delay_ms(1000);
							  LCD_Clear(WHITE);//����
								if(tp_dev.touchtype&0X80)ctp_test();	//����������
	              else rtp_test(); 						//����������
/*
//							while(1)
//							{
//							LCD_ShowString(30,230,200,16,16,"Set up ");
//							LCD_ShowString(30,250,200,16,16,"Set up ");
//							LCD_ShowString(30,270,200,16,16,"Set up ");
//						 LED1=1;
//             a=KEY_Scan(0);
//						 if(a==KEY_RIGHT)
//						 {
//							 key_place++;
//							 if(key_place>9) key_place=7;
//						  }
//						 switch(key_place)
//						 {
//							 case  7:                                                     //����KEY1
//							 LCD_ShowString(90,230,200,16,16,"KEY1->");
//							 if(a==KEY_DOWN) key_code[0]++;
//							 if(key_code[0]>10)   key_code[0]=7;
//							 switch(key_code[0])
//							 {
//								 case 7:
//								 LCD_ShowString(140,230,200,16,16,"LED1"); break;
//								 case 8:
//								 LCD_ShowString(140,230,200,16,16,"LED2"); break;
//								 case 9:
//								 LCD_ShowString(140,230,200,16,16,"LED3"); break;
//								 case 10: 
//								 LCD_ShowString(140,230,200,16,16," ALL"); break;
//							  }
//							 break;
//						   case  8:                                                     //����KEY2
//							 LCD_ShowString(90,250,200,16,16,"KEY2->");
//							 if(a==KEY_DOWN) key_code[1]++;
//							 if(key_code[1]>10)   key_code[1]=7;
//							 switch(key_code[1])
//							 {
//								 case 7:
//								 LCD_ShowString(140,250,200,16,16,"LED1"); break;
//								 case 8:
//								 LCD_ShowString(140,250,200,16,16,"LED2"); break;
//								 case 9:
//								 LCD_ShowString(140,250,200,16,16,"LED3"); break;
//								 case 10: 
//								 LCD_ShowString(140,250,200,16,16," ALL"); break;
//							  }
//							 break;
//               case  9:                                                     //����KEY3
//							 LCD_ShowString(90,270,200,16,16,"KEY3->");
//               if(a==KEY_DOWN) key_code[2]++;
//							 if(key_code[2]>10)   key_code[2]=7;
//               switch(key_code[2])
//							 {
//								 case 7:
//								 LCD_ShowString(140,270,200,16,16,"LED1"); break;
//								 case 8:
//								 LCD_ShowString(140,270,200,16,16,"LED2"); break;
//								 case 9:
//								 LCD_ShowString(140,270,200,16,16,"LED3"); break;
//								 case 10: 
//								 LCD_ShowString(140,270,200,16,16," ALL"); break;
//							  }							 
//               break;							 
//						 }
//					   if(a==KEY_LEFT)                                                //��������
//						 {
//							 STMFLASH_Write(FLASH_SAVE_ADDR_START2,(u16*)key_code,SIZE2);
//				      flag=0;
//							break;
//						 }
//						}
*/
              STMFLASH_Write(FLASH_SAVE_ADDR_START2,(u16*)key_code,SIZE2);
             	LCD_ShowString(30,150,200,16,16,"Temp:  C");	 
             	LCD_ShowString(30,170,200,16,16,"Humi:  %");
              LCD_ShowString(30,190,200,16,16,"Coal:  %");
							LCD_ShowString(20,80,200,16,16,"Access control is closed");
							LCD_ShowString(20,60,200,16,16,"wifi mode started ");
              flag=0;
							LED1=0;
						  break;
					}           
//			     ul = strlen ( cStrInput );	              //��ʾ�ͻ��˹ر�
					 cStrInput[0]='4';
	         ESP8266_SendString ( DISABLE, cStrInput, 1, Multiple_ID_0 );
						LED2=0;
				    quit=1;
			      break;
			 }
		 }
		  if(quit==1) break;                          //�˳�
		 	if(quit2==1) break;
 /*
//		if(a==KEY_RIGHT)
//		{
//			strcpy(cStrInput, "5");
//			ul = strlen ( cStrInput );	
//	    ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );
//		 }
//		if(a==KEY_DOWN)
//    {	strcpy(cStrInput, "6");
//			ul = strlen ( cStrInput );	
//	    ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );
//		}
//		if(a==KEY_LEFT)
//		{
//			strcpy(cStrInput, "5");
//			ul = strlen ( cStrInput );	
//	    ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_1 );
//		 }
//		if(a==KEY_RE) 
//		{ LED2=0;
////			break;
//		}
//	scanf ( "%s", cStrInput );
//	ul = strlen ( cStrInput );	
//	ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );
*/
}
	}
	if(chose[0]=='2')
	{
	  PC_Usart ( "\r\n�����ֻ�����������ַ����ַ���\r\n" );

		TIM_Cmd(TIM4, ENABLE);                        //���ڷ�����
		
	 while (1)
	 { 
	  Temp_Humi_Coal();                            //��ʪ�ȼ��
		 if(sleep_flag==1)                           //����wifi����   30s��һ��
		{
		strcpy(cStrInput, "sleep");         
	  ul = strlen ( cStrInput );	
	  ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );
		ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_1 );
		ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_2 );
		ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_3 );
		ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_4 );
		sleep_flag=0;
		}
	  pStr = ESP8266_ReceiveString ( DISABLE );
		 if(pStr)                                    //�յ�����.����
		  { 
         CNT3=0;				
        PC_Usart ( "%s\r\n", pStr );
				send_flag=1;
				while(send_flag)
				{
		    for(i=0;i<14;i++)
		    {
       		 *pStr++;
       		  if(i==6)                             //�ж�ID��
       		  {
       			 strncpy(str,pStr,1);
       			 str[1]='\0';
        		 address=atoi(str);                  //��id��תΪ����
       		   PC_Usart ( "IPD=%d\r", address );
       		  }
       		  if(i==10)                             //�жϽ��յ������� 
       		  {
					 	address_data=atoi(pStr);              //������תΪ����
       			PC_Usart ( "data=%d\r", address_data );
//						if(address_data>10)
//					 	{
							client_ID=address_data/10;
							PC_Usart ( "client_ID=%d\r", client_ID );
							client_IPD[client_ID]=address;
//						}
       		  }
		   }
			   
//			   if(key_code[address*5+address_data]%5==0)  itoa(5,cStrInput,10);
//				 else itoa(key_code[address*5+address_data]%5,cStrInput,10);   //��Ҫ���͵�����תΪ�ַ���
			   if(key_code[client_ID*5+address_data%10]%5==0)    itoa(5,cStrInput,10);
				 else    itoa(key_code[client_ID*5+address_data%10]%5,cStrInput,10);   //��Ҫ���͵�����תΪ�ַ���
			   address=(key_code[client_ID*5+address_data%10]-1)/5;            //���͸�ID��Ϊaddress�Ŀͻ���
//			   cStrInput[0]='0';
			   ul = strlen ( cStrInput );
	       ESP8266_SendString ( DISABLE, cStrInput, ul,( ENUM_ID_NO_TypeDef ) client_IPD[address] );
				 pStr=0;
			   send_flag=0;
		    }
		   }
			a=KEY_Scan(0);
			 if(a)                                          //����
		  {
			  switch(a)                      
			  {
					case   KEY_RE:                              //������������   �̰��˳�wifiģʽ
					 
					  if(flag)                                   //��������
						{   
						   	LCD_Clear(WHITE);//����
							  if(tp_dev.touchtype!=0XFF)LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");//����������ʾ
							  delay_ms(1000);
							  LCD_Clear(WHITE);                     //����
								if(tp_dev.touchtype&0X80)ctp_test();	//����������
	              else rtp_test(); 						          //����������
              STMFLASH_Write(FLASH_SAVE_ADDR_START2,(u16*)key_code,SIZE2);
             	LCD_ShowString(30,150,200,16,16,"Temp:  C");	 
             	LCD_ShowString(30,170,200,16,16,"Humi:  %");
              LCD_ShowString(30,190,200,16,16,"Coal:  %");
							LCD_ShowString(20,80,200,16,16,"Access control is closed");
							LCD_ShowString(20,60,200,16,16,"wifi mode started ");
              flag=0;
							LED1=0;
						  break;
					}           

//					  strcpy(cStrInput, "0");  
            cStrInput[0]='0';        					//��ʾ�ͻ��˹ر�    
	          ul = strlen ( cStrInput );
	          ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );
					  ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_1 );
					  ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_2 );
					  ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_3 );
					  ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_4 );
						LED2=0;
				    quit=1;
			      break;
			  }
		  }
			 if(quit==1||quit2==1)   break;                    //�˳�
	 }
	
  }
	     if(quit==1||quit2==1)   break;                     //�˳�
}
}


/*
 * ��������ESP8266_StaTcpClient_ApTcpServer
 * ����  ��WF-ESP8266ģ�����STA(TCP Client)+AP(TCP Server)����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_StaTcpClient_ApTcpServer ( void )
{ 
	char cStrInput [100] = { 0 }, * pStrDelimiter [3], * pBuf, * pStr;
	u8 uc = 0;
  u32 ul = 0;
  ESP8266_Choose ( ENABLE );

	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA_AP );


	PC_Usart ( "\r\n������Ҫ������WiFi�����ơ����ܷ�ʽ����Կ�����ܷ�ʽ�ı��Ϊ��\
						\r\n0 = OPEN\
						\r\n1  =WEP\
						\r\n2 = WPA_PSK\
						\r\n3 = WPA2_PSK\
						\r\n4 = WPA_WPA2_PSK\
						\r\n�����ʽΪ�������ַ�+Ӣ�Ķ���+���ܷ�ʽ���+Ӣ�Ķ���+��Կ�ַ�+�ո񣬵������\r\n" );

	scanf ( "%s", cStrInput );

	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

	pBuf = cStrInput;
	uc = 0;
	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
	{
		pStrDelimiter [ uc ++ ] = pStr;
		pBuf = NULL;
	} 
	
	ESP8266_BuildAP ( pStrDelimiter [0], pStrDelimiter [2], pStrDelimiter [1] );
	ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 ); //*
	

	ESP8266_Cmd ( "AT+CWLAP", "OK", 0, 5000 );
		
  do
	{
		PC_Usart ( "\r\n������Ҫ���ӵ�WiFi���ƺ���Կ�������ʽΪ�������ַ�+Ӣ�Ķ���+��Կ�ַ�+�ո񣬵������\r\n" );

		scanf ( "%s", cStrInput );

		PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

		pBuf = cStrInput;
		uc = 0;
		while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
		{
			pStrDelimiter [ uc ++ ] = pStr;
			pBuf = NULL;
		} 
		
  } while ( ! ESP8266_JoinAP ( pStrDelimiter [0], pStrDelimiter [1] ) );

	
	ESP8266_Enable_MultipleId ( ENABLE );
		
	
	PC_Usart ( "\r\n�����������Ҫ�����Ķ˿ںźͳ�ʱʱ�䣨0~28800����λ���룩�������ʽΪ���˿ں��ַ�+Ӣ�Ķ���+��ʱʱ���ַ�+�ո񣬵������\r\n" );

	scanf ( "%s", cStrInput );

	PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

	pBuf = cStrInput;
	uc = 0;
	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
	{
		pStrDelimiter [ uc ++ ] = pStr;
		pBuf = NULL;
	} 

	ESP8266_StartOrShutServer ( ENABLE, pStrDelimiter [0], pStrDelimiter [1] );
	
	
	do 
	{
		PC_Usart ( "\r\n���ڵ����Ͻ��������������TCP Server�������磬��������Ե�IP�Ͷ˿ںţ������ʽΪ������IP+Ӣ�Ķ���+�˿ں�+�ո񣬵������\r\n" );

		scanf ( "%s", cStrInput );

		PC_Usart ( "\r\n�Ե�Ƭ�� ����\r\n" );

		pBuf = cStrInput;
		uc = 0;
		while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
		{
			pStrDelimiter [ uc ++ ] = pStr;
			pBuf = NULL;
		} 
		
  } while ( ! ( ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_0 ) &&
	              ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_1 ) &&
	              ESP8266_Link_Server ( enumTCP, pStrDelimiter [0], pStrDelimiter [1], Multiple_ID_2 ) ) );
		
	
	do
	{
		PC_Usart ( "\r\n����ѯ��ģ��IP��ǰһ��ΪAP IP����һ��ΪSTA IP����\r\n" );
	  ESP8266_Cmd ( "AT+CIFSR", "OK", "Link", 500 );
		
		PC_Usart ( "\r\n�����ֻ����ӸղŴ�����WiFi������ֻ����һ���ֻ�����ΪID3��Ȼ�����ֻ��������������TCP Client���Ӹղſ����ķ�������AP IP������\r\n" );
		delay_ms ( 20000 ) ;
	}	while ( ! ESP8266_Cmd ( "AT+CIPSTATUS", "+CIPSTATUS:3", 0, 500 ) );
	

	for ( uc = 0; uc < 3; uc ++ )
	{
		PC_Usart ( "\r\n������˿�ID%dҪ���͵��ַ����������ʽΪ���ַ����������ո�+�ո񣬵������\r\n", uc );

		scanf ( "%s", cStrInput );

		ul = strlen ( cStrInput );
		
		ESP8266_SendString ( DISABLE, cStrInput, ul, ( ENUM_ID_NO_TypeDef ) uc );
		
	}
	
	
	PC_Usart ( "\r\n������Ҫ��˿��ֻ���ID3�����͵��ַ����������ʽΪ���ַ����������ո�+�ո񣬵������\r\n" );

	scanf ( "%s", cStrInput );

	ul = strlen ( cStrInput );
	
	ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_3 );

	
	PC_Usart ( "\r\n���ڵ�������������ֻ��ֻ�����������ַ����ַ�������\r\n" );
	while (1)
	{
	  pStr = ESP8266_ReceiveString ( DISABLE );
		PC_Usart ( "%s", pStr );
	}
}

void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);	//����   
 	POINT_COLOR=BLUE;	//��������Ϊ��ɫ 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//��ʾ��������
  	POINT_COLOR=RED;	//���û�����ɫ 
}
void rtp_test(void)
{
	u8 key,keyscan; 
  u8 line=0,i=0;	
	u16 key_p=0,led_code=0;
	char  code_place[6]={0};
	atk_8266_load_keyboard(0,20);			//��ʾ���� 
	LCD_ShowString(30,230-line,200,16,16,"Set up KEY   ---LED");
	LCD_ShowString(85,210-line,200,16,16,"___");
	while(1)
	{ 
		tp_dev.scan(0);
		keyscan=KEY_Scan(0);
		LCD_ShowNum(109,230-line,key_p,3,16);
	  LCD_ShowNum(181,230-line,led_code,3,16);
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{
		key=atk_8266_get_keynum(0,20);
		  if(key==1)                    //���ð���ֵ
		  {   
				i=0;
        key_p=code_place[0]=code_place[1]=code_place[2]=0;				
	    }	
		  if(key==2)                    //����ledֵ
	  	{
				i=3;
				led_code=code_place[3]=code_place[4]=code_place[5]=0;;
		   }
			if(key==3)                     //��ɾ
	  	{
				i--;
				code_place[i]=0; 
				if(i==255) i=0;
		   }
			if(key>3&&key<13)                //����1-10
			{
				code_place[i]=key-3;
				i++;
				if(i>6) i=6;
			 }
			if(key==14)                        //����0
			{
				code_place[i]=0x0a;
				i++;
				if(i>6) i=6;
			 }
				if(i<3)                           //�ϱ���
				{
				LCD_ShowString(85,210-line,200,16,16,"___"); 
        LCD_ShowString(157,210-line,200,16,16,"   ");
				}
				if(i>2) 
				{
				LCD_ShowString(85,210-line,200,16,16,"   ");
			  LCD_ShowString(157,210-line,200,16,16,"___");
				}
				if(!code_place[0])          key_p=0;
			  if(code_place[0])          key_p=code_place[0];
				if(code_place[0]==0x0a)    key_p=code_place[0]=0;
				if(code_place[1])          key_p=code_place[0]*10+code_place[1];
				if(code_place[1]==0x0a)    key_p=code_place[0]*10;
				if(code_place[2])          
				{
					if(code_place[1]==0x0a)  key_p=code_place[0]*100+code_place[2];
					else key_p=code_place[0]*100+code_place[1]*10+code_place[2];
				
				}
				if(code_place[2]==0x0a)    
				{
					if(code_place[1]==0x0a)  key_p=code_place[0]*100;
					else key_p=code_place[0]*100+code_place[1]*10;
				}
				if(!code_place[3])         led_code=0;                           //����ֵ
				if(code_place[3])          led_code=code_place[3];
				if(code_place[3]==0x0a)    led_code=code_place[3];
				if(code_place[4])          led_code=code_place[3]*10+code_place[4];
				if(code_place[4]==0x0a)    led_code=code_place[3]*10;
				if(code_place[5])         
        {
				  if(code_place[4]==0x0a)  led_code=code_place[3]*100+code_place[5];
				  else  led_code=code_place[3]*100+code_place[4]*10+code_place[5];
				}
				if(code_place[5]==0x0a)         
        {
				  if(code_place[4]==0x0a)  led_code=code_place[3]*100;
				  else  led_code=code_place[3]*100+code_place[4]*10;
				}
				
			if(key==13)                                    //ȷ�ϼ�
			{
				key_code[key_p]=led_code;
				LCD_ShowString(30,250-line,200,16,16,"KEY    has saved");
				LCD_ShowNum(54,250-line,key_p,3,16);
				code_place[0]=code_place[1]=code_place[2]=code_place[3]=code_place[4]=code_place[5]=0;
			 }
			if(key==15)                                     //�˳���
			{	
				LCD_Clear(WHITE);//����
				break; 
			}
	 }		
		delay_ms(10);	//û�а������µ�ʱ��
		if(keyscan==KEY_RIGHT)	//KEY0����,��ִ��У׼����
		{
			LCD_Clear(WHITE);//����
		    TP_Adjust();  	//��ĻУ׼  
			Load_Drow_Dialog();
			atk_8266_load_keyboard(0,20);
		}
	}
}
//���ݴ��������Ժ���
void ctp_test(void)
{
 	u8 key;
	u8 timex=0; 
  u8 line=100;	
//	char  key_code[3]={7,8,9};
	 atk_8266_load_keyboard(0,180);			//��ʾ���� 
	LCD_ShowString(30,230+line,200,16,16,"Set up KEY1->");
	LCD_ShowString(30,250+line,200,16,16,"Set up KEY2->");
	LCD_ShowString(30,270+line,200,16,16,"Set up KEY3->");
	while(1)
	{ 
		key=atk_8266_get_keynum(0,180);
		if(key)
		{   
			if(key>3&&key<7)
			{
				key_code[key-4]=7;
				LCD_ShowString(140,230+line+20*(key-4),200,16,16,"LED1");
			}
			if(key>6&&key<10)
			{
				key_code[key-7]=8;
				LCD_ShowString(140,230+line+20*(key-7),200,16,16,"LED2");
			 }
			if(key>9&&key<13)
			{
				key_code[key-10]=9;
				LCD_ShowString(140,230+line+20*(key-10),200,16,16,"LED3");
			 }
			if(key>12&&key<16)
			{
				key_code[key-13]=10;
				LCD_ShowString(140,230+line+20*(key-13),200,16,16," ALL");
			 } 	
		} 
		timex++;
		if(timex==20)
		{
			timex=0;
			LED0=!LED0;
		}
		delay_ms(10);
	}
}
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
  u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 

	        if(*str>0x80) bHz=1;//���� 
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
      				 
    }   
}
//�õ�������������
//x,y:��������
//����ֵ��������ֵ��1~15��Ч��0,��Ч��
u8 atk_8266_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,û���κΰ������£�1~15��1~15�Ű�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*80+80)&&tp_dev.x[0]>(x+j*80)&&tp_dev.y[0]<(y+i*28+28)&&tp_dev.y[0]>(y+i*28))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					atk_8266_key_staset(x,y,key_x-1,0);
					key_x=key;
					atk_8266_key_staset(x,y,key_x-1,1);
				}
				break;
			}
		}  
	}else if(key_x) 
	{
		atk_8266_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}
//����״̬����
//x,y:��������
//key:��ֵ��0~8��
//sta:״̬��0���ɿ���1�����£�
void atk_8266_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>15)return;
	if(sta)LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,GREEN);
	else LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,WHITE); 
 Show_Str_Mid(x+j*80,y+6+28*i,(u8*)kbd_tbl[keyx],16,80);		 		 
}
void atk_8266_load_keyboard(u16 x,u16 y)
{
	u16 i;
	POINT_COLOR=RED; 
	LCD_Fill(x,y,x+240,y+140,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+140);						   
	LCD_DrawRectangle(x+80,y,x+160,y+140);	 
	LCD_DrawRectangle(x,y+28,x+240,y+56);
	LCD_DrawRectangle(x,y+84,x+240,y+112);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		Show_Str_Mid(x+(i%3)*80,y+6+28*(i/3),(u8*)kbd_tbl[i],16,80);

	}  		 					   
}
