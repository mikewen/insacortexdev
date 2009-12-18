/*****************************************************************************************
** Titre:NOTRE_LIB_CAN.H				     		                   					**
** Auteurs: Claire DUBOIS, Denis DESMAELE, Sébastien DARDONVILLE, Katia ROULLET 	   	**
** Date:31/10/2007									                   					**
** Sujet:                               												**
******************************************************************************************/

#ifndef __NOTRE_LIB_CAN_H__
#define __NOTRE_LIB_CAN_H__

#include "../../includes/user_libs/struct_donnees_2007a.h"


//Modes de fonctionnement
#define ARRET_EN_STATION			'1'
#define POURSUITE_DE_TRAJECTOIRE	'2'
#define ARRET_URGENCE				'3'

//Message object
#define NB_VOITURES 3
#define IDMO_VOITURE 1
#define IDMO_CONSIGNE 4
#define IDMO_MODE 5

int Initialiser_Can (void);
/*_____________________________________________________________________________
/   ROLE: configure le CAN et les messages objets
/  
______________________________________________________________________________*/

int Envoyer_Msg (Etat_Trottinette);		
/*____________________________________________________________________________________________________________
/   ROLE: envoie un message contenant l'etat mesure de la voiture					  
/  
______________________________________________________________________________________________________________*/

int Envoyer_Consigne (Etat_Trottinette);		
/*____________________________________________________________________________________________________________
/   ROLE: envoie la consigne (position+vitesse) sur le bus				  
/  
____________________________________________________________________________________________________________*/

int Envoyer_Mode (char);
/*____________________________________________________________________________________________________________
/   ROLE: envoie le mode de fonctionnement (arret en station, poursuite de trajectoire, arret d'urgence)
		  sur le bus			  
/  
____________________________________________________________________________________________________________*/

int Lire_Msg(int , Etat_Trottinette *);  
/*____________________________________________________________________________________________________________
/  ROLE: lit l'etat de la voiture num_station. L'état est retourné via son pointeur
_____________________________________________________________________________________________________________*/ 

int Lire_Consigne(Etat_Trottinette *);  
/*____________________________________________________________________________________________________________
/  ROLE: lit la valeur de la consigne (position+vitesse) envoye par le generateur de trajectoire
_____________________________________________________________________________________________________________*/ 

int Lire_Mode(char * Mode);
/*____________________________________________________________________________________________________________
/  ROLE: lit le mode de fonctionnement envoye par le generateur de trajectoire
_____________________________________________________________________________________________________________*/  

void Init_Sniffer(char); 
/*____________________________________________________________________________________________________________
/  ROLE: Initialise le sniffer du CAN. Le parametre mode permet de choisir le type d'animation.
_____________________________________________________________________________________________________________*/  

void Sniffer(char); 
/*____________________________________________________________________________________________________________
/  ROLE: Met a jour l'animation du sniffer avec les nouvelles mesures dans le bon mode.
_____________________________________________________________________________________________________________*/  



#endif