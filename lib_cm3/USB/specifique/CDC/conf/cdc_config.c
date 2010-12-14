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
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

u8  USART_Rx_Buffer [USART_RX_DATA_SIZE]; 
u32 USART_Rx_ptr_in = 0;
u32 USART_Rx_ptr_out = 0;
u32 USART_Rx_length  = 0;

u8  USB_Tx_State = 0;
static void IntToUnicode (u32 value , u8 *pbuf , u8 len);
/* Extern variables ----------------------------------------------------------*/

extern LINE_CODING linecoding;

/**
  * @brief  Sets the selected data port bits.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSRR = GPIO_Pin;
}

/**
  * @brief  Clears the selected data port bits.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Init_USB_CDC(void)
{
	/* Les horloges systeme sont déja initialisées */

  	/* Enable USB_DISCONNECT GPIO clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* Configure USB pull-up pin */
//  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

   /* Select USBCLK source */
//  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

  /* Enable USB interrupts */
  USB_Interrupts_Config();

  /* Start USB subssytem */
  USB_Init();
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
//  NVIC_InitTypeDef NVIC_InitStructure;
//
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//
//  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//

	NVIC_SET_PRIO_PERIPH(USB_LP_CAN_RX0, 3);
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
		USB_DISCONNECT->BSRR = USB_DISCONNECT_PIN;
	}
	else
	{
		//GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		USB_DISCONNECT->BRR = USB_DISCONNECT_PIN;
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
* Function Name  : USB_To_USART_Send_Data.
* Description    : send the received data from USB to the UART 0.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void USB_To_USART_Send_Data(u8* data_buffer, u8 Nb_bytes)
{
  
//  u32 i;
//  
//  for (i = 0; i < Nb_bytes; i++)
//  {
//    USART_SendData(EVAL_COM1, *(data_buffer + i));
//    while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET); 
//  }  

/* Todo: remplir un buffer tempon pour stocker les donnée en reception (USB -> Appli) */
}

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
    if (USART_Rx_ptr_out == USART_RX_DATA_SIZE)
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
      USART_Rx_length = USART_RX_DATA_SIZE - USART_Rx_ptr_out;
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
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void USART_To_USB_Send_Data(void)
{
  
//  if (linecoding.datatype == 7)
//  {
//    USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1) & 0x7F;
//  }
//  else if (linecoding.datatype == 8)
//  {
//    USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1);
//  }
//  
//  USART_Rx_ptr_in++;
//  
//  /* To avoid buffer overflow */
//  if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
//  {
//    USART_Rx_ptr_in = 0;
//  }

/* Todo: probablement rien, sinon, mettre un buffer circulaire pour l'envoi des données (APP -> USB) */
}

/*******************************************************************************
* Function Name  : Loopback_To_USB_Send_Data.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Loopback_To_USB_Send_Data(u8* data_buffer, u8 Nb_bytes)
{
  u32 i;

  for (i = 0; i < Nb_bytes; i++)
  {
	  if (linecoding.datatype == 7)
	  {
	    USART_Rx_Buffer[USART_Rx_ptr_in] = data_buffer[i] & 0x7F;
	  }
	  else if (linecoding.datatype == 8)
	  {
	    USART_Rx_Buffer[USART_Rx_ptr_in] = data_buffer[i];
	  }
	  
	  USART_Rx_ptr_in++;
	  
	  /* To avoid buffer overflow */
	  if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
	  {
	    USART_Rx_ptr_in = 0;
	  }
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
