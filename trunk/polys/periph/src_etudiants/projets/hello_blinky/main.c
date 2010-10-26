//__________________________________________________________
// projet/hello_blinky/      MAIN.C
// Tentaive de faire clignoter une diode
// BINOME :
// ETAT : ni compilé ni testé
// TODO : lire la doc, configurer, compiler, deboguer
//   Quand vous serrez grands vous pourrez enlever les
// commentaires //REMARQUES qui sont à but pédagogiques
// et inutile pour les geeks
//__________________________________________________________

//____ registres de périphériques du stm_32
#include "stm_regs.h"
//REMARQUES
//  si le fichier n'est pas visible faites :
//     Project->options for target
//     onglet C/C++ remplir "include paths"
//      pour rendre visible le répertoire sys

//___ pilote de l'arbre d'horloges
#include "../../pilotes/stm_clock.h"
//REMARQUES
// ATTENTION c'est une librairie statique :
// il faut qu'un fichier stm_clock_config.h soit dans
// le répertoire projet et qu'il soit visible (ajouter
// le répertoire du projet à l'include path des option C/C++)
// Normalement c'est le cas et le fichier non modifié
// régle les fréquences à
//   SYSCLK = 40MHz = 8 (HSE quartz) * 5 (PLL)
//    SYSCLK est l'horloge du coeur qui est
//    issue du quartz externe "HSE" à 8MHz qui arrive 
//    direct sur une Phase Locked Loop PLL qui 
//    multiplie la fréquence par 5.
// 	PCLK1 = SYSCLK / 2(prescaler PRE1) = 20 MHz  
//   sortie de l'APB1 "low speed" pour périph lents.
//  PCLK2 = SYSCLK / 1 (prescaler PRE2)= 40 MHz
//   sortie de l'APB2 "high speed" pour périph pressés.
    

int main (void)
{
	//___ initialise les horloges SYSCLK,PCLK1,PCLK2
	Init_Clock_System() ; 
	//REMARQUES
	// Attention si vous oubliez cette ligne, il n'y a pas de 
	// configuration : le processeur reste sur une horloge interne
	// à la puce (oscillateur RC nommé High Speed Internal clock)
	// c'est oscillateur oscille vaguement aux alentours de 8MHz !
	 
	//___ validation de l'horloge des ports GPIOA GPIOB et GPIOC	
 	(RCC->APB2ENR)|= RCC_IOPAEN | RCC_IOPBEN | RCC_IOPCEN;
 	//REMARQUES
	// en effet le bridge APB2 amène une horloge à 40MHz 
	// aux périphériques tels que ces GPIO. Par défaut les 
	// périphériques sont étteints pour ne pas consommer de
	// courant. Cette initialisation permet d'allumer l'horloge
	// de chaque périphériques. Soyez TRES VIGILENTS car sans
	// cette ligne de validation lors d'un accès à un registre,
	//  par exemple GPIOA->ODR = ..., le programme compile, 
	//  s'exécute mais comme le registre GPIOA_ODR ne reçoit pas
	//  d'horloge il ne mémorise pas la valeur désirée et rien
	//  ne marche malgré les apparences... 
	// Bon courage les futurs geeks
	GPIOB->CRH = 0x44444474;
	GPIOB->CRL = 0x44484444;
	while(1)
	{
		GPIOB->ODR ^=(1<<9);
	}
	//__ configuration du port PB.8 à PB.15 pour faire blinky
	// sur une ou plusieurs diodes de la carte MCBSTM32
	
	return 0;
}
