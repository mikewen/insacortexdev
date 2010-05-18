#include "../Sys/stm_regs.h" 

//_______________________________________________________________________________________

// 	Auteur T.R.
//  4 Mai 2010
//  Module qui 

//_______________________________________________________________________________________

//========================================================================================
// Validation des lignes d'interruption (hors coeur), 0 à 63
//=======================================================================================

void NVIC_Enable_IT(char NumeroIT)
// 
{
char Numero_Rang;
Numero_Rang = NumeroIT % 32; // modulo
if (NumeroIT < 32)
	{
	NVIC->ISER[0]= (NVIC->ISER[0])|(1<<Numero_Rang);
	}
else if	(NumeroIT < 64)
	{
	NVIC->ISER[1]= (NVIC->ISER[1])|(1<<Numero_Rang);
	}
}

//========================================================================================
// Invalidation des lignes d'interruption (hors coeur), 0 à 63
//=======================================================================================

void NVIC_Disable_IT(char NumeroIT)
{
char Numero_Rang;
Numero_Rang = NumeroIT % 32; // modulo
if (NumeroIT < 32)
	{
	NVIC->ICER[0]= (NVIC->ICER[0])|(1<<Numero_Rang);
	}
else if	(NumeroIT < 64)
	{
	NVIC->ICER[1]= (NVIC->ICER[1])|(1<<Numero_Rang);
	}
}

//========================================================================================
// Fixer la priorité des lignes d'interruption (hors coeur), 0 à 63
//=======================================================================================

void NVIC_Prio_IT(char NumeroIT, char Prio)
{
char Numero_Reg;
char Numero_Rang;

Numero_Reg=NumeroIT/4;
Numero_Rang=NumeroIT%4;
//////!!!!!!!!!prévoir mise à 0 du champ avant masque
NVIC->IPR[Numero_Reg]=(NVIC->IPR[Numero_Reg])&~(0xFF<<(Numero_Rang*8));	
// 	 Exemple : Prio = 5, NumeroIT=18
//   NumeroRang = 2 (voir autre commentaire)
//   0xFF<<16 ce qui donne 00 FF 00 00
//   ~(0xFF<<(Numero_Rang*8) donne donc FF 00 FF FF
//   Le masque donne donc  NVIC->IPR[Numero_Reg] = xx 00 xx xx 
NVIC->IPR[Numero_Reg]=(NVIC->IPR[Numero_Reg])|(Prio<<(Numero_Rang*8+4));
// Exemple : Prio = 5, NumeroIT=18
// NumeroReg = 18/4 = 4
// NumeroRang = 2 (reste de la division par 4.
// C'est donc le 2ieme octet du registre qui est concerné.
// Le rang, en terme de bit est donc Numero_Rang*8.
// De plus la priorité tient sur 4 bits, 4 bits de pds fort de l'octet.
// Donc il faut encore décaler de 4
//   Le masque donne donc  NVIC->IPR[Numero_Reg] = xx Prio 0 xx xx 
}



//========================================================================================
// Fixer la priorité de l'IT coeur Systick
//=======================================================================================

void Systick_Prio_IT(char Prio)
{
SCB->SHPR[2]=(SCB->SHPR[2])& 0x00FFFFFF;
SCB->SHPR[2]=(SCB->SHPR[2])| (Prio<<28);
}

//========================================================================================
// Fixer la periode du Systick et revoie de la valeur effective
//=======================================================================================

float Systick_Period(float Freq_In_MHz, float Duree_us)

// Retourne la duree calculée, 0 si impossible
{
vu32 Nb_Reload;
float Nb_Reload_Float; 
float Duree_reelle_us;

Nb_Reload_Float= Duree_us*Freq_In_MHz;
Nb_Reload=(float)Nb_Reload_Float;

// test de limites
if (Nb_Reload_Float	>=16777215) // 2^24-1 maxi
	 {
	 Nb_Reload=0;
	 }
SysTick->LOAD = Nb_Reload;
Duree_reelle_us=((float)Nb_Reload) / Freq_In_MHz; 
return Duree_reelle_us;
}
