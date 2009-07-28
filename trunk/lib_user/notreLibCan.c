#include <CAN167.h>
#include <structDonnees.h>


#define MY_BAUD_RATE	1000	/* 1000 kBit/s on the CAN bus */ 
#define IE_BIT		0	/* Interrupts from CAN mod. (CAN side) = no */
#define EIE_BIT		0	/* no error interrupts from the CAN module */ 
#define SIE_BIT		0	/* no status interrupts from the CAN module */

#define MY_XP0IC_VALUE	0x00	/* Interrupts from CAN mod. (CPU side) = no */
#define MY_BAUD_RATE	1000	/* 1000 kBit/s on the CAN bus */ 
#define IE_BIT		0	/* Interrupts from CAN mod. (CAN side) = no */
#define EIE_BIT		0	/* no error interrupts from the CAN module */ 
#define SIE_BIT		0	/* no status interrupts from the CAN module */
#define MO_XTD_BIT	0	/* MO uses 11 bit ID */
#define MO1_ID		0x020	/* 11-bit identifier of Module Object */
#define MO2_ID		0x030	/* 11-bit identifier of Module Object */
#define MO3_ID		0x040	/* 11-bit identifier of Module Object */
#define MO4_ID		0x010	/* 11-bit identifier of Module Object */
#define MO5_ID		0x005	/* 11-bit identifier of Module Object */

#define MO_DLC_ETAT_TROT	sizeof(Etat_Trotinette)
#define MO_DLC_ETAT_RAME	sizeof(char)		
					/* MO1 data length code = length of strcture containing the position and speed of trotinette */
#define MO_TXIE_BIT	0	/* MO1 gen. no transmit interrupts */
#define MO_RXIE_BIT	0	/* MO1 gen. no receive interrupts */


#define BIT_EMETRE   1
#define BIT_RECEVOIR 0

/* Initialization of CAN-Module: (baud_rate, eie, sie, ie)         */
extern void init_can_16x(unsigned int baud_rate, unsigned char eie,
	unsigned char sie, unsigned char ie);

/* Define message object: MO-number, xtd, id, dir, dlc, TXIE, RXIE */	
extern void def_mo_16x(unsigned char nr, unsigned char xtd, unsigned long id,
	unsigned char dir, unsigned char dlc, unsigned char txie,
	unsigned char rxie);

/* load data bytes of a message object (1..14)                     */	
extern void ld_modata_16x(unsigned char nr, unsigned char *upl_data_ptr);
	
/* read data bytes from message object 'nr' (1..14)                */
extern void rd_modata_16x(unsigned char nr, unsigned char *downl_data_ptr);

/* Transmit message object 'nr' (1..14)                            */	
extern void send_mo_16x(unsigned char nr);
	
/* check for new data in MO 'nr' (1..14) and if so, return 1       */
extern unsigned char check_mo_16x(unsigned char nr);
	
/* check for bus off situation and recover if necessary            */
extern unsigned char check_busoff_16x(void);

/* ptr to msg. contrl. regs */
extern unsigned int *msgctrl_ptr_16x[16];

//Bit de direction des différent module object
int Bit_Direction_MO1= BIT_RECEVOIR;
int Bit_Direction_MO2= BIT_RECEVOIR;
int Bit_Direction_MO3= BIT_RECEVOIR;
int Bit_Direction_MO4= BIT_RECEVOIR;
int Bit_Direction_MO5= BIT_RECEVOIR;

int Initialiser_Can(int Num_Station){

	//1 - Test numero valide
	if ((Num_Station >4)|| (Num_Station<1)){
		return -1;
	}
	else
	{
		//1 - Initialisation generale du CAN
		init_can_16x(MY_BAUD_RATE, EIE_BIT, SIE_BIT, IE_BIT);
		
		//1- Configuration des differents modules objects

		//2- Calcul des bits correspondants à l'émission ou la reception 
		//pour les differentes modules objects
		switch (Num_Station){
		 	case 1 : 
				Bit_Direction_MO1=BIT_EMETRE;
				break;
			case 2 : 
				Bit_Direction_MO2=BIT_EMETRE;
				break;
			case 3 : 
				Bit_Direction_MO3=BIT_EMETRE;
				break;
			case 4 : 
				Bit_Direction_MO4=BIT_EMETRE;
				Bit_Direction_MO5=BIT_EMETRE;
				break;
		}
		
		//2- Configuration effective
		//Configuration du module object 1

		
	   	//def_mo_16x(1, MO_XTD_BIT, MO1_ID, Bit_Direction_MO1 , MO_DLC_ETAT_TROT, MO_TXIE_BIT,MO_RXIE_BIT); 
	   	def_mo_16x(1,  //Numero du module object
					MO_XTD_BIT, // Longueur de l'id (11 ou 29 bits)
					MO1_ID, // Identifiant du message observé par l'id
					Bit_Direction_MO1, //Direction du message
					MO_DLC_ETAT_TROT, //Longueur des données échangées
					MO_TXIE_BIT, // Generation d'interruption sur transission
					MO_RXIE_BIT); // Generation d'interruption sur reception
					
		//Configuration du module object 1
	   	def_mo_16x(2,  //Numero du module object
					MO_XTD_BIT, // Longueur de l'id (11 ou 29 bits)
					MO2_ID, // Identifiant du message observé par l'id
					Bit_Direction_MO2, //Direction du message
					MO_DLC_ETAT_TROT, //Longueur des données échangées
					MO_TXIE_BIT, // Generation d'interruption sur transission
					MO_RXIE_BIT // Generation d'interruption sur reception
					);

		//Configuration du module object 1
	   	def_mo_16x(3,  //Numero du module object
					MO_XTD_BIT, // Longueur de l'id (11 ou 29 bits)
					MO3_ID, // Identifiant du message observé par l'id
					Bit_Direction_MO3, //Direction du message
					MO_DLC_ETAT_TROT, //Longueur des données échangées
					MO_TXIE_BIT, // Generation d'interruption sur transission
					MO_RXIE_BIT // Generation d'interruption sur reception
					);

		//Configuration du module object 4
	   	def_mo_16x(4,  //Numero du module object
					MO_XTD_BIT, // Longueur de l'id (11 ou 29 bits)
					MO4_ID, // Identifiant du message observé par l'id
					Bit_Direction_MO4, //Direction du message
					MO_DLC_ETAT_TROT, //Longueur des données échangées
					MO_TXIE_BIT, // Generation d'interruption sur transission
					MO_RXIE_BIT // Generation d'interruption sur reception
			);

		//Configuration du module object 5
	   	def_mo_16x(5,  //Numero du module object
					MO_XTD_BIT, // Longueur de l'id (11 ou 29 bits)
					MO5_ID, // Identifiant du message observé par l'id
					Bit_Direction_MO5, //Direction du message
					MO_DLC_ETAT_RAME, //Longueur des données échangées
					MO_TXIE_BIT, // Generation d'interruption sur transission
					MO_RXIE_BIT // Generation d'interruption sur reception
			);

		return 0;
	}
}


//Envoie le message "Msg_A_Envoyer" sur le réseau. "Num_Station" code pour le
//numéro de station qui réalise l'action.
int Envoyer_Msg(int Num_Station, Etat_Trotinette Msg_A_Envoyer)
{
	//1 - Test numero valide
	if ((Num_Station >4)|| (Num_Station<1)){
		return -1;
	}	
	else{
		//1- Envoie du message
		//2- Chargement dans le module object des données
	 	ld_modata_16x(Num_Station, (char *)&Msg_A_Envoyer);
		//2- Envoie du message
		send_mo_16x(Num_Station);
		return 0;
	}
}

//Lit le message de la station codée par "Num_Station"
//le message est renvoyé à l'adresse Msg_A_Lire
int Lire_Msg(int Num_Station, Etat_Trotinette * Msg_A_Lire)
{
	//1- Lecture sur le module object
	rd_modata_16x(Num_Station,(char *) Msg_A_Lire);
	return 0;
}

//Envoie du mode  : a l'arret ou en marche
void Envoyer_Mode_Rame (char Le_Mode)
{
	//1 - Chargement des données
	ld_modata_16x(5, &Le_Mode);
	//2 - Envoie du message
	send_mo_16x(5);
}

//Envoie du mode  : a l'arret ou en marche
void Lire_Mode_Rame (char * Le_Mode)
{
	rd_modata_16x(5,(char *) Le_Mode);
}

//Note : il aurait fallu utiliser le codage des trotinettes defini dans "structDonnes.h"