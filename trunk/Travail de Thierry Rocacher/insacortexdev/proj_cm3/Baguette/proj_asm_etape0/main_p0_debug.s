 PRESERVE8
 THUMB
 IMPORT |Lib$$Request$$armlib| ; assure l'initialisation des variables après un reset
;___________________________________________________________________________________________________
;                  fichier main de l'étape 0 du TP asm 
; voir sujet http://www.lesia.insa-toulouse.fr/~acco/acco_wiki/doku.php?id=hard:asm:2009_10:accueil
;___________________________________________________________________________________________________
;  Créé par P. ACC0
; 
; DEBUG ! 
;   Le projet s'assemble bien mais problème en simulé
;   Lors du débug, visualisation sur la sérial window #1 (View-> Serial Wiindows -> UART#1
;   J'aurais voulu voir
;  |Texte: ABBA                            |
;  |Caractère> Q                           |
; 	 et j'obtiens
;  |Texte: ABBA                            |
;  |Caractère>                             |
;
;  TODO fixer le bogue d'affichage du caractère !
;
;   En réel même problème sur l'écran LCD
;___________________________________________________________________________________________________


 INCLUDE ../objs_lib_baguette/lib_baguette.inc  ; déclaration des fonction de la librairie

            
;___________________________________________________________________________________________________
;______________________________VARIABLES initialisées_______________________________________________           			
 AREA    RESET, DATA
Caractere		DCB 	'Q'
Texte_Baguette 	DCB 	"ABBA"

;___________________________________________________________________________________________________
;______________________________CONSTANTES MEMOIRE___________________________________________________            			
 AREA    RESET, DATA, READONLY
  

;___________________________________________________________________________________________________
;_______________________________CONSTANTES ETIQUETTES_______________________________________________



;___________________________________________________________________________________________________
;____________________________CODE___________________________________________________________________           
 AREA    |.text|, CODE, READONLY, ALIGN=2





;_________________________
main	PROC
;_________________________
 	EXPORT main             	
; IN :  rien
; OUT : rien
; REGS :
;    R6 = caractère
;    R0 = pointeur de chaine de 4 lettres à afficher
;  ATTENTION : fonction bloquante
;_________________________



		; Init des periphs à faire avant toute utilisation d'une fonction de lib_baguette
		; voir lib_baguette.inc pour les détails
		BL 	Init_Baguette


|Pour_Toujours|

			;appel de MAJ_Ecran (texte_baguette,caractere);
			LDR R0,=Texte_Baguette
			LDR R6,=Caractere
			MOV R1,R6
			BL MAJ_Ecran ; lib_baguette.inc > REG. ALT. :  R0-R2,R14,R15
			

			B |Pour_Toujours|

;_________________________
 ENDP  ; main
;_________________________



 END ; module asm
;___________________________________________________________________________________________________

 Ici on peut écrire de la pôésie par exemple :-)


