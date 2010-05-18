#include "../Sys/stm_regs.h" 

//=======================================================================================

// 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure les I/O, plus d'info ds le Port_IO.h

//=======================================================================================



char Port_IO_Init(char Port, char Broche, char Sens, char Techno)
// Doc tech : STM32F103RM.pdf
// Port : Char A à C 
// Broche : de 0 à 15 pour GPIOA, GPIOB, 
//		  : de 0 à 13 pour GPIOC 
// Sens   : I pour entree, O pour sortie

// Techno : 0 Analog						  / Output push-pull
//			1 Input floating				  / Output open-drain
//			2 Input-pull-down / up			  / Alternate function push-pull
//			3 reserved						  / Alternate function open-drain
// 
// Retourne 0 si tout est OK, 1 sinon


{
vu32 GPIOx_CRL;
vu32 GPIOx_CRH;
vu32 Masque;
char Sens_1_0;
char BrocheH;
char Erreur;

// Activer horloges A, B et C
(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_IOPAEN | RCC_IOPBEN | RCC_IOPCEN;


// Verifications	 générales
// vérification de la cohérence port / nbre de broche
Erreur=0; //pas de pb à priori

if (Port=='A' || Port=='a')
	{
	if (Broche>15) 
		{
		Erreur = 1;
		}
	}
else if (Port=='B' || Port=='b')
	{
	if (Broche>15) 
		{
		Erreur = 1;
		}
	}
else if (Port=='C' || Port=='c')
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

//verification que la "combinaison reservée" n'est pas demandée
if ((Sens == 'i' || Sens =='I') && Techno == 3) Erreur=1;

// vérification depassement valeur de techno de 0 à 3
if (Techno>3) Erreur=1;

// verification valeur de Sens

if (Sens != 'i' && Sens != 'I' && Sens != 'o' && Sens != 'O' ) Erreur=1;

//------------------------------------------
// Configuration proprement dite :
//------------------------------------------
// Techno, pull up, pushpull, analog...

// Lecture du registre concerné registre
switch(Port)
{
case 'A' :
	GPIOx_CRL = GPIOA->CRL;
	GPIOx_CRH = GPIOA->CRH;
	break;
case 'a' :
	GPIOx_CRL = GPIOA->CRL;
	GPIOx_CRH = GPIOA->CRH;
	break;
case 'B' :
	GPIOx_CRL = GPIOB->CRL;
	GPIOx_CRH = GPIOB->CRH;
	break;
case 'b' :
	GPIOx_CRL = GPIOB->CRL;
	GPIOx_CRH = GPIOB->CRH;
	break;
case 'C' :
	GPIOx_CRL = GPIOC->CRL;
	GPIOx_CRH = GPIOC->CRH;
	break;
case 'c' :
	GPIOx_CRL = GPIOC->CRL;
	GPIOx_CRH = GPIOB->CRH;
	break;

}


// But: fixer la techno de la broche en question 
if (Broche <8)
	{
	// Exemple Broche = 5, Techno = 2
	// il faut arriver à CRL = xxxx xxxx | 10xx xxxx | xxxx xxxx | xxxx xxxx
	GPIOx_CRL=GPIOx_CRL | Techno<<(4*Broche+2); // les bits à 1 sont en place
	// CRL=xxxx xxxx | 1xxx xxxx | xxxx xxxx | xxxx xxxx
	Masque=0 | 3<<(4*Broche+2);
	// Masque = 0000 0000 | 1100 0000 | 0000 0000 | 0000 0000
	Masque=~Masque; // le champ de 2 bit est à 0, le reste à 1
	// Masque = 1111 1111 | 0011 1111 | 1111 1111 | 1111 1111
   	Masque=Masque | Techno<<(4*Broche+2); // tous les bits sont à 1 sauf le ou les 0 du
    // Masque = 1111 1111 | 0011 1111 | 1111 1111 | 1111 1111 OU  0000 0000 | 1000 0000 | 0000 0000 | 0000 0000
	//        = 1111 1111 | 1011 1111 | 1111 1111 | 1111 1111
	//champ concerné
	GPIOx_CRL=GPIOx_CRL & Masque;
	// CRL=xxxx xxxx | 1xxx xxxx | xxxx xxxx | xxxx xxxx
	// 					AND
	// 	   1111 1111 | 1011 1111 | 1111 1111 | 1111 1111
	// =   xxxx xxxx | 10xx xxxx | xxxx xxxx | xxxx xxxx
	}
else
    // idem mais sur le registre CRH
 	{
	BrocheH=Broche-8;
	GPIOx_CRH=GPIOx_CRH | Techno<<(4*BrocheH+2); // les bits à 1 sont en place
	Masque=0 | 3<<(4*BrocheH+2);
	Masque=~Masque;
	Masque=Masque | Techno<<(4*BrocheH+2);
	GPIOx_CRH=GPIOx_CRH & Masque;
  	}

// Sens de la broche (Input ou Output 10MHz, on se prive de la sortie 50MHz...)
if (Sens == 'i' || Sens == 'I') Sens_1_0=0;
if (Sens == 'o' || Sens == 'O') Sens_1_0=1;

if (Broche <8)
	{
	// Mise à 0 de la zone de 2 bits
	GPIOx_CRL=GPIOx_CRL & ~(0x3<<(4*Broche)); 
	// Ecriture du sens par un OU
	GPIOx_CRL=GPIOx_CRL | Sens_1_0<<(4*Broche);  
	}
else
 	{
	// Mise à 0 de la zone de 2 bits
	GPIOx_CRH=GPIOx_CRH & ~(0x3<<(4*BrocheH)); 
	// Ecriture du sens par un OU
	GPIOx_CRH=GPIOx_CRH | Sens_1_0<<(4*BrocheH); 
	}

// Affectation au registre
switch(Port)
{
case 'A' :
	GPIOA->CRL = GPIOx_CRL;
	GPIOA->CRH = GPIOx_CRH;
	break;
case 'a' :
	GPIOA->CRL = GPIOx_CRL;
	GPIOA->CRH = GPIOx_CRH;
	break;
case 'B' :
	GPIOB->CRL = GPIOx_CRL;
	GPIOB->CRH = GPIOx_CRH;
	break;
case 'b' :
	GPIOB->CRL = GPIOx_CRL;
	GPIOB->CRH = GPIOx_CRH;
	break;
case 'C' :
	GPIOC->CRL = GPIOx_CRL;
	GPIOC->CRH = GPIOx_CRH;
	break;
case 'c' :
	GPIOC->CRL = GPIOx_CRL;
	GPIOC->CRH = GPIOx_CRH;
	break;

}

return Erreur;
}


//========================================================================================
// Mise à 1 des broches
//=======================================================================================


void Port_IO_Set(char Port, char Broche)
{
// Ecriture à 1
switch(Port)
{
case 'A' :
	GPIOA->ODR = (GPIOA->ODR)|(0x01<<Broche);
	break;
case 'a' :
	GPIOA->ODR = (GPIOA->ODR)|(0x01<<Broche);
	break;
case 'B' :
	GPIOB->ODR = (GPIOB->ODR)|(0x01<<Broche);
	break;
case 'b' :
	GPIOB->ODR = (GPIOB->ODR)|(0x01<<Broche);
	break;
case 'C' :
	GPIOC->ODR = (GPIOC->ODR)|(0x01<<Broche);
	break;
case 'c' :
	GPIOC->ODR = (GPIOC->ODR)|(0x01<<Broche);
	break;

}

}
//=======================================================================================
// Mise à 0  des broches
//=======================================================================================
void Port_IO_Clr(char Port, char Broche)
{
// Ecriture à 1
switch(Port)
{
case 'A' :
	GPIOA->ODR = (GPIOA->ODR)&~(0x01<<Broche);
	break;
case 'a' :
	GPIOA->ODR = (GPIOA->ODR)&~(0x01<<Broche);
	break;
case 'B' :
	GPIOB->ODR = (GPIOB->ODR)&~(0x01<<Broche);
	break;
case 'b' :
	GPIOB->ODR = (GPIOB->ODR)&~(0x01<<Broche);
	break;
case 'C' :
	GPIOC->ODR = (GPIOC->ODR)&~(0x01<<Broche);
	break;
case 'c' :
	GPIOC->ODR = (GPIOC->ODR)&~(0x01<<Broche);
	break;
}
}
//========================================================================================
// Lecture des broches
//=======================================================================================
unsigned int Port_IO_Lire(char Port, char Broche)
{
unsigned int Etat_Broche;

// lecture
switch(Port)
{
case 'A' :
	Etat_Broche = (GPIOA->IDR)&(0x01<<Broche);	// ex xx1x & 0010 =  0x0010 ou alors  xx0x & 0010 =  0x0000
	break;
case 'a' :
	Etat_Broche = (GPIOA->IDR)&(0x01<<Broche);	
	break;
case 'B' :
	Etat_Broche = (GPIOB->IDR)&(0x01<<Broche);	
	break;
case 'b' :
	Etat_Broche = (GPIOB->IDR)&(0x01<<Broche);	
	break;
case 'C' :
	Etat_Broche = (GPIOC->IDR)&(0x01<<Broche);	
	break;
case 'c' :
	Etat_Broche = (GPIOC->IDR)&(0x01<<Broche);	
	break;
}
return Etat_Broche;
}
