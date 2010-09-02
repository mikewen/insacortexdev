#include "stm_regs.h"

void Init_Periphs(void)
{
  //EXTI0 sur PA0 (bouton wakeup de MCB)
  EXTI->IMR |= 1;
  EXTI->RTSR |= 1;
  NVIC_ENABLE_PERIPH_IT(EXTI0); 
  NVIC_SET_PRIO_PERIPH(EXTI0,1);
}


