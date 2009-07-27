#ifndef APP_HEADER_H
#define APP_HEADER_H

#include "tpl_app_objects.h"

/*=============================================================================
 * Declaration of event related defines and structures
 */


/*=============================================================================
 * Declaration of Resource related defines and structures
 */



/*=============================================================================
 * Declaration of Task related defines and structures
 */

#define task_id_of_Controler_Voiture  0
#define Controler_Voiture  task_id_of_Controler_Voiture
#define task_id_of_Generer_Trajectoire  1
#define Generer_Trajectoire  task_id_of_Generer_Trajectoire


/*=============================================================================
 * Declaration of ISR2 related defines and structures
 */



/*=============================================================================
 * Declaration of Counters related defines and structures
 */
#define OSTICKSPERBASE_General_counter 1
#define OSMAXALLOWEDVALUE_General_counter 65535
#define OSMINCYCLE_General_counter 1



/*=============================================================================
 * Declaration of Alarm related defines and structures
 */

#define alarm_id_of_Active_Tache1  0
#define Active_Tache1  alarm_id_of_Active_Tache1
#define alarm_id_of_Active_Tache2  1
#define Active_Tache2  alarm_id_of_Active_Tache2


/*=============================================================================
 * Declaration of flags macros
 */
 

/*=============================================================================
 * Declaration of messages identifiers
 */






#ifdef WITH_AUTOSAR
/*=============================================================================
 * Declaration of schedule tables related defines and structures
 */
$SCHEDULETABLES$

/*=============================================================================
 * Declaration of counters ids
 */


#endif

#endif

/* End of file tpl_os_generated_configuration.h */
