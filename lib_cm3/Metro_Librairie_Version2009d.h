#ifndef __METRO_LIB
#define __METRO_LIB
					   /*--- commentez les lignes pour désactiver ces fonctionalites----*/


//#define USE_ADC
#ifdef USE_ADC 
	extern  short int Lire_courant(void);
#endif


#define USE_POSITION
#ifdef USE_POSITION
	extern  int Lire_Position(void);
	extern void Set_Position(int);
#endif


//#define USE_SPEED
#ifdef USE_SPEED
	#define DT 0.001     // pas de calcul de la vitesse	en s
	extern  int Lire_Vitesse(void);
#endif



#define USE_PWM 
#ifdef USE_PWM
	#define RAPPORT_MAX (0x0FFF)
	#define RAPPORT_MIN (-0x0FFF)
	#define RAPPORT_ARRET (0)
	extern void Fixe_Rapport(short int);
#endif
				    /*--------------------------------------end---------------------------------*/




extern void Init_Periphs(void);	



#endif
