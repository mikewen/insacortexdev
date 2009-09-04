#ifndef __METRO_LIB
#define __METRO_LIB
					   /*--- commentez les lignes pour désactiver ces fonctionalites----*/

//#define USE_STAR_USART 
#define USE_DMA_USART  
#define USE_SPEED
#define USE_ADC
#define USE_POSITION
#define USE_PWM 
				    /*--------------------------------------end---------------------------------*/

#define DT 0.001     // pas de calcul de la vitesse	en s



extern void Init_Periphs(void);	
extern void Fixe_Rapport(short int);
extern  int Lire_Vitesse(void);
extern  int Lire_Position(void);
extern  short int Lire_courant(void);

#endif
