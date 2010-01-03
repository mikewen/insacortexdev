#ifndef _LIB_AUTOM_
#define _LIB_AUTOM_


#include "standard_types.h"	 

// CONFIGURE the line below
#define LIB_DRIVEURS "stm_metro_v1.h"


#define TE (0.002) // période d'échantillonage du calcul d'intégrale
#define I_Max (100.0)
#define I_Min (-100.0)

typedef struct {
	s32 Pos ;
	s32 Vit ;
	s32 I_Pos ;
} Etat;


/**
	Initialise l'état en fin d'un cycle
		position <- l'erreur de position relative à la position 
					idéale en fin de cycle "Longueur Trajet"
		vitesse <- 0
		I position <-0	
**/
void reinitEtat(u16 longueurTrajet)	  ;


Etat calculerEtatRelatif(s32 pos, s32 vit)	  ;
// Lis l'état des périph et mets à jour l'intégrale de Pos
// ATTENTION il faut un appel périodique tout les TE secondes de cette fonction !

 float calculerCommande(Etat rame, float gains[3]);

#endif