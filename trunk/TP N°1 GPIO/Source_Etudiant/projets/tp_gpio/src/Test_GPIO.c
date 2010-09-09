//__________________________________________________________
// projet/TP_GPIO/      MAIN.C
// Tentative de faire clignoter une diode
// BINOME :
// ETAT : compilé et testé
//__________________________________________________________

/** TODO : Compiler ce fichier. Exécuter le en simulation puis passer irl. Constatez ce qui se passe.
**/

/** TODO : Lire le code et faire les TODO présents dans le code
**/

/** TODO : Ajouter la configuration en entrée du port PA.0 qui est cablé au bouton WKUP et allumer la diode PB13 quand le 
** bouton est maintenu appuyé.
**/

/** TODO : Ecrire une fonction qui permet de configurer une broche d'un port en output push-pull.
**			Le prototype de la fonction sera : char Port_IO_Init_Output( GPIO_TypeDef * Port, char Broche)
**/

/** TODO : Ecrire une fonction qui permet de configurer une broche d'un port en input floating.
**			Le prototype de la fonction sera : char Port_IO_Init_Input( GPIO_TypeDef * Port, char Broche)
**/

/** TODO : Ecrire une fonction qui permet de mettre à 1 une broche d'un port
** 			Le prototype de la fonction sera :void Port_IO_Set(GPIO_TypeDef * Port, char Broche)
**/
/** TODO : Ecrire une fonction qui permet de mettre à 0 une broche d'un port
** 			Le prototype de la fonction sera :void Port_IO_Reset(GPIO_TypeDef * Port, char Broche)
*/
/** TODO : Ecrire une fonction qui inverse l'état d'une broche
** 			Le prototype de la fonction sera :void Port_IO_Blink(GPIO_TypeDef * Port, char Broche)
*/
/** TODO : Ecrire une fonction qui permet de lire l'état d'une broche d'un port
**			Le prototype de la fonction sera : unsigned int Port_IO_Read(GPIO_TypeDef * Port, char Broche)
**/
/** TODO : Créer un fichier gpio.c qui contient ces fonctions et un fichier d'en-tête gpio.h et les placer dans le 
** répertoire ../../pilotes/gpio  Modifier Test_GPIO.c pour faire disparaître tout ce qui est directement
** en lien avec le périph. Vous ne pouvez que faire appel à vos nouvelles fonctions.
**/


//____ registres de périphériques du stm_32
#include "stm_regs.h"
//REMARQUES
//  si le fichier n'est pas visible faites :
//     Project->options for target
//     onglet C/C++ remplir "include paths"
//      pour rendre visible le répertoire sys


int main (void)
{
	char flipflop = 1;
	
 	// Cette ligne est nécessaire pour le bon fonctionnement de la carte, pour l'instant oubliez là !
 	(RCC->APB2ENR)|= RCC_IOPAEN | RCC_IOPBEN | RCC_IOPCEN;

	// CRH = 1111 1111 : 1111 1111 : 1111 1111 : 1111 1111 
	GPIOB->CRH = 0xFFFFFFFF;
	//__ configuration du port PB.9 en sortie (output push-pull)
	// CRH = xxxx xxxx : xxxx xxxx : xxxx xxxx : 0001 xxxx 	
	GPIOB->CRH = (0x01 << 4);
	// TODO : Qu'est-ce que veut dire (x << y) ?
	// TODO : Pour ne pas toucher aux autres bits que le 9 du registre CRH, il faut faire un masque. Faites le ! 
	 
	// Faire clignoter la diode sur le port PB.9
	while(1)
	{
		if (flipflop == 1) {
		// ODR = xxxx xxxx : xxxx xxxx : xxxx xx1x : xxxx xxxx 
			GPIOB->ODR = (0x01 << 9); 
			// TODO : Comment faire pour ne changer que le bit 9 ?
			flipflop = 0;
			// TODO : Comment faier pour se passer du flipflop en utilisant directement la valeur de ODR ?
		} else {
		// ODR=xxxx xxxx | xxxx xxxx | xxxx xx0x |xxxx xxxx 
			GPIOB->ODR = 0x00; 
			flipflop = 1;
		}
	}

	return 0;
}

