// Toutes les routines sont paramétrées par "Num_Station" qui code pour le numéro de 
// la station de la manière suivante :
//1 => trotinette de tete
//2 => trotinette du milieu
//3 => trotinette de queue
//4 => controleur

//Initialise les modules objects
//Renvoie 0 si tout se passe bien, -1 sinon
int Initialiser_Can(int Num_Station);

int Envoyer_Msg (int Num_Station, Etat_Trotinette Msg_A_Envoyer);

int Lire_Msg (int Num_Station, Etat_Trotinette* Msg_A_Envoyer);

void Envoyer_Mode_Rame (char Le_Mode);

void Lire_Mode_Rame (char * Le_Mode);

//Definition des messages et des MO utilisés :
//Etat_Trotinnette0 : MO1
//Etat_Trotinnette1 : MO2
//Etat_Trotinnette2 : MO3
//Consigne : MO4
//Etat_De_la_trame (En_Marche ou A_L_Arret) : MO5