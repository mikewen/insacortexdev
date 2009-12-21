#include "tpl_os_internal_types.h"
#include "tpl_machine.h"
#include "tpl_os_it.h"
#include "tpl_os_it_kernel.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_os_alarm.h"
#include "tpl_os_rez_kernel.h"
#include "tpl_os_rez.h"
#include "tpl_os_event_kernel.h"
#include "tpl_os_event.h"
#include "tpl_os_action.h"
#include "tpl_os_kernel.h"
#include "tpl_os_definitions.h"

/*#ifndef WITH_AUTOSAR*/
#ifndef WITH_NOCOM
#include "tpl_com_notification.h"
#include "tpl_com_mo.h"
#include "tpl_com_internal.h"
#include "tpl_com_internal_com.h"
#include "tpl_com_external_com.h"
#include "tpl_com_app_copy.h"
#include "tpl_com_filters.h"
#endif
/*#endif*/

#include "tpl_os_generated_configuration.h"
#include "tpl_app_objects.h"

#ifdef WITH_AUTOSAR
#include "tpl_as_schedtable.h"
#include "tpl_as_st_kernel.h"
#include "tpl_as_action.h"
#include "tpl_as_isr_kernel.h"
#include "tpl_as_isr.h"
#include "tpl_as_counter.h"
#include "tpl_as_app_kernel.h"
#include "tpl_as_trusted_fct.h"
#include "tpl_as_trusted_fct_kernel.h"
#include "tpl_as_application.h"
#endif

/* #include "Application_types.h" */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*=============================================================================
 * Declaration of event masks
 */


/*=============================================================================
 * Declaration of Resource IDs
 */
#define res_id 0
CONST(ResourceType, AUTOMATIC) res = res_id;


/*=============================================================================
 * Declaration of Task IDs
 */
#define Tache1_id  0
CONST(TaskType, AUTOMATIC) Tache1 = Tache1_id;
#define Tache2_id  1
CONST(TaskType, AUTOMATIC) Tache2 = Tache2_id;


/*=============================================================================
 * Declaration of Alarm IDs
 */
#define Active_Tache1_id 0
CONST(AlarmType, AUTOMATIC) Active_Tache1 = Active_Tache1_id;
#define Active_Tache2_id 1
CONST(AlarmType, AUTOMATIC) Active_Tache2 = Active_Tache2_id;


/*=============================================================================
 * Declaration of Counter IDs
 */
#ifdef WITH_AUTOSAR

#endif

/*=============================================================================
 * Declaration of messages IDs
 */




/*=============================================================================
 * Declaration of Scheduletables IDs
 */
#ifdef WITH_AUTOSAR
$SCHEDTABLEIDS$
#endif

/*=============================================================================
 * Declaration of OS Application IDs
 */
#ifdef WITH_AUTOSAR
$OSAPPIDS$

$TRUSTED_IDS$
#endif

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of Resource related structures
 */

/*
 * Resource descriptor of resource res
 *
 * Tasks which use this resource :
 * $TASKS$
 *
 * ISRs which use this resource :
 * $ISRS$
 */
VAR(tpl_resource, OS_VAR) res_rez_desc = {
  /* ceiling priority of the resource */  (tpl_priority)1,
  /* owner previous priority          */  (tpl_priority)0,
  /* owner of the resource            */  -1,
#ifdef WITH_OSAPPLICATION
  /* OS Application id                */  $APP_ID$,
#endif    
  /* next resource in the list        */  NULL
};


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_resource, AUTOMATIC, OS_APPL_DATA)
  tpl_resource_table[RESOURCE_COUNT] = {
  &res_rez_desc,
  &res_sched_rez_desc
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"



/*=============================================================================
 * Definition and initialization of Counters related defines and structures
 */

/*
 * counter General_counter descriptor
 */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONST(tpl_tick, OS_CONST) OSTICKSPERBASE_General_counter = 1;
CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE_General_counter = 65535;
CONST(tpl_tick, OS_CONST) OSMINCYCLE_General_counter = 1;
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_counter, OS_VAR) General_counter_counter_desc = {
    /* ticks per base       */  1,
    /* max allowed value    */  65535,
    /* minimum cycle        */  1,
    /* current tick         */  0,
    /* current date         */  0,
#ifdef WITH_AUTOSAR
    /* kind the counter     */  $COUNTER_KIND$,
#endif
#ifdef WITH_OSAPPLICATION
    /* OS application id    */  $APP_ID$,
#endif
    /* first alarm          */  NULL_PTR,
    /* next alarm to raise  */  NULL_PTR
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * counter SystemCounter descriptor
 */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONST(tpl_tick, OS_CONST) OSTICKSPERBASE = 1;
CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE = 32767;
CONST(tpl_tick, OS_CONST) OSMINCYCLE = 1;
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_counter, OS_VAR) SystemCounter_counter_desc = {
    /* ticks per base       */  1,
    /* max allowed value    */  32767,
    /* minimum cycle        */  1,
    /* current tick         */  0,
    /* current date         */  0,
#ifdef WITH_AUTOSAR
    /* kind the counter     */  $COUNTER_KIND$,
#endif
#ifdef WITH_OSAPPLICATION
    /* OS application id    */  $APP_ID$,
#endif
    /* first alarm          */  NULL_PTR,
    /* next alarm to raise  */  NULL_PTR
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

//_____________________________________________
// counter_specific for CM3
// widely inspired from free rtos portmacro.h
//_____________________________________________


/* For backward compatibility, ensure configKERNEL_INTERRUPT_PRIORITY is
defined.  The value should also ensure backward compatibility.
FreeRTOS.org versions prior to V4.4.0 did not include this definition. */
#ifndef configKERNEL_INTERRUPT_PRIORITY
	#define configKERNEL_INTERRUPT_PRIORITY 255
#endif

/* Constants required to manipulate the NVIC. */
#define portNVIC_SYSTICK_CTRL		( ( volatile unsigned portLONG *) 0xe000e010 )
#define portNVIC_SYSTICK_LOAD		( ( volatile unsigned portLONG *) 0xe000e014 )
#define portNVIC_INT_CTRL			( ( volatile unsigned portLONG *) 0xe000ed04 )
#define portNVIC_SYSPRI2			( ( volatile unsigned portLONG *) 0xe000ed20 )
#define portNVIC_SYSTICK_CLK		0x00000004
#define portNVIC_SYSTICK_INT		0x00000002
#define portNVIC_SYSTICK_ENABLE		0x00000001
#define portNVIC_PENDSVSET			0x10000000
#define portNVIC_PENDSV_PRI			( ( ( unsigned portLONG ) configKERNEL_INTERRUPT_PRIORITY ) << 16 )
#define portNVIC_SYSTICK_PRI		( ( ( unsigned portLONG ) configKERNEL_INTERRUPT_PRIORITY ) << 24 )


/* The priority used by the kernel is assigned to a variable to make access
from inline assembler easier. */
const unsigned portLONG ulKernelPriority = configKERNEL_INTERRUPT_PRIORITY;

/*
 * Exception handler.
 */
void SysTick_Handler( void );

void tpl_init_tick_timer()
{
	/* Configure SysTick to interrupt at the requested rate. */
	*(portNVIC_SYSTICK_LOAD) = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
	*(portNVIC_SYSTICK_CTRL) = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;

}


void tpl_call_counter_tick()
{
  tpl_status  need_rescheduling = NO_SPECIAL_CODE;
    need_rescheduling |= tpl_counter_tick(&General_counter_counter_desc);
    need_rescheduling |= tpl_counter_tick(&SystemCounter_counter_desc);


  if (need_rescheduling == NEED_RESCHEDULING)
  {
    tpl_schedule_from_running();
#ifndef WITH_SYSTEM_CALL
    if (tpl_kern.need_switch != NO_NEED_SWITCH) {
      tpl_switch_context_from_it(
        &(tpl_kern.s_old->context),
        &(tpl_kern.s_running->context)
      );
    }
#endif
 }
}

// Timer system tick
//___________________

void SysTick_Handler( void )
{

	portDISABLE_INTERRUPTS();	

	tpl_call_counter_tick();

	portENABLE_INTERRUPTS();	

}
//_______________End of Counter specific code___________________________


/*=============================================================================
 * Definition and initialization of Task related defines and structures
 */

/* ____________________________________________________________________
 * Tache1 stacks
 *
 * System stack
 */
tpl_stack_word Tache1_stack[256/sizeof(tpl_stack_word)];


#define Tache1_STACK { Tache1_stack, 256 }

/*
 * Tache1 context
 */
cm3_context Tache1_int_context;

#define Tache1_CONTEXT { &Tache1_int_context }

#define APP_Task_Tache1_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task Tache1 function prototype
 */
FUNC(void, OS_APPL_CODE) Tache1_function(void);
#define APP_Task_Tache1_STOP_SEC_CODE
#include "tpl_memmap.h"


#ifdef WITH_AUTOSAR_TIMING_PROTECTION
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Static descriptor of task Tache1
 */
CONST(tpl_proc_static, OS_CONST) Tache1_task_stat_desc = {
    /* context                  */  Tache1_CONTEXT,
    /* stack                    */  Tache1_STACK,
    /* entry point (function)   */  Tache1_function,
    /* internal ressource       */  NULL,
    /* task id                  */  Tache1_id,
#ifdef WITH_OSAPPLICATION
    /* OS application id        */  $APP_ID$,
#endif
    /* task base priority       */  1,
    /* max activation count     */  1,
    /* task type                */  TASK_BASIC,
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* pointer to the timing
       protection descriptor    */  NULL
#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task Tache1
 */
VAR(tpl_proc, OS_VAR) Tache1_task_desc = {
    /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
    /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
    /* activate count                 */  0,
    /* task priority                  */  (tpl_priority)1,
    /* task state                     */  SUSPENDED
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* activation allowed             */  ,TRUE
#endif
};



#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/* ____________________________________________________________________
 * Tache2 stacks
 *
 * System stack
 */
tpl_stack_word Tache2_stack[256/sizeof(tpl_stack_word)];


#define Tache2_STACK { Tache2_stack, 256 }

/*
 * Tache2 context
 */
cm3_context Tache2_int_context;

#define Tache2_CONTEXT { &Tache2_int_context }

#define APP_Task_Tache2_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task Tache2 function prototype
 */
FUNC(void, OS_APPL_CODE) Tache2_function(void);
#define APP_Task_Tache2_STOP_SEC_CODE
#include "tpl_memmap.h"


#ifdef WITH_AUTOSAR_TIMING_PROTECTION
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Static descriptor of task Tache2
 */
CONST(tpl_proc_static, OS_CONST) Tache2_task_stat_desc = {
    /* context                  */  Tache2_CONTEXT,
    /* stack                    */  Tache2_STACK,
    /* entry point (function)   */  Tache2_function,
    /* internal ressource       */  NULL,
    /* task id                  */  Tache2_id,
#ifdef WITH_OSAPPLICATION
    /* OS application id        */  $APP_ID$,
#endif
    /* task base priority       */  2,
    /* max activation count     */  1,
    /* task type                */  TASK_BASIC,
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* pointer to the timing
       protection descriptor    */  NULL
#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task Tache2
 */
VAR(tpl_proc, OS_VAR) Tache2_task_desc = {
    /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
    /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
    /* activate count                 */  0,
    /* task priority                  */  (tpl_priority)2,
    /* task state                     */  SUSPENDED
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* activation allowed             */  ,TRUE
#endif
};



#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of ISR2 related defines and structures
 */
void initInterrupt()
{

}


#if WITH_IT_TABLE == YES
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
$IT_TABLE$
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*=============================================================================
 * Definition and initialization of process tables (tasks and isrs)
 */
CONSTP2CONST(tpl_proc_static, AUTOMATIC, OS_APPL_DATA)
tpl_stat_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &Tache1_task_stat_desc,
  &Tache2_task_stat_desc,
  &idle_task_static
};


CONSTP2VAR(tpl_proc, AUTOMATIC, OS_APPL_DATA)
tpl_dyn_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &Tache1_task_desc,
  &Tache2_task_desc,
  &idle_task
};


#if EXTENDED_TASK_COUNT > 0
CONSTP2VAR(tpl_task_events, AUTOMATIC, OS_APPL_DATA)
tpl_task_events_table[EXTENDED_TASK_COUNT] = {

};
#endif

#if ISR_COUNT > 0


#ifdef WITH_AUTOSAR

#endif

#endif

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of Alarm related structures
 */

/*
 * Alarm descriptor of alarm Active_Tache1
 */
tpl_task_activation_action task_act_of_Active_Tache1 = {
    {
        /* action function  */  tpl_action_activate_task
    },
    /* task id              */  Tache1_id
};

tpl_alarm_static stat_Active_Tache1_alarm_desc = {
  {
    /* pointer to counter           */  &General_counter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , Active_Tache1_id
#endif
#ifdef WITH_OSAPPLICATION
    /* OS application id            */  , $APP_ID$
#endif
  },
  /* action of the alarm  */  (tpl_action *)&task_act_of_Active_Tache1
};

tpl_time_obj Active_Tache1_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&stat_Active_Tache1_alarm_desc,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  10,
    /* date                         */  1,
    /* State of the alarm           */  ALARM_AUTOSTART
};

/*
 * Alarm descriptor of alarm Active_Tache2
 */
tpl_task_activation_action task_act_of_Active_Tache2 = {
    {
        /* action function  */  tpl_action_activate_task
    },
    /* task id              */  Tache2_id
};

tpl_alarm_static stat_Active_Tache2_alarm_desc = {
  {
    /* pointer to counter           */  &General_counter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , Active_Tache2_id
#endif
#ifdef WITH_OSAPPLICATION
    /* OS application id            */  , $APP_ID$
#endif
  },
  /* action of the alarm  */  (tpl_action *)&task_act_of_Active_Tache2
};

tpl_time_obj Active_Tache2_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&stat_Active_Tache2_alarm_desc,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  2,
    /* date                         */  1,
    /* State of the alarm           */  ALARM_AUTOSTART
};

CONSTP2VAR(tpl_time_obj, AUTOMATIC, OS_APPL_DATA)
  tpl_alarm_table[ALARM_COUNT] = {
  &Active_Tache1_alarm_desc,
  &Active_Tache2_alarm_desc
};


/*=============================================================================
 * Declaration of flags functions
 */


/*=============================================================================
 * Definition and initialization of Messages related structures
 */





#ifdef WITH_AUTOSAR
/*=============================================================================
 * Declaration of schedule tables related defines and structures
 */
$SCHEDULETABLES$

/*=============================================================================
 * Declaration of OS applications related defines and structures
 */
$OSAPPLICATIONS$

/*=============================================================================
 * Declaration of Trusted Functions table
 */
$TRUSTED_FCTS$

$TRUSTED_TABLE$
#endif

#if WITH_MEMORY_PROTECTION == YES
$MEMORY_PROTECTION$
#endif

/*=============================================================================
 * Definition and initialization of Ready List structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
VAR(tpl_proc_id, OS_VAR) tpl_priority_0_fifo[1];
VAR(tpl_proc_id, OS_VAR) tpl_priority_1_fifo[1];
VAR(tpl_proc_id, OS_VAR) tpl_priority_2_fifo[2];

VAR(tpl_fifo_state, OS_VAR) tpl_fifo_rw[3] = {
    { 0 , 0 },
    { 0 , 0 },
    { 0 , 0 }
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONST(tpl_priority_level, OS_CONST) tpl_ready_list[3] = {
    { tpl_priority_0_fifo , 1 },
    { tpl_priority_1_fifo , 1 },
    { tpl_priority_2_fifo , 2 }
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#if !defined(NO_TASK) || !defined(NO_ISR)
#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2CONST(char, AUTOMATIC, OS_APPL_DATA) proc_name_table[TASK_COUNT + ISR_COUNT] = {
  "Tache1",
  "Tache2",
};
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
#endif


/*
#if CRC_LEVEL > 0
$CRC_TABLE$
#endif
*/

/* End of file tpl_os_generated_configuration.c */
