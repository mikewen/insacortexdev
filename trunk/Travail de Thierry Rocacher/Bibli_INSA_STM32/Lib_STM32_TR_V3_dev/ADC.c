#include "stm_regs.h" 

//=======================================================================================
// 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure les 2 ADC 12 bits sur le uC 
//  Voir ADC.h pour plus d'info

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
//=======================================================================================


//========================================================================================
// Config de l'ADC
//=======================================================================================

// Idée :
// on configure l'ADC 
//		- en scan (on spécifira les canaux voulus)
// 		- en mode continu
//		- en DMA (on spécifie une adresse de départ et une taille)
//		- on passe la fréquence du periph ADC ck
// 		- on passe la durée d'échantillonnage (tps de fermeture IT)
//		- la fonction renvoie Te période d'échantillonnage
//		- l'ADC est lancé, la DMA est configurée
//
//   float ADC_Init(char:Nb_Canaux, *char :Canaux, *Adr :Start_Adress, float: T_samp_us, float : Freq_ADC_kHz)

// !!!!! ATTENTION, placer les port IO correctement (voir Port_IO.h)


vu16 Tab_Resu_ADC[16]; // Table des résultats, visible par les deux fcts

float ADC_Init(char Nb_Canaux, char Seq_Canaux[], float T_samp_us, float  Freq_ADC_kHz)
// Seq_canaux de 0 à 15 (Rang)
//

{

char Erreur;
float Nb_Cycle;
float Tsample_us;
float Te;
char  SMPx; // pour le champ qui donne Tsample
char i; // indice boucle for
char Rang; // utilise pour fixer l'ordre de séquencement
char i_Reste;


// ATTENTION!!!! il faut activer l'horloge du périph AVANT d'écrire ds les reg de périph
(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_ADC1EN;
// Validation horloge pour DMA
(RCC->AHBENR)=(RCC->AHBENR) | RCC_DMA1EN;

ADC1->CR2= 1; // Réveil de l'ADC


// REGISTRE : ADC_SR : |...STRT JSTRT JEOC EOC AWD|	 (reg d'état)
// STRT : flag qui indique que la conv régulière est partie
// JSTRT : idem mais pour l'injection de canaux
// EOC,JEOC : flag de fin de conversion régulière / injection
// Flag indiquant que la tension analogique a dépassée les limite de l'analog watchdog

// REGISTRE : ADC_CR1 Registre de controle n°1 
ADC1->CR1= 1<<8;
// |Reserved AWDEN JAWDEN Reserved DUALMOD[3:0]   + 
// DISCNUM[2:0] JDISCEN DISCEN JAUTO AWDSGL SCAN JEOCIE AWDIE EOCIE AWDCH[4:0]|
// AWDEN = 0 (validation de l'analog watch dog
// JAWDEN = 0 (idem pour l'injection)
// DUALMOD[3:0]=0000 : ADC indépendant (on en a qu'un !!)
// DISCNUM[2:0]=000 on y écrit le nbre de canaux à traiter en mode trigger (discontinu)
//				pour nous, on travaille en continu, ie, dès qu'une seq est finie, on recommence
// DISCEN=0 : Dicontinuous mode Enable. On met 0 car on veut du continu
// JDISCEN=0 : idem pour injection, 0 n s'en moque
// JAUTO = 0 
// AWDSGL=0
// SCAN=1 : Scan mode On se place en mode scan
// JEOCIE=0: Interrupt enable for injected channels
// AWDIE=0: Analog watchdog interrupt enable
// EOCIE=0: Interrupt enable for EOC (on travaille en DMA donc...)
// AWDCH[4:0]: =00000 wdog, on s'en moque

// REGISTRE : ADC_CR2 Registre de controle n°2 
// |Reserved TSVREFE	SWSTART	JSWSTART EXTTRIG EXTSEL[2:0] Res.  +
//   JEXTTRIG JEXTSEL[2:0] ALIGN Reserved DMA Reserved RSTCAL CAL CONT ADON |
// TSVREFE = 0: Temperature sensor and VREFINT enable
// SWSTART = 0: Start conversion of regular channels (fait partie des sources de trig ext
//				qu'on va invalider, donc...
// JSWSTART = 0: Start conversion of injected channels
// EXTTRIG = 0 : External trigger conversion mode for regular channels Pas de trig ext
/// EXTSEL[2:0]:000 External event select for regular group =0
// JEXTTRIG: External trigger conversion mode for injected channels = 0
// JEXTSEL[2:0]=000
// Bit 11 ALIGN=0: Data alignment = 0 right aligned (16 bits)
// Bit 8 DMA = 1: Direct memory access mode	 Validation de la DMA
// RSTCAL = 0: Reset calibration
// CAL=0: A/D Calibration
// Bit 1 CONT = 1: Continuous conversion  Mode continu
// Bit 0 ADON = 1: A/D converter ON / OFF   Lancement conv	 A PLACER EN FIN D'INIT
//					POUR RESPECTER UN DELAI SUFFISANT (espérons le...)
ADC1->CR2= (ADC1->CR2) |1<<8 | 1<<1; //  / DMA on / Continuous

// Détermination des sampling time (fermeture IT), tous les mêmes !
Erreur=0; // pas d'erreur à priori
SMPx=0;   // à priori
Nb_Cycle=T_samp_us * Freq_ADC_kHz * 0.001; // 0.001 pour passer en MHz
if (Nb_Cycle<=1.5) SMPx=0;
else if (Nb_Cycle<=7.5) SMPx=1;
else if (Nb_Cycle<=13.5) SMPx=2;
else if (Nb_Cycle<=28.5) SMPx=3;
else if (Nb_Cycle<=41.5) SMPx=4;
else if (Nb_Cycle<=55.5) SMPx=5;
else if (Nb_Cycle<=71.5) SMPx=6;
else if (Nb_Cycle<=239.5) SMPx=7;
else Erreur=1;

ADC1->SMPR1=SMPx|SMPx<<3|SMPx<<6|SMPx<<9|SMPx<<12|SMPx<<15|SMPx<<18|SMPx<<21;
ADC1->SMPR2=SMPx|SMPx<<3|SMPx<<6|SMPx<<9|SMPx<<12|SMPx<<15|SMPx<<18|SMPx<<21|SMPx<<24|SMPx<<27;

switch (SMPx)
{
case 0: 
	Tsample_us = 1.5*1000.0/Freq_ADC_kHz; 
 	break;
case 1: 
	Tsample_us = 7.5*1000.0/Freq_ADC_kHz; 
 	break;
case 2: 
	Tsample_us = 13.5*1000.0/Freq_ADC_kHz; 
 	break;
case 3: 
	Tsample_us = 28.5*1000.0/Freq_ADC_kHz; 
 	break;
case 4: 
	Tsample_us = 41.5*1000.0/Freq_ADC_kHz; 
 	break;
case 5: 
	Tsample_us = 55.5*1000.0/Freq_ADC_kHz; 
 	break;
case 6: 
	Tsample_us = 71.5*1000.0/Freq_ADC_kHz; 
 	break;
case 7: 
	Tsample_us = 239.5*1000.0/Freq_ADC_kHz; 
 	break;		  
}
Te=Tsample_us+14.0*1000.0/Freq_ADC_kHz;
if (Erreur==1)
{
Te=0.0;
}

// Organisatio du sequencement
ADC1->SQR3=0;
ADC1->SQR2=0;
ADC1->SQR1=0;
if (Nb_Canaux>15) 
	{
	Erreur=1;
	}
else
	{
	// NB i est le numero de canal. i=0 correspond au canal 1
	for (i=0;i<Nb_Canaux;i++)
		{
		Rang=Seq_Canaux[i];
		if (i<=5) // ecriture ds ADC1_SQR3
			{
			ADC1->SQR3=(ADC1->SQR3) | Rang<<(i*5); // exemple Rang=2 i=1
			}
		else if (i<=11) // ecriture ds ADC1_SQR2
			{
			i_Reste=i % 6; // prise du modulo, ex : i = 7 => i_Reste=1
			ADC1->SQR2=(ADC1->SQR2) | Rang<<(i_Reste*5); // exemple Rang=2 i=1
			}								   // |xxx 00 010x xxxx
		else if (i<=15) // ecriture ds ADC1_SQR2
			{
			i_Reste=i % 6; // prise du modulo, ex : i = 7 => i_Reste=1
			ADC1->SQR1=(ADC1->SQR1) | Rang<<(i_Reste*5); // exemple Rang=2 i=1
			}								   // |xxx 00 010x xxxx
		}
	// affectation du nombre de canaux à traiter
	ADC1->SQR1=(ADC1->SQR1) | (Nb_Canaux-1)<<20; 
	}

ADC1->CR2= ADC1->CR2 | 1; // Start conv

//------- Réglage de DMA -------------------------------------------------------------------

// DMA_CPAR : peripheral adress
DMA1_Channel1->CPAR = ADC1_BASE+0x4C;

// DMA_CMARx : memory adress
DMA1_Channel1->CMAR	= (unsigned long)Tab_Resu_ADC;

// DMA_CNDTRx : nombre de data à transmettre
DMA1_Channel1->CNDTR = Nb_Canaux;


// DMA_CCR1 (DMA ch 1 config Reg)
// Res. |xxx ... MEM2MEM PL[1:0] MSIZE[1:0] PSIZE[1:0] MINC PINC CIRC DIR TEIE HTIE TCIE EN|
// Bits 13:12 PL[1:0]: Channel priority level = 11
// Bits 11:10 MSIZE[1:0]: Memory size  = 01 (16 bits)
// Bits 9:8 PSIZE[1:0]: Peripheral size = 01 (16 bits), 32 bits en fait, mais espérons qu'on prenne que les 16 bas
// Bit 7 MINC: Memory increment mode  =1
// Bit 6 PINC: Peripheral increment mode  =0
// Bit 5 CIRC: Circular mode =1
// Bit 4 DIR: Data transfer direction: lit depuis périph = 0
// Bit 0 EN: Channel enable = 1
DMA1_Channel1->CCR = 0;
// Fixer la priorité
DMA1_Channel1->CCR =(DMA1_Channel1->CCR) |3<<12;
// Fixer la direction, le sens de transfert, circulaire...
DMA1_Channel1->CCR =(DMA1_Channel1->CCR) | 1<<10|1<<8|1<<7|1<<5;
// Enable DMA
DMA1_Channel1->CCR =(DMA1_Channel1->CCR) |1;



return Te;								 												   
}

vu16 ADC_Lire(char Rang_Scan)
{
return Tab_Resu_ADC[Rang_Scan];
}
