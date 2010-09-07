#include "../sys/stm_regs.h" 

//=======================================================================================

// 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure les I/O, plus d'info ds le Port_IO.h

//=======================================================================================



char Port_IO_Init( GPIO_TypeDef * Port, char Broche, char Sens, char Techno)
{
vu32 Masque;
char Sens_1_0;
char BrocheH;
char Erreur;

// Activer horloges A, B et C
(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_IOPAEN | RCC_IOPBEN | RCC_IOPCEN;


// Verifications	 générales
// vérification de la cohérence port / nbre de broche
Erreur=0; //pas de pb à priori

if (Port==GPIOA)
	{
	if (Broche>15) 
		{
		Erreur = 1;
		}
	}
else if (Port==GPIOB)
	{
	if (Broche>15) 
		{
		Erreur = 1;
		}
	}
else if (Port==GPIOC)
	{
	if (Broche>13) 
		{
		Erreur = 1;
		}
	}
else
	{
	Erreur =1;
	}


// vérification depassement valeur de techno de 0 à 3
if (Techno>3) Erreur=1;

// verification valeur de Sens

if (Sens != 'i' && Sens != 'I' && Sens != 'o' && Sens != 'O' ) Erreur=1;




//------------------------------------------
// Configuration proprement dite :
//------------------------------------------
// Techno, pull up, pushpull, analog...


// But: fixer la techno de la broche en question 
if (Broche <8)
	{
	// Exemple Broche = 5, Techno = 2
	// il faut arriver à CRL = xxxx xxxx | 10xx xxxx | xxxx xxxx | xxxx xxxx
	Port->CRL=Port->CRL | Techno<<(4*Broche+2); // les bits à 1 sont en place
	// CRL=xxxx xxxx | 1xxx xxxx | xxxx xxxx | xxxx xxxx
	Masque=0 | 3<<(4*Broche+2);
	// Masque = 0000 0000 | 1100 0000 | 0000 0000 | 0000 0000
	Masque=~Masque; // le champ de 2 bit est à 0, le reste à 1
	// Masque = 1111 1111 | 0011 1111 | 1111 1111 | 1111 1111
   	Masque=Masque | Techno<<(4*Broche+2); // tous les bits sont à 1 sauf le ou les 0 du
    // Masque = 1111 1111 | 0011 1111 | 1111 1111 | 1111 1111 OU  0000 0000 | 1000 0000 | 0000 0000 | 0000 0000
	//        = 1111 1111 | 1011 1111 | 1111 1111 | 1111 1111
	//champ concerné
	Port->CRL=Port->CRL & Masque;
	// CRL=xxxx xxxx | 1xxx xxxx | xxxx xxxx | xxxx xxxx
	// 					AND
	// 	   1111 1111 | 1011 1111 | 1111 1111 | 1111 1111
	// =   xxxx xxxx | 10xx xxxx | xxxx xxxx | xxxx xxxx
	}
else
    // idem mais sur le registre CRH
 	{
	BrocheH=Broche-8;
	Port->CRH=Port->CRH | Techno<<(4*BrocheH+2); // les bits à 1 sont en place
	Masque=0 | 3<<(4*BrocheH+2);
	Masque=~Masque;
	Masque=Masque | Techno<<(4*BrocheH+2);
	Port->CRH=Port->CRH & Masque;
  	}

// Sens de la broche (Input ou Output 10MHz, on se prive de la sortie 50MHz...)
if (Sens == 'i' || Sens == 'I') Sens_1_0=0;
if (Sens == 'o' || Sens == 'O') Sens_1_0=1;

if (Broche <8)
	{
	// Mise à 0 de la zone de 2 bits
	Port->CRL=Port->CRL & ~(0x3<<(4*Broche)); 
	// Ecriture du sens par un OU
	Port->CRL=Port->CRL | Sens_1_0<<(4*Broche);  
	}
else
 	{
	// Mise à 0 de la zone de 2 bits
	Port->CRH=Port->CRH & ~(0x3<<(4*BrocheH)); 
	// Ecriture du sens par un OU
	Port->CRH=Port->CRH | Sens_1_0<<(4*BrocheH); 
	}



return Erreur;
}


//========================================================================================
// Mise à 1 des broches
//=======================================================================================


void Port_IO_Set(GPIO_TypeDef * Port, char Broche)
{
// Ecriture à 1

	Port->ODR = (Port->ODR)|(0x01<<Broche);

}

//=======================================================================================
// Mise à 0  des broches
//=======================================================================================
void Port_IO_Clr(GPIO_TypeDef * Port, char Broche)
{
	Port->ODR = (GPIOA->ODR)&~(0x01<<Broche);
}

//========================================================================================
// Lecture des broches
//=======================================================================================
unsigned int Port_IO_Lire(GPIO_TypeDef * Port, char Broche)
{
	unsigned int Etat_Broche;

	Etat_Broche = (Port->IDR)&(0x01<<Broche);	// ex xx1x & 0010 =  0x0010 ou alors  xx0x & 0010 =  0x0000

return Etat_Broche;
}
