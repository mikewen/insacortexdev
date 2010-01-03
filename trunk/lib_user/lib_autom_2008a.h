#ifndef _LIB_AUTOM_
#define _LIB_AUTOM_
#include "standard_types.h"	 

typedef struct {
	s32 I_Pos ;
	s32 Pos ;
	s32 Vit ;
} Un_Etat;


#define TE (0.002) // période d'échantillonage du calcul d'intégrale


// Lis l'état des périphs
Un_Etat Lire_Etat_Absolu(void);

Un_Etat Calculer_Etat_Relatif( Un_Etat );
// Lis l'état des périph et mets à jour l'intégrale de Pos
// ATTENTION il faut un appel périodique tout les TE secondes de cette fonction !


void Reinit_Etat(int Longueur_Trajet);

#endif