//Hugues Malgouyres
//16 10 2006
//Prise en main du controleur de CAN

//Définir ses clefs dans l'onglet C166 du projet
//ID_VOITURE 0 1 ou 2
//VERBEUX   // active les printf 
//DEBUG     // Active les envois incrémentaux de vitesse et position (pas de périph)


#include <c167cs.h>
#include <intrins.h>
#include <rtx166t.h>
#include <stdio.h>
#include "../../includes/libs/driveurs_2008a.h" 
#include "../../includes/libs/lib_autom_2008a.h" 
#include "../../includes/libs/comm_matlab_2007a.h"

#define NUM_T_SNIFFER		1
#define NUM_T_INIT			0

#define PERIODE_SNIFFER		4  //tours de tourniquet à 1ms
#define PERIODE 2000 //ms
#define CRENEAUX 3 // nombre d eplateaux
#define NB_VOITURES 1

int pTab[PERIODE*CRENEAUX/PERIODE_SNIFFER]	;
int pVit[PERIODE*CRENEAUX/PERIODE_SNIFFER]	;
Un_Etat  Voiture;
 int Val = 0;
void Tache_Sniffer(void) _task_ NUM_T_SNIFFER
{
	unsigned int Creneaux,Longueur;
	int i = 0;
	
	char PosName[4]="Pos";
	char VitName[4]="Vit";

	for (Creneaux=0;Creneaux<CRENEAUX;Creneaux++)
	{
		Fixe_Rapport((float)Val);
		Toggle_Led(2);
		for (Longueur=0;Longueur<(PERIODE/PERIODE_SNIFFER);Longueur++)
		{
			os_wait(K_IVL,PERIODE_SNIFFER,0);

			//1 - Lecture de l'état du système global
			Voiture = Lire_Etat_Absolu();
			pTab[i] = Voiture.Pos; 
			pVit[i++] = Voiture.Vit; 
			#ifdef VERBEUX
			//printf("___________\nCV = %f         CP=%d\n",Voiture.Vit,Voiture.Pos);
			#endif

			//1 - Transmettre état local de la voiture
		}
		if (Val == 1)	Val = -1;
		else Val=1;

	}
	Toggle_Led(5);
	EcrireMatInt (PosName,PERIODE*CRENEAUX/PERIODE_SNIFFER, pTab);
	Toggle_Led(6);
	EcrireMatInt (VitName,PERIODE*CRENEAUX/PERIODE_SNIFFER, pVit);
	Toggle_Led(7);
	while(1);
}

 

void tDemar (void) _task_ NUM_T_INIT
{

	Configurer_Tout_Les_Periphs();
	Init_Led();
	Toggle_Led(1);
	//Initialiser_Can();
	
	//1 - Interruption enabled
	//IEN	=1;

	//1 - Initialisation etat trotinette

	os_create_task (NUM_T_SNIFFER);


	os_delete_task (NUM_T_INIT);
}
