/*
_______________________________________________________________________________
			             Générateur de trajectoires
_______________________________________________________________________________

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

*/

#ifndef _LIB_AUTOM_
#define _LIB_AUTOM_



typedef struct {
	signed int Pos ;
	signed int Vit ;
	signed int I_Pos ;
} Etat;

/**
   Initialisation des paramètres du générateur
   	- période en ms
   	- temps de montée en ms
	- vitesse max en pas/s
**/
void initGenerateur(int periodeGenerateurParam, int tempsMonteeParam, int vitesseMaxParam);

/**
 	Initialisation des paramètres pour une trajectoire
		- distance à parcourir en nombre de pas
**/
void initTrajectoire(unsigned int distanceAParcourir);

/**
   Calcul la consigne suivante de la trajectoire
**/
unsigned char calculConsigneSuivante(void);

/**
	Retourne la consigne calculée sous forme d'un état
**/
Etat lireConsigne(void);

/**
   Retourne le numéro du pas de calcul de la trajectoire
**/
unsigned int GetCompteurTrajectoire (void);

 /**
   Retourne l'état du générateur
   		1 : générateur en attente
		0 : générateur en cours de calcul de trajectoire
**/
unsigned int getPhase (void);

#endif