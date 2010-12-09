#include "../sys/stm_regs.h"
#define IS_ANALOG_INPUT 0x0
#define IS_FLOATING_INPUT 0x4
#define IS_PULLUP_PULLDOWN_INPUT 0x8
#define IS_GENERAL_OUTPUT_PUSHPULL_10MHZ 0x1
#define IS_GENERAL_OUTPUT_PUSHPULL_2MHZ 0x2
#define IS_GENERAL_OUTPUT_PUSHPULL_50MHZ 0x3
#define IS_GENERAL_OUTPUT_OPENDRAIN_10MHZ 0x5
#define IS_GENERAL_OUTPUT_OPENDRAIN_2MHZ 0x6
#define IS_GENERAL_OUTPUT_OPENDRAIN_50MHZ 0x7
#define IS_ALTERNATE_OUTPUT_PUSHPULL_10MHZ 0x9
#define IS_ALTERNATE_OUTPUT_PUSHPULL_2MHZ 0xA
#define IS_ALTERNATE_OUTPUT_PUSHPULL_50MHZ 0xB
#define IS_ALTERNATE_OUTPUT_OPENDRAIN_10MHZ 0xD
#define IS_ALTERNATE_OUTPUT_OPENDRAIN_2MHZ 0xE
#define IS_ALTERNATE_OUTPUT_OPENDRAIN_50MHZ 0xF

#define P_A_0	IS_FLOATING_INPUT
#define P_A_1	IS_FLOATING_INPUT
#define P_A_2	IS_FLOATING_INPUT
#define P_A_3	IS_FLOATING_INPUT
#define P_A_4	IS_FLOATING_INPUT
#define P_A_5	IS_FLOATING_INPUT
#define P_A_6	IS_FLOATING_INPUT
#define P_A_7	IS_FLOATING_INPUT
#define P_A_8	IS_FLOATING_INPUT
#define P_A_9	IS_FLOATING_INPUT
#define P_A_10	IS_FLOATING_INPUT
#define P_A_11	IS_FLOATING_INPUT
#define P_A_12	IS_FLOATING_INPUT
#define P_A_13	IS_FLOATING_INPUT
#define P_A_14	IS_FLOATING_INPUT
#define P_A_15	IS_FLOATING_INPUT

#define P_B_0	IS_FLOATING_INPUT
#define P_B_1	IS_FLOATING_INPUT
#define P_B_2	IS_FLOATING_INPUT
#define P_B_3	IS_FLOATING_INPUT
#define P_B_4	IS_FLOATING_INPUT
#define P_B_5	IS_FLOATING_INPUT
#define P_B_6	IS_FLOATING_INPUT
#define P_B_7	IS_FLOATING_INPUT
#define P_B_8	IS_FLOATING_INPUT
#define P_B_9	IS_FLOATING_INPUT
#define P_B_10	IS_FLOATING_INPUT
#define P_B_11	IS_FLOATING_INPUT
#define P_B_12	IS_FLOATING_INPUT
#define P_B_13	IS_FLOATING_INPUT
#define P_B_14	IS_FLOATING_INPUT
#define P_B_15	IS_FLOATING_INPUT

#define P_C_0	IS_FLOATING_INPUT
#define P_C_1	IS_FLOATING_INPUT
#define P_C_2	IS_FLOATING_INPUT
#define P_C_3	IS_FLOATING_INPUT
#define P_C_4	IS_FLOATING_INPUT
#define P_C_5	IS_FLOATING_INPUT
#define P_C_6	IS_FLOATING_INPUT
#define P_C_7	IS_FLOATING_INPUT
#define P_C_8	IS_FLOATING_INPUT
#define P_C_9	IS_FLOATING_INPUT
#define P_C_10	IS_FLOATING_INPUT
#define P_C_11	IS_FLOATING_INPUT
#define P_C_12	IS_FLOATING_INPUT
#define P_C_13	IS_FLOATING_INPUT
#define P_C_14	IS_FLOATING_INPUT
#define P_C_15	IS_FLOATING_INPUT

#include "GPIO_stat_conf.h"
// Check configuration ERRORS



#ifdef GPIOA_USED
	#define RCC_IOPAEN_MASK RCC_IOPAEN
	#define GPIOA_CRL_CONF (P_A_0 << 0) |(P_A_1<<4) |(P_A_2 << 8) |(P_A_3<<12)|\
						   (P_A_4 << 16)|(P_A_5<<20)|(P_A_6 << 24)|(P_A_7<<28)
	#define GPIOA_CRH_CONF (P_A_8 << 0) |(P_A_9<<4) |(P_A_10 << 8) |(P_A_11<<12)|\
						   (P_A_12 << 16)|(P_A_13<<20)|(P_A_14 << 24)|(P_A_15<<28)
#else
	#define RCC_IOPAEN_MASK 0
#endif

#ifdef GPIOB_USED
	#define RCC_IOPBEN_MASK RCC_IOPBEN
	#define GPIOB_CRL_CONF (P_B_0 << 0) |(P_B_1<<4) |(P_B_2 << 8) |(P_B_3<<12)|\
						   (P_B_4 << 16)|(P_B_5<<20)|(P_B_6 << 24)|(P_B_7<<28)
	#define GPIOB_CRH_CONF (P_B_8 << 0) |(P_B_9<<4) |(P_B_10 << 8) |(P_B_11<<12)|\
						   (P_B_12 << 16)|(P_B_13<<20)|(P_B_14 << 24)|(P_B_15<<28)
#else
	#define RCC_IOPBEN_MASK 0
#endif

#ifdef GPIOC_USED
	#define RCC_IOPCEN_MASK RCC_IOPCEN
	#define GPIOA_CRL_CONF (P_C_0 << 0) |(P_C_1<<4) |(P_C_2 << 8) |(P_C_3<<12)|\
						   (P_C_4 << 16)|(P_C_5<<20)|(P_C_6 << 24)|(P_C_7<<28)
	#define GPIOA_CRH_CONF (P_C_8 << 0) |(P_C_9<<4) |(P_C_10 << 8) |(P_C_11<<12)|\
						   (P_C_12 << 16)|(P_C_13<<20)|(P_C_14 << 24)|(P_C_15<<28)
#else
	#define RCC_IOPCEN_MASK 0
#endif

void Port_IO_Init(void)
{

#if (RCC_IOPAEN_MASK != 0)||(RCC_IOPBEN_MASK != 0)||(RCC_IOPCEN_MASK == 0)
	// Activer horloges A, B et C
	(RCC->APB2ENR)|= RCC_IOPAEN_MASK | RCC_IOPBEN_MASK | RCC_IOPCEN_MASK;
#endif

#ifdef GPIOA_USED
	GPIOA->CRL = (GPIOA_CRL_CONF);
	GPIOA->CRH = (GPIOA_CRH_CONF);
#endif
#ifdef GPIOB_USED
	GPIOB->CRL = (GPIOB_CRL_CONF);
	GPIOB->CRH = (GPIOB_CRH_CONF);
#endif
#ifdef GPIOC_USED
	GPIOC->CRL = (GPIOC_CRL_CONF);
	GPIOC->CRH = (GPIOC_CRH_CONF);
#endif

}
