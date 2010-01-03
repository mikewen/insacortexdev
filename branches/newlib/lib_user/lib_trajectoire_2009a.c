/*
	Auteur : Pascal Acco
	Date : 15/01/2009
	Description : Librairire de generation d'une trajectoire en rampe
	Modifications : 
		- PEH 7/12/09 suppression des méthodes qui ne font pas partie
	du générateur de trajectoire 
		- PEH 7/12/09 Suppression de la méthode d'approche
*/

#include "lib_trajectoire_2009a.h"
//#include "driveurs_2008a.h"

/**
   Constante du générateur
**/
int tic ;  // en ms :  période d'échantillonnage du générateur de trajectoire
int tM ; // en ms : temps de montée/descente à vMax
int vMax ;  // Vitesse de croisière en pas/s

/**
   Variables de calcul du générateur
**/
u8 phase = 1;
u16 indexPas ; // nombre de pas de calcul fait par le générateur
u16 Dist; // en pas : distance du parcours à effectuer
u16 iFM ; // index à partir duquel la montée est terminée
u16 iDF; // index à partir duquel il faut commencer à freiner
u16 iFin; // index auquel le generateur doit terminer sa trajectoire
u16 dV; // Incrément/décrément de vitesse dans les phases de montée et descente
float  positionCourante; // Consigne de position
float dP ; // coef d'intégration de la position
u16 vitesseCourante; // Consigne de vitesse

/**
   Retourne la consigne en position et en vitesse calculée par le générateur
**/
Etat lireConsigne(void)
{	
	Etat consigne;
	consigne.I_Pos = 999;
 	consigne.Pos = (u16) positionCourante;
	consigne.Vit = (u16) (vitesseCourante);
	return consigne;
}

/**
   Initialisation des paramètres du générateur
**/
void initGenerateur( u16 ticParam,
					 u16 tMParam,
					 u16 vitesseMaxParam) {

	tic = ticParam ; // en ms
	tM = tMParam ; // en ms
	vMax  = vitesseMaxParam ; // Vitesse de croisière en pas/s
}

/**
 	Initialisation des paramètres pour une trajectoire
**/
void initTrajectoire(u16 distanceStation)
{
	
	indexPas = 0;

	Dist = distanceStation;

	// Le nombre de pas de calcul en dépend pas des temps de montée !
//	indexFinal = (unsigned int)(((long unsigned int)(Dist)*1000L)/(long unsigned int)(vMax*tic));

	iFM = (u16)(tM/tic);
	dV = vMax/iFM;  //incrément de vitesse par période du générateur pour la rampe

	iDF = (u16) ( ((u32)Dist * (u32)1000UL) / ((u32)vMax*(u32)tic) );

	iFin = iDF + iFM ;
	dP=((float)tic) / 2000.0;
	
	vitesseCourante=0;
	positionCourante=0.0;

	phase = 0;
}

 /**
   Retourne le numéro du pas de calcul de la trajectoire
**/
u16 GetCompteurTrajectoire (void)
{
   return indexPas;
}

 /**
   Retourne l'état du générateur
   		1 : générateur en attente
		0 : générateur en cours de calcul de trajectoire
**/
u8 getPhase (void)
{
   return phase;
}

/**
   Calcul le pas suivant de la trajectoire
**/
unsigned char calculConsigneSuivante(void)
{
	u16 Vitesse_Precedente=vitesseCourante;
	phase = 0;

	indexPas++;
		
	if (indexPas <= iFM)
	{
		vitesseCourante = vitesseCourante + dV;
	}
	else if (indexPas <= iDF)
	{		
		vitesseCourante = vMax;
	}
	else if (indexPas <= iFin)
	{		
		vitesseCourante = vitesseCourante - dV;
	}
	else
	{
		vitesseCourante = 0;
		phase = 1;
	}
	
	if (phase == 0) 
	{
		positionCourante = positionCourante + (float)(vitesseCourante+Vitesse_Precedente)*dP;
	}
	else 
	{
		positionCourante = Dist;
	}

	return phase;			
}



