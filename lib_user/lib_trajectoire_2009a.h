/*
________________________________________________________________________________________
	Generation de trajectoire pour le BE Métro
	lib_trajectoire_2009a.c
    GPL licensed code (P.E. Hladik and P. Acco)
________________________________________________________________________________________
USAGE
	inlude lib_trajectoire_2009a.h in your application
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
	version simplifiée en float
	version en s32
___________________________________________________________________________________
  Génère des consignes de position et de vitesse cohérentes selon le profil 
  de vitesse suivant

Vitesse
  /\         Vmax ----> _______________________________
   |                  / I                             I\
   |                 /  I							  I \
   |                /	I						 	  I  \
   |			   /	I							  I   \
   |		  	  /	    I 							  I	   \
   |   __________/	    I							  I     \____________
   |_____________I______I_____________________________I___________________\ temps
                 |       |_                            |<---->|_           /
                 |		   \_						   |		\_
				 |           \_ 					   |		  \_
				 |<----------->|					   |<---------->|
				 |  tempsMontee   						  |      tempsMontee 

La position en fin de trajectoire est celle spécifiée
*/

#ifndef _LIB_TRAJ_
#define _LIB_TRAJ_

#include "standard_types.h"	  

// Type renvoyé par lireConsigne()
typedef struct {
	u16 Pos ;	//Position en pas (attention pas de position négative)
	u16 Vit ;	//Vitesse en pas/s (attention pas de vitesse négative)
} Une_Consigne;


//_________________________________________________________________________________________
void initGenerateur(u16 periodeGenerateurParam, u16 tempsMonteeParam, u16 vitesseMaxParam);
// Initialisation des paramètres du générateur
//
//   periodeGenerateurParam : période en ms de rafraichissement de la valeur de consigne
//   tempsMonteeParam : temps de montée en ms  (tempsMontée du schéma)
//	 vitesseMaxParam : vitesse max en pas/s	  (Vmax du schéma)
// Appelez cette fonction avant toute autre fonction de cette lib
//_________________________________________________________________________________________

//_________________________________________________________________________________________
void initTrajectoire(u16 distanceAParcourir);
// 	Initialisation des paramètres pour le calcul d'une 
//   une trajectoire alant de 0 pas à distance_a_parcourir pas
//  en respectant le profil de vitesse
//  Appelez cette fonction après initGenerateur et avant calculConsigneSuivante
//_________________________________________________________________________________________


//_________________________________________________________________________________________
u8 calculConsigneSuivante(void);
//   Calcul la consigne suivante de la trajectoire
//   ATTENTION   Appelez cette fonction PERIODIQUEMENT
//      toutes les 	"periodeGenerateurParam" ms (voir initGenerateur)
//   Elle met à jour la valeur de consigne qui peut être lue avec lireConsigne()
//   La fonction renvoie l'éat du générateur de trajectoire
//	 	1 : générateur en attente
//		0 : générateur en cours de calcul de trajectoire
// Le premier appel de cette fonction fait passer l'état à 0
// Si la fonction calcule la dernière valeur de consigne (position finale)
// l'état passe à 1
//_________________________________________________________________________________________


//_________________________________________________________________________________________
Une_Consigne lireConsigne(void);
//	Retourne la dernière consigne calculée par calculConsigneSuivantesous
//_________________________________________________________________________________________


//_________________________________________________________________________________________
u16 getCompteurTrajectoire (void);
//   Retourne le numéro du pas de calcul de la trajectoire
//  à utiliser pour Debug normalement
//_________________________________________________________________________________________


//_________________________________________________________________________________________
u8 getPhase (void);
//   Retourne l'état du générateur
//   		1 : générateur en attente
//		0 : générateur en cours de calcul de trajectoire
//_________________________________________________________________________________________

//_________________________________________________________________________________________
#define TRAJECTOIRE_TERMINEE (getPhase())
// vrai lorsque le generateur est à l'arrêt (position 0 avant départ) ou (position finale)
//_________________________________________________________________________________________

//_________________________________________________________________________________________
#define TRAJECTOIRE_EN_COURS (~getPhase())
// vrai lorsque le generateur est en train de piloter la consigne
//_________________________________________________________________________________________




#endif