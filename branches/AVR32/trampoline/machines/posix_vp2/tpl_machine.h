
#ifndef TPL_MACHINE_H
#define TPL_MACHINE_H

#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

#include "tpl_os_custom_types.h"
#include "viper.h"

/**
 * tpl_stack_word is the basic element of a stack in order to have a proper
 * data alignement.
 */
typedef u32 tpl_stack_word;

/**
 * tpl_stack_size is the data type used to store the stack size
 */
typedef u16 tpl_stack_size;

struct TPL_STACK {
    tpl_stack_word      *stack_zone;
    tpl_stack_size      stack_size; 
};
typedef struct TPL_STACK *tpl_stack;
extern struct TPL_STACK idle_task_stack;

struct TPL_CONTEXT {
    jmp_buf initial;
    jmp_buf current;
};
typedef struct TPL_CONTEXT *tpl_context;
extern struct TPL_CONTEXT idle_task_context;

extern void tpl_sleep(void);

extern ipc_t viper;
extern global_ipc_t global_shared_memory;

#define IDLE_CONTEXT    &idle_task_context 
#define IDLE_ENTRY      tpl_sleep
#define IDLE_STACK      &idle_task_stack

#define TPL_IT_VECTOR_SIZE          32
#define TPL_IT_VECTOR_INDEX_OFFSET  0

#endif /* TPL_MACHINE_H */
