#ifndef _LIB_AUTOM_
#define _LIB_AUTOM_

#define TE (0.002) // période d'échantillonage du calcul d'intégrale
#define I_Max (1.0)
#define I_Min (-1.0)

typedef struct {
	signed int Pos ;
	signed int Vit ;
	signed int I_Pos ;
} Un_Etat;

// Lis l'état des périphs
Un_Etat Lire_Etat_Absolu(void);

Un_Etat Calculer_Etat_Relatif( Un_Etat );
// Lis l'état des périph et mets à jour l'intégrale de Pos
// ATTENTION il faut un appel périodique tout les TE secondes de cette fonction !


void Reinit_Etat(int Longueur_Trajet);

#endif