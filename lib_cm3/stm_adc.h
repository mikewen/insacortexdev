/*----------------------------------------------------------------------------
 * Name:    lib_adc
 * Purpose: ADCs usage for STM32
 * Version: V1.00
 *----------------------------------------------------------------------------
 * ACCO [12 Mars  2011] création 
 *----------------------------------------------------------------------------
 USAGE
	??? is the name of the library i.e. stm_adc
	RELPATH is the relative path from your projetc directory to lib_cm3 directory	
	
   	include RELPATH/lib_cm3/???.c file in your makefile
	copy    RELPATH/lib_cm3/???_config_TEMPLATE.h in your project directory	(noted ./)
	rename  ./???_config_TEMPLATE.h as ./???_config.h in your project directory
	edit    ./???_config.h file for your project (lines with //CONF tags)  
	ensure that ./ path is visible in CC compiler options 
	add "#include "RELPATH/lib_cm3/???.h" in app. code 
	add a call to "Init_???();" at initialisation step of your APP
 */
#include "stm_regs.h" 


#ifndef __STM_ADC
#define __STM_ADC

// Go to stm_adc_config to cinfigure adc behaviour
void Init_Adc(void);

#define ADC1_START_CONV 	ADC1->CR2 |= ADC_ADON
#define ADC2_START_CONV 	ADC2->CR2 |= ADC_ADON 


#endif /* __STM_ADC */
