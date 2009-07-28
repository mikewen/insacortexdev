//anim.c	Librairie d'animation
#include <intrins.h>
#include <stdio.h>
#include <../../includes/user_libs/structDonnees.h> 
#define LIB_ANIME
#include <../../includes/user_libs/anim.h"

extern void Init_IO_SSC(void); 
extern Etat_Trotinette Consigne;

/* void Attendons (long) -----------FONCTION d'ATTENTE ---------------------
 |
 |		Argument : durée de l'attente.
 |		Retourne : néant.
 |	Sert à règler la vitesse de progression de la chenille.
 -----------------------------------------------------------------------------*/
void Attendons (long Duree)
	{
	long Compte;

	for (Compte = 0; Compte < Duree; Compte++)
		{
		_nop_();	// inclut une instruction NOP (cf. C166 Library Functions)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		}
	}

void Attendre(int);
int EcrireMatInt (char *,int, int *);



/*************************************************************************/
/***************************      FONCTION                   ************/
/***********************************************************************/
/* fonction d'envoie une séquence d'envoie d'entiers sous forme Ascii */
/*********************************************************************/

int EcrireMatInt (char *nomMat,int taille, int *vecteur)
{
	int status ;
	int boukle ; 
 	


	printf("#i\n"); /* Code de début de séquence enregistrement Int pour Matalb */
	scanf("%d",&status);  /*attente de confirmation Matlab */
//	printf("J'ai recu %d\n",status); /* Code de début de séquence enregistrement Int pour Matalb */
    Attendre(1);
	if (status == 0)  /* ok pour matlab */
	{
		printf("%s\n",nomMat);  /* Envoie du nom de la matrice Matlab */
		printf("%d\n",taille);  /* Envoie de la tailel de la matrice Matlab */
		for (boukle = 0 ;  boukle < taille ; boukle++) /*  Envoie de la matrice Matlab */
		{
			printf("%d\n", *(vecteur + boukle) );
			/*if(taille % 10==9){
			 	Attendre(2);
			} */
		}
		scanf("%d",&status);
      	//Attendre(600);
	} 
 	


return status; 
}


/* Fonction d'attente */
/* Insertion de boucle d'attente pour ne pas assurer la synchronisation dans le dialogue */

void Attendre(int Valun)
{
int indice,indice2 ;

for (indice = 0;indice < Valun; indice ++)
		{
	    for (indice2 = 0;indice2 < 0xFFFF; indice2 ++)
			{
			_nop_();
			_nop_();
			_nop_();
			
			}
		} 
}

void EnvoyerEchantillonPosition(int NbreEntiers,int * PositionsVoitures)
{
	EcrireMatInt("Donnees",NbreEntiers,PositionsVoitures);
}

//Renvoie la distance en pas de la prochaine station
int DemanderDistanceProchaineStation(void)
{
	int DistanceStationSuivante;
	printf("!i\n");

	scanf("%d",&DistanceStationSuivante); //Matlab fournit la position de la station suivante en pas
										  //Si 0 il faut quitter l'application
	return (int) (DistanceStationSuivante);
}

