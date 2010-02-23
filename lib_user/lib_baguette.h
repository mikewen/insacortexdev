/*
 * Ecrire un truc ici
 */

#define ADC_POTENTIOMETRE 	0
#define ADC_X 				1
#define ADC_Y 				2

/* 
 * Fonction: 	Init_Baguette
 * Role: 		Initialisation de la baguette
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_Baguette(void);

/* 
 * Fonctions pour la gestion des IT 
 */

/* 
 * Fonction: 	Demarre_SYSTICK
 * Role: 		Lance le timer SYSTICK ( pour le clignotement des led )
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Demarre_SYSTICK(void);

/* 
 * Fonction: 	Arrete_SYSTICK
 * Role: 		Arrete le timer SYSTICK ( pour le clignotement des led )
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Arrete_SYSTICK(void);

/* 
 * Fonction: 	Demarre_Timer1
 * Role: 		Lance le timer 1 (pour le rafraichissement de l'interface)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Demarre_Timer1(void);

/* 
 * Fonction: 	Arrete_Timer1
 * Role: 		Arrete le timer 1 (pour le rafraichissement de l'interface)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Arrete_Timer1(void);

/* 
 * Fonction: 	Acquite_Timer1
 * Role: 		Acquite la drapeau d'interuption du timer 1 
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Acquite_Timer1(void);

/* 
 * Fonctions pour la gestion de l'ecran LCD 
 */

/* 
 * Fonction: 	Efface_Ecran
 * Role: 		Efface l'ecran LCD
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Efface_Ecran(void);

/* 
 * Fonction:	Ecrit_Ecran
 * Role: 		Ecrit un caractere sur l'ecran LCD
 * Entrée: 
 *		R0: Octet de poids faible contient l'octet a ecrire
 * Sortie: 		Rien
 */
void Ecrit_Ecran(char c);

/* 
 * Fonction:	Ecrit_Chaine_Ecran
 * Role: 		Ecrit d'une chaine de caractere l'ecran LCD
 * Entrée: 
 *		R0: @ de la chaine (la chaine se termine par un caractere NULL (0x00)
 * Sortie: 		Rien
 */
void Ecrit_Chaine_Ecran(char *str);

/* 
 * Fonction: 	Position_Curseur
 * Role: 		Positionne le curseur sur l'ecran LCD
 * Entrée: 
 *      R0: contient l'offset sur une ligne (coordonnée X), entre 0 et 19
 *      R1: contient l'identifiant de ligne (coordonnée Y), entre 0 et 1
 * Sortie: 		Rien
 */
void Position_Curseur(int x, int y);

/* 
 * Fonctions pour la gestion de l'ADC 
 */

/* 
 * Fonction: 	Lire_ADC
 * Role: 		Lecture de la valeur d'un canal de l'ADC
 * Entrée: 		
 *		R0:	Canal ADC à lire.
 *			0 = Potentiometre
 *			1 = Accelerometre voie X
 *			2 = Accelerometre voie Y
 * Sortie: 
 *		R0: Valeur ADC (12 bits de poids faible)
 */
int Lire_ADC(int channel);

/* 
 * Fonction: 	Watch_For_Lower_Than
 * Role: 		Regle l'analog watchdog pour surveiller le passage sous un seuil donné
 * Entrée: 		
 *				R0: Valeur 16 bits (LSW) du seuil 
 * Sortie: 		Rien
 */
void Watch_For_Lower_Than(short int gap);

/* 
 * Fonction: 	Watch_For_Higher_Than
 * Role: 		Regle l'analog watchdog pour surveiller le passage au dessus d'un seuil donné
 * Entrée: 		
 *				R0: Valeur 16 bits (LSW) du seuil 
 * Sortie: 		Rien
 */
void Watch_For_Higher_Than(short int gap);

/* 
 * Fonction: 	Acquite_ADC
 * Role: 		Acquite la drapeau d'interuption Analog Watchdog (AWD) de l'ADC
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Acquite_ADC(void);

/* 
 * Fonctions pour la lecture des touches et l'ecriture des LED
 */

/* 
 * Fonction:    Init_LED
 * Role:        Initialisation des leds (PB8 - PB15)
 * Entrée:      Rien
 * Sortie:      Rien
 */
void Init_LED(void);

/* 
 * Fonction:    Ecrit_LED
 * Role:        Ecriture du poids fort des LED (PB8 - PB15)
 * Entrée:              
 *              R0: Valeur a ecrire (octet de poids faible)
 * Sortie:      Rien
 */
void Ecrit_LED(int val);

/* 
 * Fonctions pour la lecture des touches et l'ecriture des LED
 */
#define BOUTON_EFFACE   0x01
#define BOUTON_VALID    0x02

/* 
 * Fonction:    Init_Touche
 * Role:        Initialisation des touches (PA0 (WKUP)-> RAZ, PC13 (TAMP) -> Validation)
 * Entrée:      Rien
 * Sortie:      Rien
 */
void Init_Touche(void);

/* 
 * Fonction:    Lire_Touche
 * Role:        Lecture d'une touche (PC13 (TAMP) -> Validation, PA0 (WKUP) -> RAZ)
 * Entrée:              
 *              R0: Bouton a lire
 *                      1 = Bouton Effacement (BOUTON_EFFACE)
 *                      2 = Bouton Validation (BOUTON_VALID)
 * Sortie: 
 *              R0: Etat du bouton
 *                      0 = Bouton appuyé
 *                      Different de 0 = Bouton relaché
 */
int Lire_Touche(int button);

/*
 * Fonction:    MAJ_Ecran
 * Role:        Rafraichit l'ecran et ecrit le "menu"
 * Entrée:              
 *              R0: Pointeur sur le texte a ecrire sur la baguette (pointeur sur le premier octets de la chaine, zero terminal)
 *				R1: Octet de poids faible = Caractere selectionné par le potentiometre 
 * Sortie:      Rien
 */
void MAJ_Ecran(char texte_baguette[],char caractere);


