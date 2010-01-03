#ifndef __STM_METRO
#define __STM_METRO
					   /*--- commentez les lignes pour désactiver ces fonctionalites----*/
#include "stm_metro_v1_config.h"


#ifdef USE_ADC 
	extern  u16 Lire_courant(void);
#endif

#ifdef USE_POSITION
	extern  s16 Lire_Position(void);
	extern void Set_Position(s16 pos) ;
#endif

#ifdef USE_SPEED
	extern  u16 Lire_Vitesse(void);
	#define COEFF_VITESSE (4*SPEED_TIC_FREQ)
//	#define Lire_Vitesse_En_Float (Gain_Vitesse/((float)Lire_Vitesse()))
#endif

#ifdef USE_PWM
	#define RAPPORT_MAX (0x0FFF)
	#define RAPPORT_MIN (-0x0FFF)
	#define RAPPORT_ARRET (0)
	extern void Fixe_Rapport(s16);
#endif
				    /*--------------------------------------end---------------------------------*/




extern void Init_Periphs(void);	



#endif
