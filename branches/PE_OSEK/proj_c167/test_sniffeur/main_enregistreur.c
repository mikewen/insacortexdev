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
#include <../../includes/user_libs/struct_donnees_2007a.h> 
#include <../../includes/user_libs/notre_lib_can_2007a.h> 
#include <../../includes/libs/driveurs_2007b.h> 

#define NUM_T_SNIFFER		1
#define NUM_T_INIT			0

#define PERIODE_SNIFFER		2  //tours de tourniquet à 1ms


Etat_Trottinette  Voiture[NB_VOITURES];
Etat_Trottinette  Consigne;
float I_Err_Pos[NB_VOITURES]; // pour Intégrale de l'Erreur de Position
float Com_Courant;


void Lecture_Donnes () 
{
 	unsigned char Id_Trot;

	//1 Lecture des données

	//2 - Lecture consigne
	Lire_Msg(IDMO_CONSIGNE, &Consigne);

	#ifdef VERBEUX
	printf("___________\nCV = %f         CP=%d\n",Consigne.Vitesse,Consigne.Position);
	#endif

	//2 - Lecture de l'état des autres voitures
	for (Id_Trot=0;Id_Trot<NB_VOITURES;Id_Trot++)
	{
	
			Lire_Msg(IDMO_VOITURE+Id_Trot, &Voiture[Id_Trot]);
			#ifdef VERBEUX
			printf("Trotinnette %d : V = %f m/s      P = %d pas\n",
			   Id_Trot,Voiture[Id_Trot].Vitesse,Voiture[Id_Trot].Position);
			#endif
	
	}


}


void Tache_Sniffer(void) _task_ NUM_T_SNIFFER
{
	unsigned char Le_Mode;



	while (1){
		os_wait(K_IVL,PERIODE_SNIFFER,0);

		Lire_Mode(&Le_Mode);
		if (Le_Mode==POURSUITE_DE_TRAJECTOIRE)
		{
		
		
			//1 - Lecture de l'état du système global
			Lecture_Donnes ();

			//1 - Transmettre état local de la voiture
		}
		else  // On est à l'arret
		{

		}
	}
}

 

void tDemar (void) _task_ NUM_T_INIT
{

	Configurer_Tout_Les_Periphs();
	Initialiser_Can();
	
	//1 - Interruption enabled
	IEN	=1;

	//1 - Initialisation etat trotinette

	os_create_task (NUM_T_SNIFFER);


	os_delete_task (NUM_T_INIT);
}
