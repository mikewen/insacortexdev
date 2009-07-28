/*************************************************************************************
** Titre: NOTRE_LIB_CAN.C				     		                   				**
** Auteurs: Claire DUBOIS, Denis DESMAELE, Sébastien DARDONVILLE, Katia ROULLET	    **
** Date:31/10/2007									                   				**
** Sujet:                               											**
**************************************************************************************/


#include "../../includes/user_libs/notre_lib_can_2007a.h"


#define BAUD_RATE  	1000
#define EIE		 	'0'		  // Error Interrupt Enable/Disable
#define SIE			'0'		  // Status Interrupt Enable/Disable
#define IE			'0'		  // Interrupt Line Enable/Disable


extern void init_can_16x (unsigned int , unsigned char ,
	unsigned char, unsigned char );

extern void def_mo_16x(unsigned char , unsigned char , unsigned long ,
	unsigned char , unsigned char , unsigned char ,	unsigned char ) ;

extern void send_mo_16x(unsigned char);

extern void ld_modata_16x(unsigned char, unsigned char *);

extern void rd_modata_16x(unsigned char , unsigned char *);

extern unsigned char check_mo_16x(unsigned char nr);


int Initialiser_Can (void) {

	//Init config CAN
	init_can_16x(BAUD_RATE,EIE,SIE,IE);

#ifdef ID_VOITURE	
#if ID_VOITURE == IDMO_VOITURE
	//Configuration MO1
	def_mo_16x(1,0,0x10,1,6,0,0);
	//Configuration MO2
	def_mo_16x(2,0,0x20,0,6,0,0);
	//Configuration MO3
	def_mo_16x(3,0,0x30,0,6,0,0);
#endif

#if ID_VOITURE == (IDMO_VOITURE+1)
	//Configuration MO1
	def_mo_16x(1,0,0x10,0,6,0,0);
	//Configuration MO2
	def_mo_16x(2,0,0x20,1,6,0,0);
	//Configuration MO3
	def_mo_16x(3,0,0x30,0,6,0,0);
#endif

#if ID_VOITURE == (IDMO_VOITURE+2)
	//Configuration MO1
	def_mo_16x(1,0,0x10,0,6,0,0);
	//Configuration MO2
	def_mo_16x(2,0,0x20,0,6,0,0);
	//Configuration MO3
	def_mo_16x(3,0,0x30,1,6,0,0);
#endif

#endif

#ifdef CONTROLEUR
	//Configuration MO1
	def_mo_16x((IDMO_VOITURE),0,0x10,0,6,0,0);
	//Configuration MO2
	def_mo_16x((IDMO_VOITURE+1),0,0x20,0,6,0,0);
	//Configuration MO3
	def_mo_16x((IDMO_VOITURE+2),0,0x30,0,6,0,0);
	//Configuration MO4
	def_mo_16x(IDMO_CONSIGNE,0,0x05,1,6,0,0);
	//Configuration MO5
	def_mo_16x(IDMO_MODE,0,0x04,1,1,0,0);
#else  //Voiture
	//Configuration MO4
	def_mo_16x(IDMO_CONSIGNE,0,0x05,0,6,0,0);
	//Configuration MO5
	def_mo_16x(IDMO_MODE,0,0x04,0,1,0,0);
#endif



	return 0;

}


int Envoyer_Msg (Etat_Trottinette State_To_Send){
	ld_modata_16x(2,&State_To_Send);
	send_mo_16x(2);
	return 0;
}		

int Envoyer_Consigne (Etat_Trottinette Etat_Consigne){
	ld_modata_16x(4,&Etat_Consigne);
	send_mo_16x(4);
	return 0;
}

int Envoyer_Mode (char Mode){
	ld_modata_16x(5,&Mode);
	send_mo_16x(5);
	return 0;
}


int Lire_Msg(int Num_Station, Etat_Trottinette* Etat_A_Lire){

	//while (check_mo_16x(Num_Station) != 1);//	
	
	rd_modata_16x(Num_Station,Etat_A_Lire);

	return 0;
}


int Lire_Consigne(Etat_Trottinette * Etat_Consigne){ 
	//while (check_mo_16x(4) != 1);//  	
	rd_modata_16x(4,Etat_Consigne);
	return 0;
}

int Lire_Mode(char * Mode){   	
	//while (check_mo_16x(5) != 1);//
	rd_modata_16x(5,Mode);
	return 0;
}

void Init_Sniffer(char mode){
}

void Sniffer(char mode){
}



