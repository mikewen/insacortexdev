#ifndef _COMM_MATLAB_
#define _COMM_MATLAB_
int EcrireMatInt (char *,int , int *);

int Envoi_Matrice_En_Binaire ( 
	char *, // nom de la matrice sous matlab
	int, // nombre d'entier à envoyer
	int * // pointeur sur le tableau d'entier
	)  ;
#endif