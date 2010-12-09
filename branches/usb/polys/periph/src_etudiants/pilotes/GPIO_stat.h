#include "../sys/stm_regs.h"

//=======================================================================================

// 	Auteur Periph Team
//  1 Sept 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure les I/O
//
//  ________ Rappel sur les ressources __________________________________________________
// Broche : de 0 à 15 pour GPIOA, GPIOB, 
//		  : de 0 à 13 pour GPIOC 
//
//  ________ Comment utiliser la lib ? __________________________________________________
//   
//	1 - configurer la broche voulue Fct : char Port_IO_Init(char Port, char Broche, char Sens, char Techno);
//  2 - opérer une écriture : void Port_IO_Set(char Port, char Broche) et void Port_IO_Clr(char Port, char Broche)
//  3 - opérer une lecture : unsigned int Port_IO_Lire(char Port, char Broche);
//
//=======================================================================================
//=======================================================================================

#ifndef _Port_IO_H__
#define _Port_IO_H__

//__________________________________________________________________________________________
//
void Port_IO_Init(void);
//__________________________________________________________________________________________
//




#define PORT_IO_SET(Port,Broche) Port->ODR |= (1<<Broche)
// Spécifier Port (A, B ou C)
// Spécifier le numéro de broche (0 à 15)

//__________________________________________________________________________________________
//
#define PORT_IO_CLR(Port,Broche) Port->ODR &= ~(1<<Broche)
// Spécifier Port (A, B ou C)
// Spécifier le numéro de broche (0 à 15)

//__________________________________________________________________________________________
//
#define PORT_IO_LIRE(Port, Broche) (Port->IDR & (1<<Broche))
// Spécifier Port (A, B ou C)
// Spécifier le numéro de broche (0 à 15)
// Renvoie l'état de la broche, 0 ou autre (si 1)
#endif
