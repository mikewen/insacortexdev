#ifndef _COMM_MATLAB_
#define _COMM_MATLAB_

#include <intrins.h>
#include <stdio.h>


//___________________________________________________
// Fonction d'attente 
// Insertion de boucle d'attente pour ne pas assurer 
// la synchronisation dans le dialogue 

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

/*************************************************************************/
/***************************      FONCTION                   ************/
/***********************************************************************/
/* fonction d'envoie une séquence sous forme binaire */
/*********************************************************************/

int Envoi_Matrice_En_Binaire (char *nomMat,int taille, int *vecteur)
{
	int status ;
	int boukle ;
	char * vec_byte = vecteur; 
 	
	printf("#ib\n"); /* Code de début de séquence enregistrement Int pour Matalb */
	scanf("%d",&status);  /*attente de confirmation Matlab */
	//	printf("J'ai recu %d\n",status); /* Code de début de séquence enregistrement Int pour Matalb */
    Attendre(1);
	if (status == 0)  /* ok pour matlab */
	{
		printf("%s\n",nomMat);  /* Envoie du nom de la matrice Matlab */
		printf("%d\n",taille);  /* Envoie de la tailel de la matrice Matlab */
		for (boukle = 0 ;  boukle < taille ; boukle++) /*  Envoie de la matrice Matlab */
		{
			printf("%s%s", *(vec_byte++),*(vec_byte++) );
			/*if(taille % 10==9){
			 	Attendre(2);
			} */
		}
		scanf("%d",&status);
      	//Attendre(600);
	} 
 	
return status; 
}


#endif