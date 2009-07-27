/*
D'après Hugues Malgouyres
Repris par Pierre-Emmanuel Hladik
05/01/09
_______________________________________________________________________________
			Fonction de calcul de la consigne
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
				 |  TpsMontee   						  TpsMontee 
                 |------------------------------------>|
                                  Tstop

Pour que la position finale soit Position_Voulue[m] il faut calculer le 
                                                               Tstop(Vmax,TpsMontee,Position_Voulue)
	          Position_Voulue
	Tstop = __________________   surprenant mais vrai ! indépendant des rampes de montée...
	              Vmax

*/

#include <structDonnees.h>

#define PERIODE_GENERER_TRAJECTOIRE 	20  //période de génération à 20[ms]
#define VMAX  3.3 // vitesse de croisière du métro [m/seconde]
#define TPS_MONTEE 4 //temps de montée/descente à Vmax [s]

// Structure stockant la valeur courante de la consigne
Struct_Consigne Consigne;

//Tableau qui stocke les différentes stations [m]
int Pos_Stations[3]={7000,10000,8500};

//Nombre de pas pour montee en vitesse
int NbPasMontee = TPS_MONTEE*1000.0/PERIODE_GENERER_TRAJECTOIRE;

//Incrément de vitesse par période du générateur pour la rampe
float Delta_V = VMAX/(float)NbPasMontee;

// Calul de la position apres le temps de montee
unsigned int PosApresTempsMontee = VMAX*TPS_MONTEE /(PAS*2);

//Paramètres constants de la trajectoire demandée
int Position_Voulue;  // Nombre de pas souhaite
int Pas_Voulue;  // Nombre de pas souhaite
unsigned int Tstop; // Date de debut de freinage
unsigned int Pas_stop; // Date de debut de freinage	

// Nombre d'increments effectue
unsigned int NbIncrement;

void initTrajectoire(int numero_station)
{
	//Init des variables pour le profil de vitesse
	Consigne.Vitesse=0.0;
	Consigne.Position=0;

	Position_Voulue = (int) Pos_Stations[numero_station]/PAS;
	Tstop = (unsigned int)(Pos_Stations[numero_station]*1000.0)/(VMAX*PERIODE_GENERER_TRAJECTOIRE);
	
	Pas_Voulue = (int) Pos_Stations[numero_station]/PAS;
	Pas_stop = (unsigned int)(Pas_Voulue*1000.0)/(VMAX*PERIODE_GENERER_TRAJECTOIRE);

	NbIncrement = 0;
}

void incrementerConsigne()
{
	if (NbIncrement<=NbPasMontee){
		Consigne.Vitesse = Delta_V*NbIncrement;
		//Calcul de la consigne de position par calcul de l'integrale
		Consigne.Position = (Consigne.Vitesse*NbIncrement*PERIODE_GENERER_TRAJECTOIRE)/(1000.0*PAS*2.0);
	}
	else if (NbIncrement>Pas_Voulue){
		Consigne.Vitesse = 0;
		//Calcul de la consigne de position par calcul de l'integrale
		Consigne.Position = Position_Voulue;
	}
	else if ((NbIncrement>Pas_stop)&&(NbIncrement<=Pas_Voulue)){
		Consigne.Vitesse = VMAX - Delta_V * (NbIncrement-Tstop);
		//Calcul de la consigne de position par la méthode des trapezes
		Consigne.Position = (Position_Voulue -  PosApresTempsMontee) + (Consigne.Vitesse + VMAX) * (NbIncrement-Tstop) * PERIODE_GENERER_TRAJECTOIRE / (2*PAS*1000);								 
	} 
	else{
		 Consigne.Vitesse = VMAX;
		 //Calcul de la consigne de position par calcul de l'integrale
		 Consigne.Position = PosApresTempsMontee + VMAX*(NbIncrement - NbPasMontee)*PERIODE_GENERER_TRAJECTOIRE/(1000.0*PAS);
	}
	NbIncrement ++;
}