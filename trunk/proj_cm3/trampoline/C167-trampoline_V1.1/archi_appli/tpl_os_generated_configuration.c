#include "tpl_os_internal_types.h"
#include "tpl_os_definitions.h"
#include "tpl_machine.h"
#include "tpl_os_it.h"
#include "tpl_os_it_kernel.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_os_action.h"

#ifndef WITH_AUTOSAR
#include "tpl_com_notification.h"
#include "tpl_com_mo.h"
#include "tpl_com_internal_com.h"
#include "tpl_com_app_copy.h"
#include "tpl_com_filters.h"
#endif

#include "tpl_os_generated_configuration.h"
#include "tpl_app_objects.h"

#ifdef WITH_AUTOSAR
#include "tpl_as_st_kernel.h"
#include "tpl_as_action.h"
#endif

/*=============================================================================
 * Definition and initialization of event related defines and structures
 */


/*=============================================================================
 * Definition and initialization of Resource related structures
 */



/*=============================================================================
 * Definition and initialization of Task related defines and structures
 */
extern tpl_internal_resource INTERNAL_RES_SCHEDULER;


/*
 * Controler_Voiture stacks
 *
 * System stack
 */
tpl_stack_word idata sys_stack_zone_of_Controler_Voiture[256/sizeof(tpl_stack_word)];

/*
 * User stack
 */
tpl_stack_word near usr_stack_zone_of_Controler_Voiture[256/sizeof(tpl_stack_word)];

#define STACK_OF_TASK_Controler_Voiture { sys_stack_zone_of_Controler_Voiture, 256, usr_stack_zone_of_Controler_Voiture, 256 }

/*
 * Controler_Voiture context
 */
c166_context integer_context_of_Controler_Voiture;

#define CONTEXT_OF_TASK_Controler_Voiture { &integer_context_of_Controler_Voiture }

/*
 * Task Controler_Voiture function prototype
 */
void function_of_task_Controler_Voiture(void);

#ifdef WITH_AUTOSAR_TIMING_PROTECTION

#endif

/*
 * Static descriptor of task Controler_Voiture
 */
tpl_exec_static static_descriptor_of_task_Controler_Voiture = {
    /* context                  */  CONTEXT_OF_TASK_Controler_Voiture,
    /* stack                    */  STACK_OF_TASK_Controler_Voiture,
    /* entry point (function)   */  function_of_task_Controler_Voiture,
    /* internal ressource       */  &INTERNAL_RES_SCHEDULER,
    /* task id                  */  task_id_of_Controler_Voiture,
    /* task base priority       */  (tpl_priority)2,
    /* max activation count     */  1,
    /* task type                */  TASK_BASIC,
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* pointer to the timing
       protection descriptor    */  NULL
#endif
};

/*
 * Dynamic descriptor of task Controler_Voiture
 */
tpl_task descriptor_of_task_Controler_Voiture = {
    {       /* beginning of exec_desc part */
    /* static descriptor    */  &static_descriptor_of_task_Controler_Voiture,
    /* resources            */  NULL,
    /* activate count       */  0,
    /* task priority        */  (tpl_priority)2,
    /* task state           */  SUSPENDED,
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* start date           */  0,
    /* time left            */  0,
#endif
    },    /* end of exec_desc part */
    /* event mask           */  0,
    /* event wait           */  0
};


/*
 * Generer_Trajectoire stacks
 *
 * System stack
 */
tpl_stack_word idata sys_stack_zone_of_Generer_Trajectoire[256/sizeof(tpl_stack_word)];

/*
 * User stack
 */
tpl_stack_word near usr_stack_zone_of_Generer_Trajectoire[256/sizeof(tpl_stack_word)];

#define STACK_OF_TASK_Generer_Trajectoire { sys_stack_zone_of_Generer_Trajectoire, 256, usr_stack_zone_of_Generer_Trajectoire, 256 }

/*
 * Generer_Trajectoire context
 */
c166_context integer_context_of_Generer_Trajectoire;

#define CONTEXT_OF_TASK_Generer_Trajectoire { &integer_context_of_Generer_Trajectoire }

/*
 * Task Generer_Trajectoire function prototype
 */
void function_of_task_Generer_Trajectoire(void);

#ifdef WITH_AUTOSAR_TIMING_PROTECTION

#endif

/*
 * Static descriptor of task Generer_Trajectoire
 */
tpl_exec_static static_descriptor_of_task_Generer_Trajectoire = {
    /* context                  */  CONTEXT_OF_TASK_Generer_Trajectoire,
    /* stack                    */  STACK_OF_TASK_Generer_Trajectoire,
    /* entry point (function)   */  function_of_task_Generer_Trajectoire,
    /* internal ressource       */  &INTERNAL_RES_SCHEDULER,
    /* task id                  */  task_id_of_Generer_Trajectoire,
    /* task base priority       */  (tpl_priority)1,
    /* max activation count     */  1,
    /* task type                */  TASK_BASIC,
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* pointer to the timing
       protection descriptor    */  NULL
#endif
};

/*
 * Dynamic descriptor of task Generer_Trajectoire
 */
tpl_task descriptor_of_task_Generer_Trajectoire = {
    {       /* beginning of exec_desc part */
    /* static descriptor    */  &static_descriptor_of_task_Generer_Trajectoire,
    /* resources            */  NULL,
    /* activate count       */  0,
    /* task priority        */  (tpl_priority)1,
    /* task state           */  SUSPENDED,
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    /* start date           */  0,
    /* time left            */  0,
#endif
    },    /* end of exec_desc part */
    /* event mask           */  0,
    /* event wait           */  0
};


tpl_task *tpl_task_table[TASK_COUNT] = {
    (tpl_task *)&descriptor_of_task_Controler_Voiture,
    (tpl_task *)&descriptor_of_task_Generer_Trajectoire
};


/*=============================================================================
 * Definition and initialization of ISR2 related defines and structures
 */



/*=============================================================================
 * Definition and initialization of Counters related defines and structures
 */

/*
 * Counter descriptor of counter General_counter
 */
tpl_counter descriptor_of_counter_General_counter = {
    /* ticks per base       */  1,
    /* max allowed value    */  65535,
    /* minimum cycle        */  1,
    /* current tick         */  0,
    /* current date         */  0,
    /* first alarm          */  NULL,
    /* next alarm to raise  */  NULL
};

/*
 * Tick for counters. 
 * There is a tick each 1ms for a 40 MHz microcontroller
 * WARNING: It uses *** timer 6 ***
 * This timer MUST NOT be used elsewhere in the application.
 */
#include <C167CS.H> /*TODO: C166 */
void tpl_init_tick_timer()
{
  /* use interrupt mode: 
   * T6IE:1 Interrupt enable
   * ilvl:13 Interrupt Level
   * glvl:1 Group Level
  */
  T6IC = 0x61;
  /* at 40 MHz, resultion max is 100ns.
   * To get 1ms, we need 10 000 ticks.
   * 65536 - 10000 = 55536
   */
  CAPREL = 55536;
  /* T6SR :1 reload with CAPREL
   * T6OTL:0 No toggle latch.
   * T6OE :0 Output Function disabled
   * T6UDE:0 No external Up/Down
   * T6UD :0 Count Up
   * T6R  :1 Run
   * T6M  :0 Timer mode
   * T6I  :0 100ns resolution
   */
  T6CON = 0x8040;
}

void tpl_schedule(int from);
tpl_status tpl_counter_tick(tpl_counter *counter);
void tpl_call_counter_tick()
{
	tpl_status  need_rescheduling = NO_SPECIAL_CODE;
    need_rescheduling |= tpl_counter_tick(&descriptor_of_counter_General_counter);

	if (need_rescheduling == NEED_RESCHEDULING) {
		tpl_schedule(FROM_IT_LEVEL);
    }
}

extern unsigned int registers_it[16]; 

#pragma NOFRAME
#pragma warning disable = 138 /* disables the "expression with possibly no effect" warning */
/* timer 6 -> trap number 38*/
void tpl_timer6_tick(void) interrupt 38
{
	__asm {
		BCLR IEN /*disable interrupt */
		SCXT DPP3, #3
		NOP
		SCXT CP,#registers_it
		NOP
		MOV R0,0xfe12 
		MOV R0,[R0] //get R0<-CP
		MOV R0,[R0] //get the previous user stack.
		NOP
		PUSH DPP0
	}
	
	/* We have to call a function, because we can not use any local var here.*/
	tpl_call_counter_tick();
	
	/* dummy code to allow the use of registers_it in
	 * the assembly code
	 */
	if(registers_it[0]);
	__asm {
		POP DPP0
		POP CP
		POP DPP3
	}
}




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
    /* task descriptor ptr  */  &descriptor_of_task_Controler_Voiture
};

tpl_alarm_static stat_descriptor_of_alarm_Active_Tache1 = {
    {
        /* pointer to counter           */  &descriptor_of_counter_General_counter,
        /* pointer to the expiration    */  tpl_raise_alarm
    },
    /* action of the alarm  */  (tpl_action *)&task_act_of_Active_Tache1
};

tpl_time_obj descriptor_of_alarm_Active_Tache1 = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&stat_descriptor_of_alarm_Active_Tache1,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  1,
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
    /* task descriptor ptr  */  &descriptor_of_task_Generer_Trajectoire
};

tpl_alarm_static stat_descriptor_of_alarm_Active_Tache2 = {
    {
        /* pointer to counter           */  &descriptor_of_counter_General_counter,
        /* pointer to the expiration    */  tpl_raise_alarm
    },
    /* action of the alarm  */  (tpl_action *)&task_act_of_Active_Tache2
};

tpl_time_obj descriptor_of_alarm_Active_Tache2 = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&stat_descriptor_of_alarm_Active_Tache2,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  10,
    /* date                         */  1,
    /* State of the alarm           */  ALARM_AUTOSTART
};

tpl_time_obj *tpl_alarm_table[ALARM_COUNT] = {
    (tpl_time_obj *)&descriptor_of_alarm_Active_Tache1,
    (tpl_time_obj *)&descriptor_of_alarm_Active_Tache2
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
#endif

/*=============================================================================
 * Definition and initialization of Ready List structures
 */
tpl_exec_common *tpl_priority_0_fifo[1];
tpl_exec_common *tpl_priority_1_fifo[1];
tpl_exec_common *tpl_priority_2_fifo[2];

tpl_fifo_state tpl_fifo_rw[3] = {
    { 0 , 0 },
    { 0 , 0 },
    { 0 , 0 }
};

tpl_priority_level tpl_ready_list[3] = {
    { tpl_priority_0_fifo , 1 },
    { tpl_priority_1_fifo , 1 },
    { tpl_priority_2_fifo , 2 }
};



/* End of file tpl_os_generated_configuration.c */
