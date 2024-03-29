#include <stm32f10x_lib.h>
#include "RS606.h"

void RS606SetMode(RS606_MODE mode)
{
volatile int temp;

	/* Met les lignes PB9 (TX_CMD) PB8 (RX_CMD) en sortie : fait dans stm32_Init.c */

	/* remet PB8 et PB9 � 0 */
	GPIOB->ODR = GPIOB->ODR & ~(GPIO_Pin_8 + GPIO_Pin_9);

	switch (mode)
	{
		case RS606_OFF:
			GPIOB->ODR = GPIOB->ODR | GPIO_Pin_8; // met la ligne RX_CMD � 1 (donc 0 en entr�e du RS606) */
			GPIOB->ODR = GPIOB->ODR | GPIO_Pin_9; // met la ligne TX_CMD � 1 (donc 0 en entr�e du RS606) */
			break;
		case RS606_TX:
			GPIOB->ODR = GPIOB->ODR | GPIO_Pin_8; // met la ligne RX_CMD � 1 (donc 0 en entr�e du RS606) */ 
			break;
		case RS606_RX:
			GPIOB->ODR = GPIOB->ODR | GPIO_Pin_9; // met la ligne TX_CMD � 1 (donc 0 en entr�e du RS606) */
			break;
		case RS606_TX_LOOPBACK:
			/* Rien a faire (car un 0 en sortie du STM32 donne un 1 en entr�e du module RS606) */
			break;
	}
}
