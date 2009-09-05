#ifndef __METRO_LIB
#define __METRO_LIB
					   /*--- commentez les lignes pour désactiver ces fonctionalites----*/

//#define USE_STAR_USART 
#define USE_DMA_USART 
#ifdef USE_DMA_USART
	#define USART_NUM 2 
	#define IN_BUFF_SIZE 25
	#define SEND_BUFF_SIZE 16

	extern void flush_serial_input(void);
#endif


//#define USE_ADC
#ifdef USE_ADC 
	extern  short int Lire_courant(void);
#endif


//#define USE_POSITION
#ifdef USE_POSITION
	extern  int Lire_Position(void);
#endif


//#define USE_SPEED
#ifdef USE_SPEED
	#define DT 0.001     // pas de calcul de la vitesse	en s
	extern  int Lire_Vitesse(void);
#endif



#define USE_PWM 
#ifdef USE_PWM
	extern void Fixe_Rapport(short int);
#endif
				    /*--------------------------------------end---------------------------------*/




extern void Init_Periphs(void);	



#endif
