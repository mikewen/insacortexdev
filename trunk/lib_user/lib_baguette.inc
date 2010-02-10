;/*****************************************************************************/
;/* Fichier "include" pour les fonctions de lib_baguette                      */
;/*****************************************************************************/

;/* 
; * Fonction: 	Init_Baguette
; * Role: 		Initialisation de la baguette
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
EXPORT Init_Baguette

;/* 
; * Fonctions pour la gestion des IT 
; */

;/* 
; * Fonction: 	Demarre_SYSTICK
; * Role: 		Lance le timer SYSTICK ( pour le clignotement des led )
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
EXPORT Demarre_SYSTICK

;/* 
; * Fonction: 	Arrete_SYSTICK
; * Role: 		Arrete le timer SYSTICK ( pour le clignotement des led )
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
EXPORT Arrete_SYSTICK

;/* 
; * Fonction: 	Demarre_Timer1
; * Role: 		Lance le timer 1 (pour le rafraichissement de l'interface)
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
EXPORT Demarre_Timer1

;/* 
; * Fonction: 	Arrete_Timer1
; * Role: 		Arrete le timer 1 (pour le rafraichissement de l'interface)
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
EXPORT Arrete_Timer1

;/ * Fonction: 	Acquite_Timer1
; * Role: 		Acquite la drapeau d'interuption du timer 1 
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
EXPORT Acquite_Timer1

;/* 
; * Fonctions pour la gestion de l'ecran LCD 
; */

;/* 
; * Fonction: 	Efface_Ecran
; * Role: 		Efface l'ecran LCD
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
EXPORT Efface_Ecran

;/* 
; * Fonction:	Ecrit_Ecran
; * Role: 		Ecrit un caractere sur l'ecran LCD
; * Entrée: 
; *		R0: Octet de poids faible contient l'octet a ecrire
; * Sortie: 		Rien
; */
EXPORT Ecrit_Ecran

;/* 
; * Fonction:	Ecrit_Chaine_Ecran
; * Role: 		Ecrit d'une chaine de caractere l'ecran LCD
; * Entrée: 
; *		R0: @ de la chaine (la chaine se termine par un caractere NULL (0x00))
; * Sortie: 		Rien
; */
EXPORT Ecrit_Chaine_Ecran

;/* 
; * Fonction: 	Position_Curseur
; * Role: 		Positionne le curseur sur l'ecran LCD
; * Entrée: 
; *      R0: contient l'offset sur une ligne (coordonnée X), entre 0 et 19
; *      R1: contient l'identifiant de ligne (coordonnée Y), entre 0 et 1
; * Sortie: 		Rien
; */
EXPORT Position_Curseur

;/* 
; * Fonctions pour la gestion de l'ADC 
; */

;/* 
; * Definition (EQU) a utiliser avec Lire_ADC
; */
ADC_POTENTIOMETRE 	EQU		0
ADC_X				EQU		1
ADC_Y 				EQU		2

;/* 
; * Fonction: 	Lire_ADC
; * Role: 		Lecture de la valeur d'un canal de l'ADC
; * Entrée: 		
; *		R0:	Canal ADC à lire.
; *			0 = Potentiometre
; *			1 = Accelerometre voie X
; *			2 = Accelerometre voie Y
; * Sortie: 
; *		R0: Valeur ADC (12 bits de poids faible)
; */
EXPORT Lire_ADC
