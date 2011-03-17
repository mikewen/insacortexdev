/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Virtual Com Port Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
#include "stm_regs.h"

#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_cdc.h"
#include "usb_pwr.h"
#include "stm_clock.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 tampon;
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Init_Periphs(void);

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
	/* Init des peripheriques (en gros, l'horloge et l'USB) */
	Init_Periphs();
	  
	while (1)
	{
		/* Si des données sont arrivées sur le port COM virtuel */
		if (USB_CDC_RXBufferLength()!=0)
		{
			/* Loopback, mais avec les data +1 */
			USB_CDC_ReadData(&tampon, 1);
			tampon ++;
			USB_CDC_SendData(&tampon, 1);
		}
	}
}

void Init_Periphs(void)
{
	/* Demarrage des horloges */
	Init_Clock_System();

	/* Demarrage de l'USB en classe CDC*/
	USB_CDC_Init();
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
