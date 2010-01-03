#include "lib_autom_2009a.h"
#include LIB_DRIVEURS  


volatile float I_Pos = 0.0;

void reinitEtat(u16 longueurTrajet)
{

	Set_Position(Lire_Position()-(s32)longueurTrajet);
	I_Pos = 0.0;
}

Etat calculerEtatRelatif(s32 pos, s32 vit)
{
	Etat nouv;
	nouv.Pos = pos - Lire_Position();
	nouv.Vit = vit - Lire_Vitesse();
	I_Pos = I_Pos + TE* (float)(nouv.Pos);
	if (I_Pos > I_Max) I_Pos = I_Max;
	if (I_Pos < I_Min) I_Pos = I_Min;
	nouv.I_Pos = (s32)(I_Pos);
	return nouv;
}

 float calculerCommande(Etat rame, float gains[3])
 {	
 	return rame.I_Pos * gains[0] + rame.Pos * gains[1] + rame.Vit * gains[2] ;	
 }
