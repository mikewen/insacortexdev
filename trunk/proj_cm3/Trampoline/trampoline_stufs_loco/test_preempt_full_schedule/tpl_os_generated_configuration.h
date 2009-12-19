#ifndef APP_HEADER_H
#define APP_HEADER_H

#include "tpl_app_objects.h"

#if !defined(NO_TASK) || !defined(NO_ISR)
#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
extern CONSTP2CONST(char, AUTOMATIC, OS_APPL_DATA) proc_name_table[TASK_COUNT + ISR_COUNT];
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
#endif


/*=============================================================================
 * Declaration of Counters macros
 */
extern CONST(tpl_tick, OS_CONST) OSTICKSPERBASE_General_counter;
extern CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE_General_counter;
extern CONST(tpl_tick, OS_CONST) OSMINCYCLE_General_counter;

extern CONST(tpl_tick, OS_CONST) OSTICKSPERBASE;
extern CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE;
extern CONST(tpl_tick, OS_CONST) OSMINCYCLE;



/*=============================================================================
 * Declaration of flags macros
 */
 

#endif

/* End of file tpl_os_generated_configuration.h */
