#ifndef __METRO_LIB
#define __METRO_LIB
					   /*--- commentez les lignes pour désactiver ces fonctionalites----*/

//#define USE_STAR_USART 
//#define USE_DMA_USART 
#ifdef USE_DMA_USART
	#define USARTx USART3 
	#define IN_BUFF_SIZE 25
	#define SEND_BUFF_SIZE 16

	extern void flush_serial_input(void);
#endif /* USE_DMA_USART */ 


#define USE_ADC
#ifdef USE_ADC 
	extern  short int Lire_courant(void);
#endif /* USE_ADC */


#define USE_POSITION
#ifdef USE_POSITION
	extern  int Lire_Position(void);
#endif /* USE_POSITION */


#define USE_SPEED
#ifdef USE_SPEED
	#define DT 0.001     // pas de calcul de la vitesse	en s
	extern  int Lire_Vitesse(void);
#endif	/* USE_SPEED */


#define USE_PWM
#ifdef USE_PWM  
	extern void Fixe_Rapport(short int);
#endif /* USE_PWM */
				    /*--------------------------------------end---------------------------------*/

extern void Init_Periphs(void);	

#endif /* __METRO_LIB */
