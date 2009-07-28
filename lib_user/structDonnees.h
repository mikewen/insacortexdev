#ifndef __STRUCTURE_DONNEES_H__
#define __STRUCTURE_DONNEES_H__

#define NB_TROT	3
#define NUM_MO_CONTROLEUR   3
#define NUM_MO_TROTINETTE_0 0
#define NUM_MO_TROTINETTE_1 1
#define NUM_MO_TROTINETTE_2 2

#define MODE_EN_MARCHE 1
#define MODE_A_L_ARRET 0


typedef struct {
	int Position;
	float Vitesse;	
} Etat_Trotinette;


#endif