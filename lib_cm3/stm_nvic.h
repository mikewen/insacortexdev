#ifndef __STM_NVIC_H__
#define __STM_NVIC_H__

#include <stm32f10x_lib.h>

/* Registers definition */
/* NVIC and SCB registers are already defined by stm32f10x_lib.h */
/* Other NVIC registers are defined here. 
   Pages are ones of ARM CORTEX Reference Manual (r2p0) (ARM DDI 0337G)*/

#define NVIC_ICTR 	((u32 *)((u32)0xE000E004))  /* Interrupt Controller Type Register, P8-7 */
#define NVIC_ACR 	((u32 *)((u32)0xE000E008))  /* Auxiliary Control Register */

/* Peripheral and user interrupts priority setting */

#define WWDG_VECT_INDEX 			0
#define PVD_VECT_INDEX 				0
#define TAMPER_VECT_INDEX 			0
#define RTC_VECT_INDEX 				0
#define FLASH_VECT_INDEX 			1
#define RCC_VECT_INDEX 				1
#define EXTI0_VECT_INDEX 			1
#define EXTI1_VECT_INDEX 			1
#define EXTI2_VECT_INDEX 			2
#define EXTI3_VECT_INDEX 			2
#define EXTI4_VECT_INDEX 			2
#define DMA1_CHANNEL1_VECT_INDEX 	2
#define DMA1_CHANNEL2_VECT_INDEX 	3
#define DMA1_CHANNEL3_VECT_INDEX 	3
#define DMA1_CHANNEL4_VECT_INDEX 	3
#define DMA1_CHANNEL5_VECT_INDEX 	3
#define DMA1_CHANNEL6_VECT_INDEX 	4
#define DMA1_CHANNEL7_VECT_INDEX 	4
#define ADC1_2_VECT_INDEX 			4
#define USB_HP_CAN_TX_VECT_INDEX 	4
#define USB_LP_CAN_RX0_VECT_INDEX 	5
#define CAN_RX1_VECT_INDEX 			5
#define CAN_SCE_VECT_INDEX 			5
#define EXTI9_5_VECT_INDEX 			5
#define TIM1_BRK_VECT_INDEX 		6
#define TIM1_UP_VECT_INDEX 			6
#define TIM1_TRG_COM_VECT_INDEX 	6
#define TIM1_CC_VECT_INDEX 			6
#define TIM2_VECT_INDEX 			7
#define TIM3_VECT_INDEX 			7
#define TIM4_VECT_INDEX 			7
#define I2C1_EV_VECT_INDEX 			7
#define I2C1_ER_VECT_INDEX 			8
#define I2C2_EV_VECT_INDEX 			8
#define I2C2_ER_VECT_INDEX 			8
#define SPI1_VECT_INDEX 			8
#define SPI2_VECT_INDEX 			9
#define USART1_VECT_INDEX 			9
#define USART2_VECT_INDEX 			9
#define USART3_VECT_INDEX 			9
#define EXTI15_10_VECT_INDEX 		10
#define RTCALARM_VECT_INDEX 		10
#define USBWAKEUP_VECT_INDEX 		10
#define TIM8_BRK_VECT_INDEX 		10
#define TIM8_UP_VECT_INDEX 			11
#define TIM8_TRG_COM_VECT_INDEX 	11
#define TIM8_CC_VECT_INDEX 			11
#define ADC3_VECT_INDEX 			11
#define FSMC_VECT_INDEX 			12
#define SDIO_VECT_INDEX 			12
#define TIM5_VECT_INDEX 			12
#define SPI3_VECT_INDEX 			12
#define UART4_VECT_INDEX 			13
#define UART5_VECT_INDEX 			13
#define TIM6_VECT_INDEX 			13
#define TIM7_VECT_INDEX 			13
#define DMA2_CHANNEL1_VECT_INDEX 	14
#define DMA2_CHANNEL2_VECT_INDEX 	14
#define DMA2_CHANNEL3_VECT_INDEX 	14
#define DMA2_CHANNEL4_5_VECT_INDEX 	14

#define WWDG_VECT_SHIFT 			4
#define PVD_VECT_SHIFT 				12
#define TAMPER_VECT_SHIFT 			20
#define RTC_VECT_SHIFT 				28
#define FLASH_VECT_SHIFT 			4
#define RCC_VECT_SHIFT 				12
#define EXTI0_VECT_SHIFT 			20
#define EXTI1_VECT_SHIFT 			28
#define EXTI2_VECT_SHIFT 			4
#define EXTI3_VECT_SHIFT 			12
#define EXTI4_VECT_SHIFT 			20
#define DMA1_CHANNEL1_VECT_SHIFT 	28
#define DMA1_CHANNEL2_VECT_SHIFT 	4
#define DMA1_CHANNEL3_VECT_SHIFT 	12
#define DMA1_CHANNEL4_VECT_SHIFT 	20
#define DMA1_CHANNEL5_VECT_SHIFT 	28
#define DMA1_CHANNEL6_VECT_SHIFT 	4
#define DMA1_CHANNEL7_VECT_SHIFT 	12
#define ADC1_2_VECT_SHIFT 			20
#define USB_HP_CAN_TX_VECT_SHIFT 	28
#define USB_LP_CAN_RX0_VECT_SHIFT 	4
#define CAN_RX1_VECT_SHIFT 			12
#define CAN_SCE_VECT_SHIFT 			20
#define EXTI9_5_VECT_SHIFT 			28
#define TIM1_BRK_VECT_SHIFT 		4
#define TIM1_UP_VECT_SHIFT 			12
#define TIM1_TRG_COM_VECT_SHIFT 	20
#define TIM1_CC_VECT_SHIFT 			28
#define TIM2_VECT_SHIFT 			4
#define TIM3_VECT_SHIFT 			12
#define TIM4_VECT_SHIFT 			20
#define I2C1_EV_VECT_SHIFT 			28
#define I2C1_ER_VECT_SHIFT 			4
#define I2C2_EV_VECT_SHIFT 			12
#define I2C2_ER_VECT_SHIFT 			20
#define SPI1_VECT_SHIFT 			28
#define SPI2_VECT_SHIFT 			4
#define USART1_VECT_SHIFT 			12
#define USART2_VECT_SHIFT 			20
#define USART3_VECT_SHIFT 			28
#define EXTI15_10_VECT_SHIFT 		4
#define RTCALARM_VECT_SHIFT 		12
#define USBWAKEUP_VECT_SHIFT 		20
#define TIM8_BRK_VECT_SHIFT 		28
#define TIM8_UP_VECT_SHIFT 			4
#define TIM8_TRG_COM_VECT_SHIFT 	12
#define TIM8_CC_VECT_SHIFT 			20
#define ADC3_VECT_SHIFT 			28
#define FSMC_VECT_SHIFT 			4
#define SDIO_VECT_SHIFT 			12
#define TIM5_VECT_SHIFT 			20
#define SPI3_VECT_SHIFT 			28
#define UART4_VECT_SHIFT 			4
#define UART5_VECT_SHIFT 			12
#define TIM6_VECT_SHIFT 			20
#define TIM7_VECT_SHIFT 			28
#define DMA2_CHANNEL1_VECT_SHIFT 	4
#define DMA2_CHANNEL2_VECT_SHIFT 	12
#define DMA2_CHANNEL3_VECT_SHIFT 	20
#define DMA2_CHANNEL4_5_VECT_SHIFT 	28

#define NVIC_SET_PRIO_PERIPH(vector,prio) NVIC->IPR[vector##_VECT_INDEX] = NVIC->IPR[vector##_VECT_INDEX] & ~((u32)(0xF << vector##_VECT_SHIFT));\
								   	      NVIC->IPR[vector##_VECT_INDEX] = NVIC->IPR[vector##_VECT_INDEX] | (prio << vector##_VECT_SHIFT)


/* System interrupts and exception priority setting */
#define MEM_FAULT_VECT_INDEX		0
#define BUS_FAULT_VECT_INDEX		0
#define USAGE_FAULT_VECT_INDEX		0
#define RES1_FAULT_VECT_INDEX		0
#define RES2_FAULT_VECT_INDEX		1
#define RES3_FAULT_VECT_INDEX		1
#define RES4_FAULT_VECT_INDEX		1
#define SVCALL_VECT_INDEX			1
#define DEBUG_VECT_INDEX			2
#define RES5_FAULT_VECT_INDEX		2
#define PENDSV_VECT_INDEX			2
#define SYSTICK_VECT_INDEX			2

#define MEM_FAULT_VECT_SHIFT		4
#define BUS_FAULT_VECT_SHIFT		12
#define USAGE_FAULT_VECT_SHIFT		20
#define RES1_FAULT_VECT_SHIFT		28
#define RES2_FAULT_VECT_SHIFT		4
#define RES3_FAULT_VECT_SHIFT		12
#define RES4_FAULT_VECT_SHIFT		20
#define SVCALL_VECT_SHIFT			28
#define DEBUG_VECT_SHIFT			4
#define RES5_FAULT_VECT_SHIFT		12
#define PENDSV_VECT_SHIFT			20
#define SYSTICK_VECT_SHIFT			28

#define NVIC_SET_PRIO_SYSTEM(vector,prio) NVIC->SHPR[vector##_VECT_INDEX] = NVIC->SHPR[vector##_VECT_INDEX] & ~((u32)(0xF << vector##_VECT_SHIFT));\
								          NVIC->SHPR[vector##_VECT_INDEX] = NVIC->SHPR[vector##_VECT_INDEX] | (prio << vector##_VECT_SHIFT)

/* Peripherals interrupts enable/disable/clear and set pending */
#define WWDG_BIT_INDEX 				0
#define PVD_BIT_INDEX 				0
#define TAMPER_BIT_INDEX 			0
#define RTC_BIT_INDEX 				0
#define FLASH_BIT_INDEX 			0
#define RCC_BIT_INDEX 				0
#define EXTI0_BIT_INDEX 			0
#define EXTI1_BIT_INDEX 			0
#define EXTI2_BIT_INDEX 			0
#define EXTI3_BIT_INDEX 			0
#define EXTI4_BIT_INDEX 			0
#define DMA1_CHANNEL1_BIT_INDEX 	0
#define DMA1_CHANNEL2_BIT_INDEX 	0
#define DMA1_CHANNEL3_BIT_INDEX 	0
#define DMA1_CHANNEL4_BIT_INDEX 	0
#define DMA1_CHANNEL5_BIT_INDEX 	0
#define DMA1_CHANNEL6_BIT_INDEX 	0
#define DMA1_CHANNEL7_BIT_INDEX 	0
#define ADC1_2_BIT_INDEX 			0
#define USB_HP_CAN_TX_BIT_INDEX 	0
#define USB_LP_CAN_RX0_BIT_INDEX 	0
#define CAN_RX1_BIT_INDEX 			0
#define CAN_SCE_BIT_INDEX 			0
#define EXTI9_5_BIT_INDEX 			0
#define TIM1_BRK_BIT_INDEX 			0
#define TIM1_UP_BIT_INDEX 			0
#define TIM1_TRG_COM_BIT_INDEX 		0
#define TIM1_CC_BIT_INDEX 			0
#define TIM2_BIT_INDEX 				0
#define TIM3_BIT_INDEX 				0
#define TIM4_BIT_INDEX 				0
#define I2C1_EV_BIT_INDEX 			0

#define I2C1_ER_BIT_INDEX 			1
#define I2C2_EV_BIT_INDEX 			1
#define I2C2_ER_BIT_INDEX 			1
#define SPI1_BIT_INDEX 				1
#define SPI2_BIT_INDEX 				1
#define USART1_BIT_INDEX 			1
#define USART2_BIT_INDEX 			1
#define USART3_BIT_INDEX 			1
#define EXTI15_10_BIT_INDEX 		1
#define RTCALARM_BIT_INDEX 			1
#define USBWAKEUP_BIT_INDEX 		1
#define TIM8_BRK_BIT_INDEX 			1
#define TIM8_UP_BIT_INDEX 			1
#define TIM8_TRG_COM_BIT_INDEX 		1
#define TIM8_CC_BIT_INDEX 			1
#define ADC3_BIT_INDEX 				1
#define FSMC_BIT_INDEX 				1
#define SDIO_BIT_INDEX 				1
#define TIM5_BIT_INDEX 				1
#define SPI3_BIT_INDEX 				1
#define UART4_BIT_INDEX 			1
#define UART5_BIT_INDEX 			1
#define TIM6_BIT_INDEX 				1
#define TIM7_BIT_INDEX 				1
#define DMA2_CHANNEL1_BIT_INDEX 	1
#define DMA2_CHANNEL2_BIT_INDEX 	1
#define DMA2_CHANNEL3_BIT_INDEX 	1
#define DMA2_CHANNEL4_5_BIT_INDEX 	1

#define WWDG_BIT_SHIFT 				0
#define PVD_BIT_SHIFT 				1
#define TAMPER_BIT_SHIFT 			2
#define RTC_BIT_SHIFT 				3
#define FLASH_BIT_SHIFT 			4
#define RCC_BIT_SHIFT 				5
#define EXTI0_BIT_SHIFT 			6
#define EXTI1_BIT_SHIFT 			7
#define EXTI2_BIT_SHIFT 			8
#define EXTI3_BIT_SHIFT 			9
#define EXTI4_BIT_SHIFT 			10
#define DMA1_CHANNEL1_BIT_SHIFT 	11
#define DMA1_CHANNEL2_BIT_SHIFT 	12
#define DMA1_CHANNEL3_BIT_SHIFT 	13
#define DMA1_CHANNEL4_BIT_SHIFT 	14
#define DMA1_CHANNEL5_BIT_SHIFT 	15
#define DMA1_CHANNEL6_BIT_SHIFT 	16
#define DMA1_CHANNEL7_BIT_SHIFT 	17
#define ADC1_2_BIT_SHIFT 			18
#define USB_HP_CAN_TX_BIT_SHIFT 	19
#define USB_LP_CAN_RX0_BIT_SHIFT 	20
#define CAN_RX1_BIT_SHIFT 			21
#define CAN_SCE_BIT_SHIFT 			22
#define EXTI9_5_BIT_SHIFT 			23
#define TIM1_BRK_BIT_SHIFT 			24
#define TIM1_UP_BIT_SHIFT 			25
#define TIM1_TRG_COM_BIT_SHIFT 		26
#define TIM1_CC_BIT_SHIFT 			27
#define TIM2_BIT_SHIFT 				28
#define TIM3_BIT_SHIFT 				29
#define TIM4_BIT_SHIFT 				30
#define I2C1_EV_BIT_SHIFT 			31

#define I2C1_ER_BIT_SHIFT 			0
#define I2C2_EV_BIT_SHIFT 			1
#define I2C2_ER_BIT_SHIFT 			2
#define SPI1_BIT_SHIFT 				3
#define SPI2_BIT_SHIFT 				4
#define USART1_BIT_SHIFT 			5
#define USART2_BIT_SHIFT 			6
#define USART3_BIT_SHIFT 			7
#define EXTI15_10_BIT_SHIFT 		8
#define RTCALARM_BIT_SHIFT 			9
#define USBWAKEUP_BIT_SHIFT 		10
#define TIM8_BRK_BIT_SHIFT 			11
#define TIM8_UP_BIT_SHIFT 			12
#define TIM8_TRG_COM_BIT_SHIFT 		13
#define TIM8_CC_BIT_SHIFT 			14
#define ADC3_BIT_SHIFT 				15
#define FSMC_BIT_SHIFT 				16
#define SDIO_BIT_SHIFT 				17
#define TIM5_BIT_SHIFT 				18
#define SPI3_BIT_SHIFT 				19
#define UART4_BIT_SHIFT 			20
#define UART5_BIT_SHIFT 			21
#define TIM6_BIT_SHIFT 				22
#define TIM7_BIT_SHIFT 				23
#define DMA2_CHANNEL1_BIT_SHIFT 	24
#define DMA2_CHANNEL2_BIT_SHIFT 	25
#define DMA2_CHANNEL3_BIT_SHIFT 	26
#define DMA2_CHANNEL4_5_BIT_SHIFT 	27

#define NVIC_ENABLE_PERIPH_IT(vector) NVIC->ISER[vector##_BIT_INDEX] = NVIC->ISER[vector##_BIT_INDEX] | ((u32)(1 << vector##_BIT_SHIFT))
#define NVIC_DISABLE_PERIPH_IT(vector) NVIC->ICER[vector##_BIT_INDEX] = NVIC->ICER[vector##_BIT_INDEX] | ((u32)(1 << vector##_BIT_SHIFT))
#define NVIC_SET_PENDING_PERIPH_IT(vector) NVIC->ISPR[vector##_BIT_INDEX] = NVIC->ISPR[vector##_BIT_INDEX] | ((u32)(1 << vector##_BIT_SHIFT))
#define NVIC_CLEAR_PENDING_PERIPH_IT(vector) NVIC->ICPR[vector##_BIT_INDEX] = NVIC->ICPR[vector##_BIT_INDEX] | ((u32)(1 << vector##_BIT_SHIFT))

#define NVIC_IS_PENDING_PERIPH_IT(vector) (NVIC->ICPR[vector##_BIT_INDEX] & ((u32)(1 << vector##_BIT_SHIFT)))
#define NVIC_IS_ENABLED_PERIPH_IT(vector) (NVIC->ICER[vector##_BIT_INDEX] & ((u32)(1 << vector##_BIT_SHIFT)))

/* Generate SW interrupt */
#define NVIC_RAISE_SW_INTERRUPT(interrupt) (NVIC_STIR=interrupt)

void NVIC_INIT(void);

#endif /*  __STM_NVIC_H__ */