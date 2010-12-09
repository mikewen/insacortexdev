/*
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 *
 * Based on work for Cortex-M3 port
 * Modified by DI MERCURIO
 */

#ifndef __TPL_MACHINE_AVR32A_H__
#define __TPL_MACHINE_AVR32A_H__

#include "tpl_os_custom_types.h"
#include "port_macros.h"


/**
 * tpl_stack_word is the basic element of a stack in order to have a proper
 * data alignement.
 */
typedef u32 tpl_stack_word;
/**
 * tpl_stack_size is the data type used to store the stack size
 */
typedef u16 tpl_stack_size; //limit stack to 64kb



// Store context in same order as stack frame after interrupt
// plus space for R4 to R11 gprs

struct AVR32A_CONTEXT {
	u32 r0_7[8] ; //grps registers not in stack frame
	tpl_stack_word *sp;		// SP stack pointer of task
							// point to stak frame after interrupt
							// R8 R9 R10 R11 R12 LR PC SR
							// Warning: After an exception or NMI, only PC and SR registers are pushed onto stack

};

typedef struct AVR32A_CONTEXT avr32a_context;


struct TPL_CONTEXT {
	avr32a_context * ic;
};

typedef struct TPL_CONTEXT tpl_context;


extern avr32a_context idle_task_context;
#define IDLE_CONTEXT {&idle_task_context}


struct TPL_STACK {
    tpl_stack_word 		 *stack_zone;
    tpl_stack_size        stack_size; 
};
typedef struct TPL_STACK tpl_stack;

/**
 * @def IDLE_ENTRY
 *
 * Entry point of the idle task.
 */
#define IDLE_ENTRY tpl_sleep

/**
 * @def SIZE_OF_IDLE_TASK
 *
 * The size of the stack of the idle task which is also the stack
 * used for machine startup
 */
#define SIZE_OF_IDLE_STACK  64
/**
 * @def IDLE_STACK
 *
 * the idle stack definition
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
extern VAR(tpl_stack_word, OS_VAR)
idle_stack[SIZE_OF_IDLE_STACK/sizeof(tpl_stack_word)];
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#define IDLE_STACK { idle_stack, SIZE_OF_IDLE_STACK }

#endif /* __TPL_MACHINE_AVR32A_H__ */
