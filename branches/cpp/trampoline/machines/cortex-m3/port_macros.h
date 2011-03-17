/*_______________________________________________________________________
	Port macro widely inspired from 
	FreeRTOS.org V5.3.1 - Copyright (C) 2003-2009 Richard Barry.

	http://www.FreeRTOS.org - Documentation, latest information, license and
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting,
	licensing and training services.
*/


#ifndef PORTMACRO_H
#define PORTMACRO_H

/*-----------------------------------------------------------
 * Port specific definitions.  
 *-----------------------------------------------------------
 */



/* configuration */

// Contournement du bug R1
// Pour activer le contournement du bug R1 dans le changement de contexte, activez la ligne suivant
#define __BUG_R1_WORKAROUND__

/* Type definitions.  */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	unsigned portLONG
#define portBASE_TYPE	long
					 
typedef unsigned portLONG portTickType;
#define portMAX_DELAY ( portTickType ) 0xffffffff

// TICK configuration
#define configCPU_CLOCK_HZ			( ( unsigned portLONG ) 40000000 )	
#define configTICK_RATE_HZ			( ( portTickType ) 1000 )

					 
/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_RATE_MS			( ( portTickType ) 1000 / configTICK_RATE_HZ )		
#define portBYTE_ALIGNMENT			4

/* Constants required to set up the initial stack. */
#define portINITIAL_XPSR			( 0x01000000 )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY ( 0x0F0 ) 


/*-----------------------------------------------------------*/	

/* Critical section management. */

/* 
 * Set basepri to 1 without effecting other
 * registers.  r0 is clobbered.
 */ 

/* Disable priority based interrupt */
#define portSET_INTERRUPT_MASK()  __asm__ (" CPSID	I")
							
/*
 * Set basepri back to 0 without effective other registers.
 * r0 is clobbered.
 */

/* Enable priority based interrupt */
#define portCLEAR_INTERRUPT_MASK()  __asm__ (" CPSIE	I")

#define portDISABLE_INTERRUPTS()	portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()		portCLEAR_INTERRUPT_MASK()

/*-----------------------------------------------------------*/

#endif /* PORTMACRO_H */

