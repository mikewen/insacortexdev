#include "lib_autom_2008a.h"
#include "driveurs_2008a.h"

float Vieux_I_Pos = 0.0;



Un_Etat Lire_Etat_Absolu(void)
{
	Un_Etat Courant;
	Courant.Pos=Lire_Position();
	Courant.Vit=Lire_Vitesse();
	Courant.I_Pos = 666;

   return Courant;
}

Un_Etat Calculer_Etat_Relatif(Un_Etat Etat_Cons)
{
	Un_Etat Courant;
	Courant.Pos= Etat_Cons.Pos - Lire_Position();
	Courant.Vit= Etat_Cons.Vit -  Lire_Vitesse();
	Vieux_I_Pos = (float)(Courant.Pos) * TE + Vieux_I_Pos ;
	if (Vieux_I_Pos > I_Max) Vieux_I_Pos = I_Max;
	if (Vieux_I_Pos < I_Min) Vieux_I_Pos = I_Min;

	Courant.I_Pos = (signed int)(Vieux_I_Pos);
   	return Courant;
}

void Reinit_Etat(int Longueur_Trajet)
{
	Init_Position(Lire_Position()-Longueur_Trajet);
	Vieux_I_Pos = 0.0;
}

