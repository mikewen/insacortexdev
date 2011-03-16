/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm_regs.h"
#include "usb_conf.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "usb_cdc.h"
#include "stm_clock.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_nvic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

u8  USART_Rx_Buffer [BUFFER_RX_DATA_SIZE]; 
u32 USART_Rx_ptr_in = 0;
u32 USART_Rx_ptr_out = 0;
u32 USART_Rx_length  = 0;

u8  USART_Tx_Buffer [BUFFER_TX_DATA_SIZE]; 
u32 USART_Tx_ptr_in = 0;
u32 USART_Tx_ptr_out = 0;
u32 USART_Tx_length  = 0;

u8  USB_Tx_State = 0;
static void IntToUnicode (u32 value , u8 *pbuf , u8 len);
void USB_CDC_SendByte(u8 data);
void USB_To_Buffer_Send_Data(u8* data_buffer, u8 Nb_bytes);

#define USB_LP_CAN1_RX0_IRQn 20

/* Extern variables ----------------------------------------------------------*/

extern LINE_CODING linecoding;

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_CDC_Init(void)
{

	/* Les horloges systeme sont déja initialisées */

  	/* Enable USB_DISCONNECT GPIO clock */
	RCC->APB2ENR |= RCC_IOPDEN;

  	/* Configure USB pull-up pin */
	GPIOD->CRL =  GPIOD->CRL |
	       ((GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_2_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_2_SHIFT));
		   
	GPIOD->ODR = GPIOD->ODR & ~(GPIO_PIN_2);
	 
   	/* Select USBCLK source */
#if (((__SYSCLK)!=72000000UL) && ((__SYSCLK)!=48000000UL ))
#error "SYSCLK doit etre egal à 72 Mhz ou 48 Mhz pour pouvoir utiliser l'USB "
#endif

#if ((__SYSCLK)==72000000UL)
// PLL USB reglé à 1.5 */ 
	RCC ->CFGR &= ~(RCC_USBPRE);
#else
// PLL USB reglé à 1 */
	RCC ->CFGR |= RCC_USBPRE;
#endif /* ((__SYSCLK)==72000000UL) */

  	/* Enable the USB clock */
	RCC->APB1ENR |= RCC_USBEN;

  	/* Enable USB interrupts */
  	USB_Interrupts_Config();

  	/* Start USB subssytem */
  	USB_Init();
}

/*******************************************************************************
* Function Name  : USB_CDC_SendByte.
* Description    : send the received data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void USB_CDC_SendByte(u8 data)
{	
	if (linecoding.datatype == 7)
	{
		USART_Rx_Buffer[USART_Rx_ptr_in] = data & 0x7F;
	}
	else if (linecoding.datatype == 8)
	{
		USART_Rx_Buffer[USART_Rx_ptr_in] = data;
	}
	
	USART_Rx_ptr_in++;
	
	/* To avoid buffer overflow */
	if(USART_Rx_ptr_in == BUFFER_RX_DATA_SIZE)
	{
		USART_Rx_ptr_in = 0;
	}
}

/*******************************************************************************
* Function Name  : USB_To_USART_Send_Data.
* Description    : send the received data from USB to buffer.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void USB_To_Buffer_Send_Data(u8* data_buffer, u8 Nb_bytes)
{
int i;

	for (i=0; i<Nb_bytes; i++)
	{
		USART_Tx_Buffer[USART_Tx_ptr_in]= data_buffer[i];

		USART_Tx_ptr_in ++;
		USART_Tx_length ++;

		if (USART_Tx_ptr_in>= BUFFER_TX_DATA_SIZE) USART_Tx_ptr_in=0; 
	} 
}

/*******************************************************************************
* Function Name  : USB_CDC_ReadData.
* Description    : read a buffer of data from USB buffer.
* Input          : len: length of data to send
* Output         : buffer: array of unsigned byte.
* Return         : length of data sent.
*******************************************************************************/
int USB_CDC_ReadData(u8 *buffer, int len)
{	
int i;
int length;

	length = len;
	if (length > USART_Tx_length) length = USART_Tx_length;
	
	for (i=0; i<length; i++)
	{
		buffer[i] = USART_Tx_Buffer[USART_Tx_ptr_out];

	    USART_Tx_ptr_out ++;
		USART_Tx_length --;

		if (USART_Tx_ptr_out >=	BUFFER_TX_DATA_SIZE) USART_Tx_ptr_out=0; 
	}

	return length;
}

/*******************************************************************************
* Function Name  : USB_CDC_SendData.
* Description    : send a buffer of data to USB.
* Input          : buffer: array of unsigned byte.
*                : len: length of data to send
* Return         : length of data sent.
*******************************************************************************/
int USB_CDC_SendData(u8 *buffer, int len)
{	
int i;
	
	for (i=0; i<len; i++)
	{
		USB_CDC_SendByte(buffer[i]);
	}

	return len;
}

/*******************************************************************************
* Function Name  : USB_CDC_RXBufferLength.
* Description    : get RX buffer length.
* Input          : nothing
* Return         : length of buffer.
*******************************************************************************/
int USB_CDC_RXBufferLength(void)
{
	return USART_Tx_length; 
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
	NVIC_SET_PRIO_PERIPH(USB_LP_CAN_RX0, USB_LP_IT_LEVEL);
	NVIC_ENABLE_PERIPH_IT(USB_LP_CAN_RX0);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		//GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		GPIOD->BRR = GPIO_PIN_2;
	}
	else
	{
		//GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		GPIOD->BSRR = GPIO_PIN_2;
	}
}

/*******************************************************************************
* Function Name  :  USART_Config_Default.
* Description    :  configure the EVAL_COM1 with default values.
* Input          :  None.
* Return         :  None.
*******************************************************************************/
//void USART_Config_Default(void)
//{
//  /* EVAL_COM1 default configuration */
//  /* EVAL_COM1 configured as follow:
//        - BaudRate = 9600 baud  
//        - Word Length = 8 Bits
//        - One Stop Bit
//        - Parity Odd
//        - Hardware flow control desabled
//        - Receive and transmit enabled
//  */
//  USART_InitStructure.USART_BaudRate = 9600;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_Odd;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//
//  /* Configure and enable the USART */
//  STM_EVAL_COMInit(COM1, &USART_InitStructure);
//
//  /* Enable the USART Receive interrupt */
//  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
//}

/*******************************************************************************
* Function Name  :  USART_Config.
* Description    :  Configure the EVAL_COM1 according to the linecoding structure.
* Input          :  None.
* Return         :  Configuration status
                    TRUE : configuration done with success
                    FALSE : configuration aborted.
*******************************************************************************/
bool USART_Config(void)
{
	/* TODO: Penser a rajouter un callback pour gerer la config de l'UART */

	return (TRUE);
}
//{
//
//  /* set the Stop bit*/
//  switch (linecoding.format)
//  {
//    case 0:
//      USART_InitStructure.USART_StopBits = USART_StopBits_1;
//      break;
//    case 1:
//      USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
//      break;
//    case 2:
//      USART_InitStructure.USART_StopBits = USART_StopBits_2;
//      break;
//    default :
//    {
//      USART_Config_Default();
//      return (FALSE);
//    }
//  }
//
//  /* set the parity bit*/
//  switch (linecoding.paritytype)
//  {
//    case 0:
//      USART_InitStructure.USART_Parity = USART_Parity_No;
//      break;
//    case 1:
//      USART_InitStructure.USART_Parity = USART_Parity_Even;
//      break;
//    case 2:
//      USART_InitStructure.USART_Parity = USART_Parity_Odd;
//      break;
//    default :
//    {
//      USART_Config_Default();
//      return (FALSE);
//    }
//  }
//
//  /*set the data type : only 8bits and 9bits is supported */
//  switch (linecoding.datatype)
//  {
//    case 0x07:
//      /* With this configuration a parity (Even or Odd) should be set */
//      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//      break;
//    case 0x08:
//      if (USART_InitStructure.USART_Parity == USART_Parity_No)
//      {
//        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//      }
//      else 
//      {
//        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
//      }
//      
//      break;
//    default :
//    {
//      USART_Config_Default();
//      return (FALSE);
//    }
//  }
//
//  USART_InitStructure.USART_BaudRate = linecoding.bitrate;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
// 
//  /* Configure and enable the USART */
//  STM_EVAL_COMInit(COM1, &USART_InitStructure);
//
//  return (TRUE);
//}


/*******************************************************************************
* Function Name  : Handle_USBAsynchXfer.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Handle_USBAsynchXfer (void)
{
  
  u16 USB_Tx_ptr;
  u16 USB_Tx_length;
  
  if(USB_Tx_State != 1)
  {
    if (USART_Rx_ptr_out == BUFFER_RX_DATA_SIZE)
    {
      USART_Rx_ptr_out = 0;
    }
    
    if(USART_Rx_ptr_out == USART_Rx_ptr_in) 
    {
      USB_Tx_State = 0; 
      return;
    }
    
    if(USART_Rx_ptr_out > USART_Rx_ptr_in) /* rollback */
    { 
      USART_Rx_length = BUFFER_RX_DATA_SIZE - USART_Rx_ptr_out;
    }
    else 
    {
      USART_Rx_length = USART_Rx_ptr_in - USART_Rx_ptr_out;
    }
    
    if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE)
    {
      USB_Tx_ptr = USART_Rx_ptr_out;
      USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;
      
      USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;	
      USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;	
    }
    else
    {
      USB_Tx_ptr = USART_Rx_ptr_out;
      USB_Tx_length = USART_Rx_length;
      
      USART_Rx_ptr_out += USART_Rx_length;
      USART_Rx_length = 0;
    }
    USB_Tx_State = 1; 
    
    UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
    SetEPTxCount(ENDP1, USB_Tx_length);
    SetEPTxValid(ENDP1); 
  }  
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(volatile u32*)(0x1FFFF7E8);
  Device_Serial1 = *(volatile u32*)(0x1FFFF7EC);
  Device_Serial2 = *(volatile u32*)(0x1FFFF7F0);

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (u32 value , u8 *pbuf , u8 len)
{
  u8 idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
