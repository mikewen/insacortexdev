/**
 * @file tpl_machine_interface.h
 *
 * @section descr File description
 *
 * Trampoline machine dependant functions interface
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date: 2009-05-15 12:01:22 +0200 (Ven, 15 mai 2009) $
 * $Rev: 826 $
 * $Author: jlb $
 * $URL: https://trampoline.rts-software.org/svn/trunk/os/tpl_machine_interface.h $
 */

#ifndef TPL_MACHINE_INTERFACE_H
#define TPL_MACHINE_INTERFACE_H

#include "tpl_os_kernel.h"
#include "tpl_os_internal_types.h"
#include "tpl_os_custom_types.h"
#include "tpl_os_application_def.h"

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

/**
 * @internal
 *
 * tpl_switch_context switches the context in a cooperative way
 *
 * @param old_context   Pointer to the context of the task that will be
 *                      descheduled.
 * @param new_context   Pointer to the context of the task that will be
 *                      scheduled.
 */
extern FUNC(void, OS_CODE) tpl_switch_context(
  CONSTP2CONST(tpl_context, AUTOMATIC, OS_CONST) old_context,
  CONSTP2CONST(tpl_context, AUTOMATIC, OS_CONST) new_context
);


/**
 * @internal
 *
 * tpl_switch_context_from_it switches the context under interrupt,
 * ie in a preemptive way.
 *
 * @param old_context   Pointer to the context of the task that will be
 *                      descheduled.
 * @param new_context   Pointer to the context of the task that will be
 *                      scheduled.
 */
extern FUNC(void, OS_CODE) tpl_switch_context_from_it(
  CONSTP2CONST(tpl_context, AUTOMATIC, OS_CONST) old_context,
  CONSTP2CONST(tpl_context, AUTOMATIC, OS_CONST) new_context
);


/**
 * @internal
 *
 * tpl_init_context initialized the context of a task that is being activated
 *
 * @param exec_obj      Pointer to the task descriptor
 */
extern FUNC(void, OS_CODE) tpl_init_context(
  CONST(tpl_proc_id, AUTOMATIC) proc_id);


/**
 * @internal
 *
 * tpl_disable_interrupts disable all interrupts
 */
extern FUNC(void, OS_CODE) tpl_disable_interrupts(void);

/**
 * @internal
 *
 * tpl_enable_interrupts disable all interrupts
 */
extern FUNC(void, OS_CODE) tpl_enable_interrupts(void);

/**
 * @internal
 *
 * tpl_get_task_lock locks the kernel
 */
extern FUNC(void, OS_CODE) tpl_get_task_lock(void);


/**
 * @internal
 *
 * tpl_release_task_lock unlocks the kernel
 */
extern FUNC(void, OS_CODE) tpl_release_task_lock(void);


/**
 * @internal
 *
 * tpl_init_machine performs machine (hardware of virtual machine)
 * dependant initializations
 */
extern FUNC(void, OS_CODE) tpl_init_machine(void);


/**
 * @internal
 *
 * tpl_sleep is called by the idle task. It should put the system in sleep
 * mode of go in an infinite loop if there is no sleep mode available.
 */
extern FUNC(void, OS_CODE) tpl_sleep(void);


/**
 * @internal
 *
 * tpl_shutdown shutdown trampoline. It is called by ShutdownOS and should
 * perform hardware or virtual machine shutdown.
 */
extern FUNC(void, OS_CODE) tpl_shutdown(void);

#ifdef WITH_AUTOSAR
/**
 * @internal
 *
 * tpl_get_interrupt_lock_status checks if the user has released any call to
 * DisableAllInterrupt/SuspendAllInterrupt/SuspendOsInterrupt
 *
 */
extern FUNC(tpl_bool, OS_CODE) tpl_get_interrupt_lock_status(void);

/**
 * @internal
 *
 * tpl_reset_interrupt_lock_status reset the status of interrupt lock by user
 *
 */
extern FUNC(void, OS_CODE) tpl_reset_interrupt_lock_status(void);

#endif

#ifdef WITH_AUTOSAR_TIMING_PROTECTION
/**
 * @internal
 *
 * Start the watchdog to call function when delay expires. The watchdog
 * can be canceled before expiration
 *
 * @param delay time (in tpl_time unit) since now the expire function will be
 * called (cannot be zero)
 *
 * @see #tpl_cancel_watchdog
 * @see #tpl_get_local_current_date
 */
extern FUNC(void, OS_CODE) tpl_set_watchdog(
    VAR(tpl_time, AUTOMATIC) delay
);

/**
 * @internal
 *
 * Cancels the watchdog before its expiration. This has no effet if the
 * watchdog has not been started before.
 *
 * @see #tpl_set_watchdog
 */
extern FUNC(void, OS_CODE) tpl_cancel_watchdog(void);

/**
 * @internal
 *
 * Gives the current date in tpl_time unit. See the os machine specifications
 * to know what is the unit of tpl_time.
 *
 * @return the current date when called
 */
extern FUNC(tpl_time, OS_CODE) tpl_get_local_current_date(void);
#endif /* WITH_AUTOSAR_TIMING_PROTECTION */

#ifdef WITH_AUTOSAR_STACK_MONITORING
/**
 * @internal
 *
 * This function tells if the saved stack pointer (into saved context) is
 * located into the stack boundaries or not.
 *
 * Depending on what the hardware is able to do, this function may not be
 * able to detect an error. Only undoubted errors are reported.
 *
 * @param proc_id is the id of the process (task or ISR) to check
 *
 * @retval 1 no stack overflow detected
 * @retval 0 stack overflow deteted
 *
 * @see #tpl_check_stack_footprint
 */
FUNC(tpl_bool, OS_CODE) tpl_check_stack_pointer(
  CONST(tpl_proc_id, AUTOMATIC) proc_id);

/**
 * @internal
 *
 * This functions checks the stack did not overflowed by looking if the stack
 * tagging has not been completly erased.
 *
 * @param proc_id is the id of the process (task or ISR) to check
 *
 * @note This function needs a stack tagging which should be done at system
 * initialization (#tpl_init_machine). The stack tagging consists in filling
 * stack with known unique and unusual values (e.g. not zero). When the stack
 * will be used, these values will be overriden and we will be able to know
 * how high the stack have been. Of course, it is intended for debug stage,
 * then we suggest you oversize the stack.
 *
 * @retval 1 no stack overflow detected
 * @retval 0 stack overflow deteted
 */
FUNC(u8, OS_CODE) tpl_check_stack_footprint(
  CONST(tpl_proc_id, AUTOMATIC) proc_id);
#endif /* WITH_AUTOSAR_STACK_MONITORING */

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

#endif /* TPL_MACHINE_INTERFACE_H */
