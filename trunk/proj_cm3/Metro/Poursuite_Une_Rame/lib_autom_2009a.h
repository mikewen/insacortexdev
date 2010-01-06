/*
________________________________________________________________________________________
 Petites fonctions de calcul de retour d'état pour le BE 5TRS locomotives 
 	lib_autom_2009a[_TEMPLATE].h
    GPL licensed code (P. Acco)
________________________________________________________________________________________
USAGE

	copy /lib_users/lib_autom_2009a_TEMPLATE.h as ./lib_autom_2009a.h 
	    in your project directory
	configure the local .H

	add include "./lib_autom_2009a.h in your application
________________________________________________________________________________________
REVS
	[PEH 7/12/09] suppression des méthodes qui ne font pas partie
	du générateur de trajectoire 
	[PEH 7/12/09 ]Suppression de la méthode d'approche
	[PAC 3/1/10 ] Passage à des types standards u16 u32 etc...
	[Acco 06/01/2010] finalisation et commentaires de la première version
		Testée en réel et simulé
________________________________________________________________________________________
TODO 
__________________________________________________________________________________
  Calcule l'intégrale de l'erreur de position par rapport à la consigne (variable statique)
  Fournit des fonctions de réinit de l'état de la rame de métro en utilisant
    la librairie de périphériques
  Fournit une fonction de calcul de retour d'état de la voiture

*/

#ifndef _LIB_AUTOM_
#define _LIB_AUTOM_


#include "standard_types.h"	 

// CONFIGURE the line below
#define LIB_DRIVEURS "stm_metro_v1.h"
//Librairie offrant les primitives
// Set_Position(pos), Lire_Position(), Lire_Vitesse()
// RAPPORT_MAX
// RAPPORT_MIN

//CONFIGURE the line below
#define TE (0.002) // période d'échantillonage du calcul de la commande
				// et donc de l'intégration d'erreur de position


// Limitation du windup de l'intégrateur d'erreur de position
// Une bonne valeur est Max_Commande/Max_Ki où Ki est le gain d'intégrale
#define I_MAX (RAPPORT_MAX/100.0) //Limite SUP de l'intégrateur
#define I_MIN (-RAPPORT_MIN/100.0)//Limite SUP de l'intégrateur


// Vecteur d'état relatif d'une rame de métro
typedef struct {
	s32 Pos ;	// Position en pas (signée)
	s32 Vit ;   // Vitesse en pas/s (signée)
	s32 I_Pos ;	// Intégrale de l'érreur de position (pas.s)  (signée)
} Etat;



//_____________________________________________________________
void reinitEtat(u16 longueurTrajet)	  ;
//	Initialise l'état en fin d'une poursuite de trajectoire
//  longueurTrajet : position finale en pas donnée au générateur 
//                    de trajectoire
// cette fonction agit directement sur la librairie 
//      de périphériques
// ATTENTION : appelez cette fonction avant de regénérer la trajectoire
// ATTENTION : Lors de la première trajectoire il convient de faire un 
// Set_Position(longueurTrajet); puis un reinitEtat(longueurTrajet);
// de manière à initialiser la position à 0 comme si une
// trajectoire avait parfaitement été suivie
//______________
//		position <- longueurTrajet - position courante (Lire_Position())
//		vitesse <- 0
//		I position <-0	
//_____________________________________________________________


//_____________________________________________________________
Etat calculerEtatRelatif(s32 pos, s32 vit)	  ;
// pos : valeur de la consigne de position
// vit : valeur de la vitesse de consigne
//
// Lis l'état via les périphs, calcul les erreur de position et vitesse 
//  et l'intégrale de l'erreur de Pos (backward euler)
// ATTENTION il faut un appel périodique tout les TE secondes de 
//   cette fonction !  sinon le calcul est faussé
// La valeur de l'intégrateur est bornée par I_MAX et I_MIN
//_____________________________________________________________


//_____________________________________________________________
 float calculerCommande(Etat rame, float gains[3]);
// rame : un état contenant l'erreur d'état d'une voiture par rapport 
//     à la consigne
// gains : un vecteur des gains de commande {Ki Kp Kv}
// renvoie la valeur de la commande en float
// com = Ki * Err_Integrale_Pos + Kp * Err_Pos + Kv*Err_Vit
//_____________________________________________________________

#endif