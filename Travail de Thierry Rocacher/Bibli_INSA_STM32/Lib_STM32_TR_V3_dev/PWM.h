


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
// 		TM2_CH3 - PA1		TM3_CH3 - PA8		TIM4_CH3 - PB8
// 		TM2_CH4 - PA3		TM3_CH4 - PA9		TIM4_CH4 - PB9
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
// PWM_Valeur_Timer_2_Voie_4=Reso/2;  Reso de type vu16 !
//
// On obtient sur la voie PA3, une PWM à 20kHz (sachant que le prescaler est attaqué à 8MHz
// il s'agit du timer 2 en PWM sur la voie 4 (PA3). Rapport cyclique de 50%
//
//=======================================================================================



#ifndef _PWM_H__
#define _PWM_H__

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

unsigned int PWM_Init(char Timer, char Voie, float Frequence_PWM_Khz, float Frequence_Ck_Timer_Khz );


//_______________________________________________________________________________________
//
// Les définitions, pratiques car lisibles, qui permettent d'affecter la valeur PWM
//
// Exemple d'utilisation dans le pgm appelant :
//	 PWM_Valeur_Timer_2_Voie_3 = 512;
//
// ce qui veut dire	affecter 512 en largeur PWM sur la voie 3 gérée par le Timer 2
// NB : le type à utiliser est vu16 !
//_______________________________________________________________________________________
//


#define PWM_Valeur_Timer_2_Voie_1 TIM2->CCR1
#define PWM_Valeur_Timer_2_Voie_2 TIM2->CCR2
#define PWM_Valeur_Timer_2_Voie_3 TIM2->CCR3
#define PWM_Valeur_Timer_2_Voie_4 TIM2->CCR4

#define PWM_Valeur_Timer_3_Voie_1 TIM3->CCR1
#define PWM_Valeur_Timer_3_Voie_2 TIM3->CCR2
#define PWM_Valeur_Timer_3_Voie_3 TIM3->CCR3
#define PWM_Valeur_Timer_3_Voie_4 TIM3->CCR4

#define PWM_Valeur_Timer_4_Voie_1 TIM4->CCR1
#define PWM_Valeur_Timer_4_Voie_2 TIM4->CCR2
#define PWM_Valeur_Timer_4_Voie_3 TIM4->CCR3
#define PWM_Valeur_Timer_4_Voie_4 TIM4->CCR4




#endif
