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
int periodeGenerateur ;  // en ms :  période d'échantillonnage du générateur de trajectoire
int tempsMontee ; // en ms : temps de montée/descente à vitesseMaximale
int vitesseMaximale ;  // Vitesse de croisière en pas/s

/**
   Variables de calcul du générateur
**/
unsigned int phase = 1;
unsigned int indexPas ; // nombre de pas de calcul fait par le générateur
unsigned int distanceDuParcours; // en pas : distance du parcours à effectuer
unsigned int indexFinMontee ; // index à partir duquel la montée est terminée
unsigned int indexDebutFreinage; // index à partir duquel il faut commencer à freiner
unsigned int indexFinTrajectoire; // index auquel le generateur doit terminer sa trajectoire
unsigned int incrementVitesse; // Incrément/décrément de vitesse dans les phases de montée et descente
float  positionCourante; // Consigne de position
unsigned int vitesseCourante; // Consigne de vitesse

/**
   Retourne la consigne en position et en vitesse calculée par le générateur
**/
Etat lireConsigne(void)
{	
	Etat consigne;
	consigne.I_Pos = 999;
 	consigne.Pos = (unsigned int) positionCourante;
 	//consigne.Vit = (unsigned int) ((float)vitesseCourante*KD);
	consigne.Vit = (unsigned int) (vitesseCourante);
	return consigne;
}

/**
   Initialisation des paramètres du générateur
**/
void initGenerateur(int periodeGenerateurParam,
					 int tempsMonteeParam,
					 int vitesseMaxParam) {

	periodeGenerateur = periodeGenerateurParam ; // en ms
	tempsMontee = tempsMonteeParam ; // en ms
	vitesseMaximale  = vitesseMaxParam ; // Vitesse de croisière en pas/s
}

/**
 	Initialisation des paramètres pour une trajectoire
**/
void initTrajectoire(unsigned int distanceStation)
{
	
	int indexFinal;

	indexPas = 0;

	distanceDuParcours = distanceStation;

	// Le nombre de pas de calcul en dépend pas des temps de montée !
	indexFinal = (unsigned int)(((long unsigned int)(distanceDuParcours)*1000L)/(long unsigned int)(vitesseMaximale*periodeGenerateur));

	indexFinMontee = ((unsigned int)(tempsMontee/periodeGenerateur));

//	indexFinal = (unsigned int)(((long unsigned int)(distanceDuParcours-(indexFinMontee*periodeGenerateur*vitesseMaximale))*1000L)/(long unsigned int)(vitesseMaximale*periodeGenerateur));

	indexDebutFreinage = indexFinal - indexFinMontee ;	

	indexFinTrajectoire = indexFinal ;
	
	incrementVitesse = vitesseMaximale/indexFinMontee;  //incrément de vitesse par période du générateur pour la rampe
	vitesseCourante=0;
	positionCourante=0.0;

	phase = 0;
}

 /**
   Retourne le numéro du pas de calcul de la trajectoire
**/
unsigned int GetCompteurTrajectoire (void)
{
   return indexPas;
}

 /**
   Retourne l'état du générateur
   		1 : générateur en attente
		0 : générateur en cours de calcul de trajectoire
**/
unsigned int getPhase (void)
{
   return phase;
}

/**
   Calcul le pas suivant de la trajectoire
**/
unsigned char calculConsigneSuivante(void)
{
	unsigned int Vitesse_Precedente=vitesseCourante;
	phase = 0;

	indexPas++;
		
	if (indexPas < indexFinMontee)
	{
		vitesseCourante = vitesseCourante + incrementVitesse;
	}
	else if (indexPas <= indexDebutFreinage)
	{		
		vitesseCourante = vitesseMaximale;
	}
	else if (indexPas <= indexFinTrajectoire)
	{		
		vitesseCourante = vitesseCourante - incrementVitesse;
	}
	else
	{
		vitesseCourante = 0;
		phase = 1;
	
	}
	
	if (phase == 0) 
	{
		positionCourante = positionCourante + (float)((vitesseCourante+Vitesse_Precedente)*periodeGenerateur)/2000.0;
	}
	else 
	{
		positionCourante = distanceDuParcours;
	}

	return phase;			
}



