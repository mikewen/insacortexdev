#ifndef __STM_METRO
#define __STM_METRO

//IMPORTANT 
// Faites une copie stm_metro_v1_config_TEMPLATE.h dans votre répertoire de projet 
// et configurez votre copie locale pour adapter cette librairie
#include "stm_metro_v1_config.h"



// Appelez cette fonction au début
//  Position = 0
//  PWM à l'arret
//  Vitesse = 0 (Arrêt de mesure)  
extern void Init_Periphs(void);	


#ifdef USE_ADC 
	extern  u16 Lire_courant(void);
#endif

#ifdef USE_POSITION
	extern  s16 Lire_Position(void);
	extern void Set_Position(s16 pos) ;
#endif

#ifdef USE_SPEED
	extern  u16 Lire_Vitesse(void);
//	#define Lire_Vitesse_En_Float (Gain_Vitesse/((float)Lire_Vitesse()))
#endif

#ifdef USE_PWM
	#define RAPPORT_MAX (0x0FFF)
	#define RAPPORT_MIN (-0x0FFF)
	#define RAPPORT_ARRET (0)
	extern void Fixe_Rapport(s16);
#endif

#ifdef USE_BLINK_LEDS
	extern void Blink_Leds(int repet, int duree) ;
	#define DUREE_RAPIDE (0x1FFF*90)
	#define DUREE_LENTE (0x1FFF*150)
#endif




#endif
