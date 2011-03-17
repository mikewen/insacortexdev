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
#include "lib_autom_2009a.h"
#include LIB_DRIVEURS  


volatile float I_Pos = 0.0;

void reinitEtat(u16 longueurTrajet)
{

	Set_Position(Lire_Position()-(s32)longueurTrajet);
	I_Pos = 0.0;
}

Etat calculerEtatRelatif(s32 pos, s32 vit)
{
	Etat nouv;
	nouv.Pos = pos - Lire_Position();
	nouv.Vit = vit - Lire_Vitesse();
	I_Pos = I_Pos + TE* (float)(nouv.Pos);
	if (I_Pos > I_MAX) I_Pos = I_MAX;
	if (I_Pos < I_MIN) I_Pos = I_MIN;
	nouv.I_Pos = (s32)(I_Pos);
	return nouv;
}

 float calculerCommande(Etat rame, float gains[3])
 {	
 	return rame.I_Pos * gains[0] + rame.Pos * gains[1] + rame.Vit * gains[2] ;	
 }
