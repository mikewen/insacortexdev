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
#include <structDonnees.h> 
#include <../../includes/driveurs_2007a.h> 
#include <stdio.h>


#define NUM_T_ASSERVIR 		1
#define NUM_T_INIT			0

#define PERIODE_ASSERVIR     2  //periode d'asservissement à 2[ms]
#define TE  ((float)PERIODE_ASSERVIR/1000.0) //pareil en float [secondes]
extern void Init_IO_SSC(void);


Etat_Trotinette  Voiture[NB_TROT];
Etat_Trotinette  Consigne;
float I_Err_Pos[NB_TROT]; // pour Intégrale de l'Erreur de Position
float Com_Courant;

// Gains de la loi de commande
#if (ID_VOITURE==0)
float K_Int[NB_TROT]={1.7321/4.0*PAS,0.0,0.0};
float K_Pos[NB_TROT]={1.5729/4.0*PAS,0.0,0.0};
float K_Vit[NB_TROT]={0.4811/4.0,0.0,0.0};
#endif
#if (ID_VOITURE==1)
float K_Int[NB_TROT]={0.0,1.7321/4.0*PAS,0.0};
float K_Pos[NB_TROT]={0.0,1.5729/4.0*PAS,0.0};
float K_Vit[NB_TROT]={0.0,0.4811/4.0,0.0};
#endif
#if (ID_VOITURE==2)
float K_Int[NB_TROT]={0.0,0.0,1.7321/4.0*PAS};
float K_Pos[NB_TROT]={0.0,0.0,1.5729/4.0*PAS};
float K_Vit[NB_TROT]={0.0,0.0,0.4811/4.0};
#endif

void Lecture_Donnes () 
{
 	unsigned char Id_Trot;

	//1 Lecture des données

	//2 - Lecture consigne
	Lire_Msg(NUM_MO_CONTROLEUR, &Consigne);

	#ifdef VERBEUX
	printf("___________\nCV = %f         CP=%d\n",Consigne.Vitesse,Consigne.Position);
	#endif

	//2 - Lecture de l'état des autres voitures
	for (Id_Trot=0;Id_Trot<NB_TROT;Id_Trot++)
	{
		if (Id_Trot != ID_VOITURE)
			{
			Lire_Msg(NUM_MO_TROTINETTE_0+Id_Trot, &Voiture[Id_Trot]);
			#ifdef VERBEUX
			printf("Trotinnette %d : V = %f m/s      P = %d pas\n",
			   Id_Trot,Voiture[Id_Trot].Vitesse,Voiture[Id_Trot].Position);
			#endif
		}
	}

	//2- Lecture de l'état de local
	Voiture[ID_VOITURE].Vitesse = Lire_Vitesse();
	Voiture[ID_VOITURE].Position= Lire_Position();
}

void Calculer_Integrales(void)
{
	unsigned char Id_Trot;

	for (Id_Trot=0;Id_Trot<NB_TROT;Id_Trot++)
	{
		I_Err_Pos[Id_Trot] = I_Err_Pos[Id_Trot] + TE * ((float)(Consigne.Position-Voiture[ID_VOITURE].Position));
		if (I_Err_Pos[Id_Trot]>(1.0/K_Int[Id_Trot])) I_Err_Pos[Id_Trot] = 1.0/K_Int[Id_Trot];
		if (I_Err_Pos[Id_Trot]<(-1.0/K_Int[Id_Trot])) I_Err_Pos[Id_Trot] = -1.0/K_Int[Id_Trot];

	}
}
void Initialiser_Integrales(void)
{
	unsigned char Id_Trot;

	for (Id_Trot=0;Id_Trot<NB_TROT;Id_Trot++)
	{
		I_Err_Pos[Id_Trot] = 0.0;
	}
}

void tAsservir(void) _task_ NUM_T_ASSERVIR
{
	unsigned char Id_Trot,Le_Mode;
	float Com_Int, Com_Pos, Com_Vit;
	bit Y_A_Recallage=0;

	while (1){
		os_wait(K_IVL,PERIODE_ASSERVIR,0);

		Lire_Mode_Rame (&Le_Mode);

		if (Le_Mode==MODE_EN_MARCHE)
		{
			Y_A_Recallage=1;
		
			//1 - Lecture de l'état du système global
			Lecture_Donnes ();
			Calculer_Integrales();

			//1 - Calculer loi de commande
			Com_Int = 0.0;
			Com_Pos = 0.0;
			Com_Vit = 0.0;
			for (Id_Trot=0;Id_Trot<NB_TROT;Id_Trot++)
			{
				//2 - Calculer l'apport de l'erreur d'intégrale
				Com_Int = Com_Int + K_Int[Id_Trot]*I_Err_Pos[Id_Trot];		 	
		
				//2 - Calculer l'apport de l'erreur de position
				Com_Pos = Com_Pos + K_Pos[Id_Trot]*(Consigne.Position-Voiture[Id_Trot].Position);		 	

				//2 - Calculer l'apport de l'erreur de vitesse
				Com_Vit = Com_Vit + K_Vit[Id_Trot]*(Consigne.Vitesse-Voiture[Id_Trot].Vitesse);
			}
					
			//1 - Envoyer Commande
			Com_Courant = (Com_Int + Com_Pos + Com_Vit);
			if (Com_Courant>1.0)  Com_Courant=1.0;
			if (Com_Courant< -1.0) Com_Courant=-1.0;
			Fixe_Courrant(Com_Courant);

			//1 - Transmettre état local de la voiture

			Envoyer_Msg(ID_VOITURE+1, Voiture[ID_VOITURE]);
			#ifdef VERBEUX
			printf("MsgEnvoyé voture %d: V=%f       P=%d\n",
			  ID_VOITURE, Voiture[ID_VOITURE].Vitesse, Voiture[ID_VOITURE].Position);
			#endif
		}
		else  // On est à l'arret
		{
			Fixe_Courrant(0.0);

  		  	Voiture[ID_VOITURE].Vitesse = Lire_Vitesse();
			Voiture[ID_VOITURE].Position= Lire_Position();

			if(Y_A_Recallage){
				Init_Position(Voiture[ID_VOITURE].Position-Consigne.Position);
				Y_A_Recallage=0;
			}
			Initialiser_Integrales();

			Envoyer_Msg(ID_VOITURE+1, Voiture[ID_VOITURE]);	
		}
	}
}
 

void tDemar (void) _task_ NUM_T_INIT
{

	//1 - Configuration des periphs
	Config_ADConverter();
	Config_Capture();
	Config_PWM();
	//2 - Configure l'IT qui prend en compte la position
	// Capcom CC8
	// ILVL=1
	CC8IC = CC8IC | 0x0050;
	CC8IE=1;
	// Capcom CC9
	// ILVL=2
	CC9IC = CC9IC | 0x0060;  
	CC9IE=1;

	Init_Position(0.0);
    Fixe_Courrant(0.0);
	//1 - Interruption enabled
	IEN	=1;

	//1 - Initialisation etat trotinette
	Voiture[ID_VOITURE].Vitesse = 0.0;
	Voiture[ID_VOITURE].Position= 0;
	Initialiser_Integrales();
	
	#ifdef VERBEUX
	printf("INIT voiture %d: V=%f  P=%f\n",ID_VOITURE,Voiture[ID_VOITURE].Vitesse, Voiture[ID_VOITURE].Position);
	#endif


	//1  - Initialisation du can
	Initialiser_Can(ID_VOITURE+1);

	os_create_task (NUM_T_ASSERVIR);
	os_delete_task (NUM_T_INIT);
}
#endif