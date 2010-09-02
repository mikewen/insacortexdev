#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#define __STM32F10x_TYPE_H
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

#include "stm_regs.h"
/*
 * ISR2 $EXEC_NAME$ stacks
 *
 * System stack
 */

tpl_stack_word  $STACK_ZONE$[$STACK_SIZE$/sizeof(tpl_stack_word)];


#define $EXEC_STACK$ { $STACK_ZONE$, $STACK_SIZE$}

/* 
 * ISR2 $TRAP_NUMBER$_IRQHandler that calls the central handler
 */


void $EXEC_NAME$_IRQHandler(void)
{
	tpl_central_interrupt_handler($EXEC_NAME$_id);

}


/*
 * ISR2 $EXEC_NAME$ context
 */
cm3_context $EXEC_INTEGER_CONTEXT$;

#define $EXEC_CONTEXT$ { &$EXEC_INTEGER_CONTEXT$ }
