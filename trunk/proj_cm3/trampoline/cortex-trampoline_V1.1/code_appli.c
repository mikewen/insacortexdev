// Hugues Malgouyres
// Modifie par Pierre-Emmanuel Hladik
// 06/01/2009
/***********************************
Fichier type du code pour la consigne
************************************/
#include "tpl_os.h"
#include "tpl_com.h"
#include "tpl_os_generated_configuration.h"


#include "lib_autom_2008a.h"

void InitApp(void)
{
	Init_Mon_Stdio();
	Configurer_Tout_Les_Periphs();
}

float Y = 100.0;
TASK(Generer_Trajectoire)
{ 
	if (Iterer_Trajectoire()==1)
	{
				
	}
	 TerminateTask();

}

TASK(Controler_Voiture)
{
	int pos;
	Un_Etat cons;

	pos = Lire_Position();
	cons= Lire_Consigne();
	Fixe_Rapport((float) ((cons.Pos-pos)>>2));
	TerminateTask();

}

void main (void)
{
	InitApp();
	Configurer_Tout_Les_Periphs();
	Init_Trajectoire(2000);
	StartOS(OSDEFAULTAPPMODE);	
}
