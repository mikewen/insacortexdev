


//=======================================================================================

// 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure l'ADC en mode DMA
//
//  ________ Rappel sur les ressources __________________________________________________
//
//  - 2ADC 16 canaux
//  - Les broches :
//		- ADC_In0  	PA0
//		- ADC_In1	PA1
//		- ADC_In2	PA2
//		- ADC_In3	PA3
//		- ADC_In4	PA4
//		- ADC_In5 	PA5
//		- ADC_In6	PA6
//		- ADC_In7	PA7
//		- ADC_In8	PB0
//		- ADC_In9	PB1
//		- ADC_In10	PC0
//		- ADC_In11	PC1
//		- ADC_In12	PC2
//		- ADC_In13	PC3
//		- ADC_In14	PC4
//		- ADC_In15	PC5
//
// 	-  ADCck Maxi = 14MHz
//
//  ________ Comment utiliser la lib ? __________________________________________________
// 	
// Fonction d'init de l'ADC
// float ADC_Init(char Nb_Canaux, char Seq_Canaux[], unsigned int *Start_Adress, float T_samp_us, float  Freq_ADC_kHz)
//
//	Configuré en DMA, mode continu. Les datas sont alignées à gauche
//  La fonction renvoie la valeur recalculée de la période d'échantillonnage Te
//  Une fois la fonction lancée, l'ADC tourne en boucle. A l'utilisateur de venir
//  Chercher les variables en RAM, en évitant un suréchantillonnage qui fausserait la réalité.
//  ATTENTION !! configurer les ports IO en entrée analogique (cf Port_IO.h)
//
//	- Nb_Canaux : de 1 à 16, c'est le nombre de canaux scannés
//	- Seq_Canaux[] Table qui doit avoir la taille Nb_Canaux.
//		- Seq_Canaux[0] = 0 à 15 c'est le rang du scan pour la voie 0
//		- Seq_Canaux[n] = 0 à 15 c'est le rang du scan pour la voie n
//	- *Start_Adress : pointeur (qui contient l'adresse de la table de résultats)
//					  cette table a la dimension Nb_Canaux
//	- T_samp_us : Temps de fermeture de l'IT (echantillonnage de la capa ADC) en µs
//	- Freq_ADC_kHz : fréquence d'attaque de l'ADC en MHz, maxi 14 MHz
//=======================================================================================



#ifndef _ADC_H__
#define _ADC_H__


float ADC_Init(char Nb_Canaux, char Seq_Canaux[], unsigned int *Start_Adress, float T_samp_us, float  Freq_ADC_kHz);




#endif
