

//=======================================================================================

// 	Auteur T.R.
//  4 Mai 2010
//  Module qui gere les IO du STM32 (configuration des E/S, mise à 1, à 0, lecture 

//=======================================================================================


#ifndef _Port_IO_H__
#define _Port_IO_H__

//__________________________________________________________________________________________
//
char Port_IO_Init(char Port, char Broche, char Sens, char Techno);
// Doc tech : STM32F103RM.pdf
// Port : Char A à C 
// Broche : de 0 à 15 pour GPIOA, GPIOB, 
//		  : de 0 à 13 pour GPIOC 
// Sens   : I pour entree, O pour sortie

// Techno : 0 Analog						  / Output push-pull
//			1 Input floating				  / Output open-drain
//			2 Input-pull-down / up			  / Alternate function push-pull
//			3 reserved						  / Alternate function open-drain
// 	REMARQUE : Pour entree pull down/up, on choisit down en écrivant 0 en sortie
//				et up pour 1 en sortie
// Retourne 0 si tout est OK, 1 sinon

//__________________________________________________________________________________________
//
void Port_IO_Set(char Port, char Broche);
// Spécifier Port (A, B ou C)
// Spécifier le numéro de broche (0 à 15)

//__________________________________________________________________________________________
//
void Port_IO_Clr(char Port, char Broche);
// Spécifier Port (A, B ou C)
// Spécifier le numéro de broche (0 à 15)

//__________________________________________________________________________________________
//
unsigned int Port_IO_Lire(char Port, char Broche);
// Spécifier Port (A, B ou C)
// Spécifier le numéro de broche (0 à 15)
// Renvoie l'état de la broche, 0 ou autre (si 1)
#endif
