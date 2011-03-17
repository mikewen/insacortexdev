;_______________________________________________________________________________
;  Fichier "include" pour les fonctions de lib_baguette  
;
;  La baguette est utilisée pour les TP asm voir la page
;                       
;_______________________________________________________________________________
;
; Résumé des fonctions sous forme de prototypes en langage C
; ===========================================================
;	void Init_Baguette(void);
;
;	int Lire_Touche(int button);
;	#define BOUTON_EFFACE   0x01
;	#define BOUTON_VALID    0x02
;
;	void Ecrit_LED(int val);
;
;   void MAJ_Ecran(char texte_baguette[],char caractere);
;
;	void Demarre_Systick(void);
;
;	int Lire_ADC(int channel);
;	#define ADC_POTENTIOMETRE   0
;	#define ADC_X				1
;	#define ADC_Y 				2
;	void Analog_Dog_Watch_For_Lower_Than(short int gap);
;	void Analog_Dog_Watch_For_Higher_Than(short int gap);
;	void Acquite_ADC(void);
;
; ___ résumé des conventions du compilateur C ARM
;  	     out  ma_function (parm0, param1, param2, ...)
;
;     out est retournée dans R0 (si <= 32bits)  
;     param0 dans R0, param1 dans R1, param2 dans R2 etc.. si param < 32 bits
; 	  param4 et plus sont stockés dans la pile
;
;     R0 à R3, et bien sur R13 à R15, sont potentiellement altérées par l'appel de la fonction 





; ===========================================================
; Déclarations asm et description dans la suite 
; ===========================================================


;________________________________________________________________________________________
; void Init_Baguette(void);
 EXTERN Init_Baguette [CODE]
; IN: 	Rien
; OUT: 	Rien
; REG. ALT. : R0-R3, R14, R15
;________________________________________________________________________________________
;
; Initialise le système et les périphériques liés à la baguette :
;  - initialise les horloges  du coeur et des périphs 
;     + coeur à 40 MHz cadencé par le quartz extérieur (8MHz) multiplié par PLL interne
;	 + low speed périph APB1 à 20MHz, high speed APB2 à 40MHz
;	 + ADC à 5MHz
;  - initialise les périphériques reliés à l'écran LCD de la carte MCBSTM32
;  	 + IO_Ports du PORT C broches 0,1,2,3 et 10,11,12 : noté PC.0-3 et PC.10-12
;  - liaison série 	(utilisée en simulée pour mimer l'écran LCD)
;  	 + USART1 à 9600 bauds en mode POLLING sur broches PA.9 PA.10
;  - ADC pour mesure analogique des accéléros (baguette)  et potentiomètre de saisie (MCBSTM32)
;     + ADC1 en conversion continue de la séquence AIN1 (pot) , AIN14(accèl X), AIN15(accel Y)
;	 + DMA1 channel 1, récupère les conversion ADC et stocke dans buffer tournant en continu
;	 + ADC1 Analog watchdog du canal AIN14 et génération d'interruption sur passage de seuil de AIN14
;  - init timers pour cadencer clignottements mesures etc. par interruptions	et niveau des handlers d'interruption
;  	 + systick timer (core) : période de 800us, timer arrêté, interruption  "SysTick_Handler" de niveau 13
;	 			pour cadencer le clignottement des diodes
;	 + Analog watchdog interruption "ADC_IRQHandler" de niveau 14
;	 			pour repérer les phase d'aller/retour du mouvement de secousse de baguette
;	 + TIM1 (APB2) 	: période 100ms, timer arrêté, interruption "TIM1_UP_IRQHandler" de niveau 15
;	  			pour autres taches si besoin est...
;   - init des IO ports pour allumer les LEDs de la baguette reliées aux leds de la carte MCBSTM32
;     + broche du port B : PB8 à PB15 en sorties push/pull  (diodes MCB allumées par un 1, diodes baguettes éteintes)
;   - init des IO ports pour recevoir l'appuis des bouton poussoirs TAMP et WKUP de MCBSTM32
;     + bouton TAMP utilisé en tant que bouton Valider : PA.1 en entrée	  (reçoit un 0 pour appuyé et 1 sinon)
;	 + bouton WKUP utilisé en tant que bouton Effacer : PC.13 en entrée	  (reçoit un 0 pour appuyé et 1 sinon)
;_______________
;  TOUJOURS appeler cette fonction en début de programme (1 seule fois) 
;     avant d'utiliser les autres fonctions de cette librairie.
;
;________________________________________________________________________________________


;________________________________________________________________________________________
; int Lire_Touche(int button);
 EXTERN Lire_Touche [CODE]
; IN: 
;              R0: Bouton à lire
;                      1 = Bouton Effacement (BOUTON_EFFACER)
;                      2 = Bouton Validation (BOUTON_VALIDER)
; OUT: 	Rien
;              R0: Etat du bouton
;                      0 = Bouton appuyé
;                      Different de 0 = Bouton relaché
; REG. ALT. : R0-R1, R14, R15
;______________________
BOUTON_EFFACER equ 1
BOUTON_VALIDER equ 2
;________________________________________________________________________________________
;  Lecture de l'état d'une touche de la carte MCBSTM32 
;   PC.13 (TAMP) -> Validation 
;   PA.0  (WKUP) -> RAZ 
;  Attention renvoie 0 pour "bouton appuyé" (cablâge inverse sur MCBSTM32)
;________________________________________________________________________________________



;________________________________________________________________________________________
;void Ecrit_LED(int val);
 EXTERN Ecrit_LED [CODE]
; IN: 
;     R0.0-R0.7: Valeur à envoyer sur les diodes du port PB.8 à PB.15
; OUT: 	Rien
; REG. ALT. : R1-R3, R14, R15
;________________________________________________________________________________________
;   Copy en octet en sortie sur port PB.8-PB.15
;      ATTENTION, un 1 logique allume les diodes de MCBSTM32 
;                              mais etteind celles de la baguette  (montage inverse)
;________________________________________________________________________________________


;________________________________________________________________________________________
;void MAJ_Ecran(char texte_baguette[],char caractere);
 EXTERN MAJ_Ecran [CODE]
; IN: 
;     R0: @ des 4 caractères saisis par l'utilisateur
; 	  R1.0-R1.7: le caractère en cours de saisi 
; OUT: 	Rien
; REG. ALT. :  R0-R2,R14,R15
; ATTENTION : temps d'exécution très long car envoie vers LCD et liason série à 9600 bauds
;    soit 960 caractères par seconde
;
;  L'exécution de cette procédure dure environ 200ms en simulé ! 
;________________________________________________________________________________________
;	 Rafraichit l'ecran LCD et envoie l'équivalent sur la liaison série
;  Le texte affiché est sur 2 lignes :
;   _______________________________________
;  |Texte: wxyz                            |
;  |Caractère> c                           |
;   =======================================
;  où wxyz sont les 4 caractères pointé par R0
;     c le caractère contenu dans R1.0-R1.7
;____DEBUG
;  En simulé, l'écran LCD n'est pas simulé mais on peut observer l'UART1
;  Allez en debug dans l'onglet View->Serial Windows->UART1
;____ REEL 
;  Observez simplement l'écran LCD, ou connectez un terminal sur UART1 avec un 
;       connecteur DB9 sur la carte MCBSTM32
;________________________________________________________________________________________



;________________________________________________________________________________________
; void Demarre_Systick(void)
 EXTERN Demarre_Systick
; IN:  rien
; OUT: 	Rien
; REG. ALT. : R0,R1,R14,R15
;________________________________________________________________________________________
;  Lance le timer SYSTICK ( pour le clignotement des led ) de période 800us
;    à partir de cet instant une interruption sera déclanchée toute les 800us
;  Il faut donc définir un Handler pour cette interruption :
;  SysTick_Handler PROC
;	EXPORT SysTick_Handler
;	; code de gestion de l'interruption
;	BX LR  
;   ENDP
;________________________________________________________________________________________


;________________________________________________________________________________________
; int Lire_ADC(int channel);
 EXTERN Lire_ADC
; IN: 	 R0: canal à lire         
;          		0 = valeur du potentiomètre de saisie (ADC_POTENTIOMETRE)
;				1 = mesure de l'accélération de l'axe X (ADC_X) l'axe est indiqué sur la baguette.
;				2 = mesure de l'accélération de l'axe Y (ADC_Y) l'axe est indiqué sur la baguette.
; OUT: 	 R0: Résultat de la conversion sur 16 bits alignés à droite
; 
; REG. ALT. : R0-R2,R14,R15
;______________________
ADC_POTENTIOMETRE 	EQU		0
ADC_X				EQU		1
ADC_Y 				EQU		2
;________________________________________________________________________________________
;  Lis le résultat de la conversion analogique effectué par l'ADC1 et stocké par la DMA
;  Le résultat est stocké en 16 bits alignés à droite, la pleine échelle est de 3.3 Volts
;  0x?????000  => 0 Volts mesurés sur le canal indiqué
;  0x?????FFF  => 3.3 Votls mesurés sur le canal indiqué
;________________________________________________________________________________________



;________________________________________________________________________________________
;void Analog_Dog_Watch_For_Higher_Than(short int gap);
;void Analog_Dog_Watch_For_Lower_Than(short int gap);
 EXTERN Analog_Dog_Watch_For_Higher_Than [CODE]
 EXTERN Analog_Dog_Watch_For_Lower_Than [CODE]
; IN    R0.0-R0.11 : "gap" valeur du seuil sur 12 bits 
; OUT 	rien
; REG. ALT. : R0-R2, R14, R15
;________________________________________________________________________________________
;  Indique au watchdog analogique de déclencher une interruption si
;      l'accélération en x (ADC_X) devient inférieure à "gap" pour "..For_Lower_Than"
;      l'accélération en x (ADC_X) devient supérieure à "gap" pour "..For_Higwer_Than"
;  L'appel de cette fonction déclenchera donc une interruption à récupérer en exportant
; me handler "ADC_IRQhandler comme dans l'exemple suivant 
;_________________________exemple de handler
;ADC_IRQHandler PROC
;	EXPORT ADC_IRQHandler
;
;       ; code à exécuter sur aboiment de l'analog watchdog
;
;	  BL Acquite_ADC		; acquittement sinon l'interruption reste toujours active !
;
;     ; retour d'interruption 
;	  LDR LR,=0xFFFFFFF9  (au cas ou LR ai été modifié par le code)	
; 	  BX LR
; ENDP
;_________________________
; ATTENTION appelez Acquite_ADC en fin de handler (voir Acquite_ADC)
;
;   L'analog watchdog ne peut surveiller qu'un seul seuil à la foi !
;   impossible de surveiller si l'on dépasse un certain seuil ET en même temps
;   si l'on passe sous un seuil...
;________________________________________________________________________________________

;________________________________________________________________________________________
; void Acquite_ADC(void);
 EXTERN Acquite_ADC [CODE]
; IN:  rien
; OUT: 	Rien
; REG. ALT. : R0,R1,R14,R15
;________________________________________________________________________________________
;	Acquite le drapeau d'interruption de l'Analog Watchdog (AWD) de l'ADC
;   Si ce n'est pas fait l'interruption reste active : le retour d'IT ira 
;         encore dans le handler de l'adc => répétition infinie de l'IT...
;________________________________________________________________________________________



;//////////////////////////////////////////////////////////////////////////////////////////
; LES FONCTIONS SUIVANTES NE SONT PAS UTILES AU PROJET NORMALEMENT
;/////////////////////////////////////////////////////////////////////////////////////////
; Prototypes C
; 	void Arrete_Systick(void);
; Utiliser le timer 1 qui est cadencé à 100ms
; 	void Demarre_Timer1(void);
; 	void Arrete_Timer1(void);
; 	void Acquite_Timer1(void);
; Primitives de l'écran LCD/USART1
; 	void Efface_Ecran(void);
; 	void Ecrit_Ecran(char c);
; 	void Ecrit_Chaine_Ecran(char *str);
; 	void Position_Curseur(int x, int y);


;/////////////////
; déclaration asm

;/* 
; * Fonction: 	Arrete_SYSTICK
; * Role: 		Arrete le timer SYSTICK ( pour le clignotement des led )
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
 EXTERN Arrete_Systick

;/* 
; * Fonction: 	Demarre_Timer1
; * Role: 		Lance le timer 1 (pour le rafraichissement de l'interface)
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
 EXTERN Demarre_Timer1

;/* 
; * Fonction: 	Arrete_Timer1
; * Role: 		Arrete le timer 1 (pour le rafraichissement de l'interface)
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
 EXTERN Arrete_Timer1

;/ * Fonction: 	Acquite_Timer1
; * Role: 		Acquite la drapeau d'interuption du timer 1 
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
 EXTERN Acquite_Timer1

;/* 
; * Fonctions pour la gestion de l'ecran LCD 
; */



;/* 
; * Fonction: 	Efface_Ecran
; * Role: 		Efface l'ecran LCD
; * Entrée: 		Rien
; * Sortie: 		Rien
; */
 EXTERN Efface_Ecran

;/* 
; * Fonction:	Ecrit_Ecran
; * Role: 		Ecrit un caractere sur l'ecran LCD
; * Entrée: 
; *		R0: Octet de poids faible contient l'octet a ecrire
; * Sortie: 		Rien
; */
 EXTERN Ecrit_Ecran

;/* 
; * Fonction:	Ecrit_Chaine_Ecran
; * Role: 		Ecrit d'une chaine de caractere l'ecran LCD
; * Entrée: 
; *		R0: @ de la chaine (la chaine se termine par un caractere NULL (0x00))
; * Sortie: 		Rien
; */
 EXTERN Ecrit_Chaine_Ecran

;/* 
; * Fonction: 	Position_Curseur
; * Role: 		Positionne le curseur sur l'ecran LCD
; * Entrée: 
; *      R0: contient l'offset sur une ligne (coordonnée X), entre 0 et 19
; *      R1: contient l'identifiant de ligne (coordonnée Y), entre 0 et 1
; * Sortie: 		Rien
; */
 EXTERN Position_Curseur


 END
