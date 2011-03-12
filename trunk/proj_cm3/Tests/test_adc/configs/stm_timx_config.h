//=======================================================================================

// 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure les PWM possibles sur le uC 
//
//  ________ Rappel sur les ressources __________________________________________________
//
//  3 Timer "general Purpose", TIM2, TIM3 et TIM4
//  Chacun d'entre eux dispose de 4 voies de sorties numérotées de 1 à 4
//  Mapping des IO:
//  	TM2_CH1_ETR - PA0 	TM3_CH1 - PA6		TIM4_CH1 - PB6	
//    	TM2_CH2 - PA1		TM3_CH2 - PA7		TIM4_CH2 - PB7
// 		TM2_CH3 - PA1		TM3_CH3 - PB0		TIM4_CH3 - PB8
// 		TM2_CH4 - PA3		TM3_CH4 - PB1		TIM4_CH4 - PB9
//
//   Chaque sortie physique, par exemple PA1, partage l'IO et l'alternate function (PWM ici)
//
//   Chaque Timer possède 
//			- 1 prescaler 16 bits.
//			- 1 Timer bien sûr
//			- 1 registre Reload (pour définir la période PWM)
//			- 4 registres de comparaisons CCR qui permettent de faire les PWM
//			- un dispositif de synchro pour updater tous les registres sur un event (pas utilisé)
//
//  ________ Comment utiliser la lib ? __________________________________________________
//   
//	1 - configurer la PWM avec la fct PWM_Init(char Timer, char Voie, float Frequence_PWM_Khz, float Frequence_Ck_Timer_Khz );
//  2 - dériver la broche de sortie sur l'alternate fct, broche en output : voir Port_IO.h
//  3 - utiliser les define pour affecter la valeur PWM : 

//__________ Exemple de configuration : _________________________________________________
//
// Reso= PWM_Init(2, 4, 20.0, 8000.0 );	   
// Port_IO_Init('A', 3, 'o', 2);
// PWM_Valeur_Timer_2_Voie_4=Reso/2;
//
// On obtient sur la voie PA3, une PWM à 20kHz (sachant que le prescaler est attaqué à 8MHz
// il s'agit du timer 2 en PWM sur la voie 4 (PA3). Rapport cyclique de 50%
//
//=======================================================================================



#ifndef _TIMX_CONF_H__
#define _TIMX_CONF_H__

//_______________________________________________________________________________________
//
// PWM_Init(char Timer, char Voie, float Frequence_PWM_Khz, float Frequence_Ck_Timer_Khz );
//     
//		- Timer : de 2 à 4, c'est le numéro du timer de la PWM
//		- Voie  : de 1 à 4, c'est la voie de sortie PWM
// 		- Frequence_PWM_Khz : précise la fréquence PWM voulue. NB: préciser bien .0 pour le float
//		- Frequence_Ck_Timer_Khz : la fréquence qui attaque le prescaler du Timer
//		   NB : les fréquences sont donnée en KHz, par exemple 20kHz => 20.0
//
//		- Renvoie la résolution maximale (valeur discrète maxi pour la largeur)
//        Il faut la connaître pour ensuite pouvoir utiliser la pleine échelle de la PWM
//
//
//________________________________________________________________________________________



//__________________TIMER2 CONFIGURATION____________________________________
//CONFIG : uncomment the following line to use the TIMER
//#define TIM2_IS_USED
#ifdef TIM2_IS_USED
	#ifndef TIMx_IS_USED
	 #define TIMx_IS_USED
	#endif

   	//CONFIG : configure the timer frequency
   	#define TIM2_FREQ	1000

  	//CONFIG : uncomment the following line to use interruption
	#define TIM2_IT_IS_USED
	#ifdef TIM2_IT_IS_USED
		//CONFIG: set the TIM2 interrupt level (0=High 15=lowest)
		#define TIM2_PRIO	5
		//CONFG: add prototype and define the function to be called on interrupt
		extern void blinkit(void);
		#define TIM2_HOOK blinkit()
	#endif
	
   //CONFIG : uncomment the following line to use the PWM channel
   #define TIM2_CHANNEL_1_IS_PWM	// PA0
   #define TIM2_CHANNEL_1_DUTY_CYCLE 0.0
   #define TIM2_CHANNEL_2_IS_PWM	// PA1
   #define TIM2_CHANNEL_2_DUTY_CYCLE 25.0
   #define TIM2_CHANNEL_3_IS_PWM	// PA2
   #define TIM2_CHANNEL_3_DUTY_CYCLE	75.0
   #define TIM2_CHANNEL_4_IS_PWM	// PA3
   #define TIM2_CHANNEL_4_DUTY_CYCLE	100.0
    

#endif


//__________________TIMER3 CONFIGURATION____________________________________
//CONFIG : uncomment the following line to use the TIMER
//#define TIM3_IS_USED
#ifdef TIM3_IS_USED

	#ifndef TIMx_IS_USED
	 #define TIMx_IS_USED
	#endif

   	//CONFIG : configure the timer frequency
   	#define TIM3_FREQ	1000

  	//CONFIG : uncomment the following line to use interruption
	#define TIM3_IT_IS_USED
	#ifdef TIM3_IT_IS_USED
		//CONFIG: set the TIM3 interrupt level (0=High 15=lowest)
		#define TIM3_PRIO	5
		//CONFG: add prototype and define the function to be called on interrupt
		extern void blinkit(void);
		#define TIM3_HOOK blinkit()
	#endif
	
   //CONFIG : uncomment the following line to use the PWM channel
   #define TIM3_CHANNEL_1_IS_PWM	// PA6
   #define TIM3_CHANNEL_1_DUTY_CYCLE 0.0
   #define TIM3_CHANNEL_2_IS_PWM	// PA7
   #define TIM3_CHANNEL_2_DUTY_CYCLE 25.0
   #define TIM3_CHANNEL_3_IS_PWM	// PB0
   #define TIM3_CHANNEL_3_DUTY_CYCLE	75.0
   #define TIM3_CHANNEL_4_IS_PWM	// PB1
   #define TIM3_CHANNEL_4_DUTY_CYCLE	100.0
    

#endif

//__________________TIMER4 CONFIGURATION____________________________________
//CONFIG : uncomment the following line to use the TIMER
//#define TIM4_IS_USED
#ifdef TIM4_IS_USED

	#ifndef TIMx_IS_USED
	 #define TIMx_IS_USED
	#endif

   	//CONFIG : configure the timer frequency
   	#define TIM4_FREQ	200000

  	//CONFIG : uncomment the following line to use interruption
	#define TIM4_IT_IS_USED
	#ifdef TIM4_IT_IS_USED
		//CONFIG: set the TIM4 interrupt level (0=High 15=lowest)
		#define TIM4_PRIO	5
		//CONFG: add prototype and define the function to be called on interrupt
		extern void blinkit(void);
		#define TIM4_HOOK blinkit()
	#endif
	
   //CONFIG : uncomment the following line to use the PWM channel
   #define TIM4_CHANNEL_1_IS_PWM	// PB6
   #define TIM4_CHANNEL_1_DUTY_CYCLE 0.0
   #define TIM4_CHANNEL_2_IS_PWM	// PB7
   #define TIM4_CHANNEL_2_DUTY_CYCLE 25.0
   #define TIM4_CHANNEL_3_IS_PWM	// PB8
   #define TIM4_CHANNEL_3_DUTY_CYCLE	50.0
   #define TIM4_CHANNEL_4_IS_PWM	// PB9
   #define TIM4_CHANNEL_4_DUTY_CYCLE	100.0
    

#endif



#endif
