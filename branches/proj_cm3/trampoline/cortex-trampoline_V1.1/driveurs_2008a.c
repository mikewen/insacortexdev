// Librairie de gestion des PERIPHs
#include <c167cs.h>
#include "driveurs_2008a.h"


//=================================================================================================
//
//       Commentaires  :   Version 3_1
//      
//
//______________________________________________________________________
// Test effectués:
// init à 10m. Avancée de 9m environ, recul idem => aucun pas raté, l'overflow se passe bien à priori
// init à 10m, avancée de 100m environ, recul idem, aucun problème
// init à 0m, avancée de 170mn recule de 170m, aucun pb.
// 
// NB: la précision est due au pas estimée de la roue ( 0.004254236)  , lié au diamètre...


//___________________ADC___________________________________
//
// Voici la correspondance : [-12km/h  0km/h  +12km/h] = [-3.333.. m/s  0  +3.333..m/s] =>[0  2V5  5V]
// Test	(simu seule)
// Conforme pour les 3 valeurs de l'intervalle 
// La fonction Lire_Vitesse() dure 33us environ, dont 11us pour l'acquisition en scrutation
// et 22 us pour le calcul flottant.


//___________________PWM___________________________________
//
// Sur la voie 7.1 (simu)
// testé en simu. conforme pour des valeurs de -1.0 à +1.0 OK
// testé pour -50.0 et +50.0 (n'importe quoi !) ok, saturation correcte de la PWM
// durée de la fonction Fixe_vitesse(..) : 50us environ (du au calcul flottant vers int)

//__________________LEDS____________________________________
//Bibliotheque de gestion d'une LED pour la prise en main de tiny



//=================================================================================================
// unite Capcom, variables permanente globale à ce fichier______________________
// elles sont figées en RAM
int N_Pulse;




//=================================================================================================
//
//       Les sous-programmes
// 	
//=================================================================================================


void Configurer_Tout_Les_Periphs(void)
{
 	



	//1 Configuration des periphs
	Config_PWM();
	Config_ADConverter();
	Config_Capture();
	Init_Led();
	Init_Position(90.0);
	Fixe_Rapport(0.0);

   	IEN	=1;

}


//______________________________________________________________________________________________
//
//  Config_ADConverter 
//  règle le timing de l'échantllonneur du c167 et protège les voies 6 et 7 du bruit numérique 
//  Ressources utilisées :
// 	 
//   
//______________________________________________________________________________________________

//______________________________________________________________________________________________
//
//  Config_ADConverter 
//  place P5.5 en entrée
// configure l'ADC Tconv #9.7us channel 5
// 	 
//   
//______________________________________________________________________________________________

void Config_ADConverter(void)
{
// P5.5 =>	

// deconnexion des circuits numériques du port P5 (bruit, P5.5)
P5DIDIS=P5DIDIS | 0x0020;

// config de l'ADC:
// Mode simple,CAIN=33pF, Rsource # 1000 => charge OK à 5*RC = 165ns = ts.
// Configuré comme suit, à 20MHz => Tconv = tBC*8 + 40tBC + 2 tCPU 
//										  = 48*tCPU*4 + 2tCPU = 194/20Meg = 9,7 us
//										dont  ts = 8*tBC = 8*4/20M = 1,6us >> 165ns: OK
//

 
// ADCON = 0000 xxxx  xx00 0101
ADCON=ADCON|0x0005;
ADCON=ADCON&0x0FC5;
}


 //______________________________________________________________________________________________
//
// float Lire_Vitesse();
// lit la valeur lue par l'ADC, en float : 
// [-12km/h  0km/h  +12km/h] = [-3.333.. m/s  0  +3.333..m/s] =>[0V  2V5  0V]
// 	 
//   
//______________________________________________________________________________________________

float Lire_Vitesse_Float(void)
{
float Vitesse_Lue;
int Nbre;

// Start
ADST=1;
while(ADBSY)
	{};
Nbre=ADDAT & 0x03FF;
// [0V  5V] =>[0  1023]
Nbre=Nbre-512;
// [0V  5V] =>[-512  +511]

Vitesse_Lue=((float)Nbre)*0.00651042;
// [0V  5V] =>[-3.33sm/s  +3.326m/s] (=[-12km/h + 12km/h])

return(Vitesse_Lue);
}

//______________________________________________________________________________________________
//
// int Lire_Vitesse();
// lit la valeur lue par l'ADC, en float : 
// [-12km/h  0km/h  +12km/h] = [-3.333.. m/s  0  +3.333..m/s] =>[0V  2V5  0V] => [-511 0 512]
// 	 
//   
//______________________________________________________________________________________________

signed int Lire_Vitesse(void)
{

// Start
ADST=1;
while(ADBSY)
	{};

return ((signed int)( ADDAT & 0x03FF) - 511);
}




//______________________________________________________________________________________________
//
//  
//   
//______________________________________________________________________________________________

void Config_Capture(void)
{


T3CON=0x0173; // T3 en mode codeur incremental
DP3 = DP3 & ~(1<<4);   // P3.4 orientée IN
DP3 = DP3 & ~(1<<6);   // P3.6 orientée IN
T3UD=SENS;	// sens de comptage selon les voies A et B
}

signed int Position ;
signed int Lire_Position(void)
{
	Position =  (signed int) T3;
	return Position;
}


void Init_Position(signed int Position_Initiale)
{
	Position = Position_Initiale;
	T3=	 (unsigned int) Position;
}




//______________________________________________________________________________________________

void Config_PWM(void)	 // P7.1 
{
// PWM1 validée, Clk à 20MHz => si on prend un PT1=1024, on est à 20kHz de découpage
// 
// PWMCON0 = xxxx xx0x xx0x xx0x 
// PWMCON1 = xxx0 xxxx xx0x xx1x
PWMCON0=PWMCON0&0xFDDD;
PWMCON1=PWMCON1&0xEFDF;
PWMCON1=PWMCON1|0x0002;

PT1=0;	// le timer à 0
PW1=0;	// au depart, duty = 0
PP1=1024; // période qui donnera environ 20k


/* Configuration PORT7.1 en sortie pushpull	*/	 
// ODP7.1 = 0, 
ODP7=ODP7&0XFFFD;
// DP7.1 =1, bit 1 du port7 en sortie
DP7=DP7|0X0002;

// P7.1 = 1, pour replacer la PWM en phase (inversion par XOR nécessaire)
P7=P7|0X0002;

// demarrage timer PWM1 
PTR1=1;


}

void Fixe_Rapport(float Sigma)
{
	float valeur;
	signed int valeur_entiere;

	valeur = (512.0*Sigma);
	// -512.0 à +511.0
	valeur_entiere = (signed int) valeur;
	valeur_entiere = valeur_entiere+512;
	if (valeur_entiere>1023) valeur_entiere=1023;
	if (valeur_entiere<1) valeur_entiere=1;
	PW1=valeur_entiere;
}

//_________________________________________LEDS__________________
// initLED avant la première utilisation (inite les LEDS éteintes)
// puis  appel de modifierLed1 ou 2 pour changer son état

 void Init_Led(void){
   	DP2=0x00FF;
	ODP2=0x00;
	P2 =   P2 & 0xFF00;
}
void Modifier_Led_1(void){
	P2=P2 ^ 0x01;
}

void Modifier_Led_2(void){
	P2=P2 ^ 0x02;
}	 

void Toggle_Led(unsigned char Num_Led){
	P2=P2 ^ (1<<Num_Led);
}

void Set_Led(unsigned char Num_Led){
	P2=P2 | (1<<Num_Led);
}

void Reset_Led(unsigned char Num_Led){
	P2=P2 & ~(1<<Num_Led);
}
//______________________________________________________________________________________________

