;/*****************************************************************************/
;/* STM32F10x.s: Startup file for ST STM32F10x device series                  */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2007 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


 PRESERVE8
 THUMB
 
 IMPORT |Lib$$Request$$armlib| ; assure l'initialisation des variables après un reset

 INCLUDE ../objs_lib_baguette/lib_baguette.inc 
            
;______________________________VARIABLES________________________________________________            			
 AREA    RESET, DATA
Phase			DCB 	'Q'	 
Texte_Baguette 	DCB 	"ABBA"
 
;______________________________CONSTANTES_______________________________________            			
 AREA    RESET, DATA, READONLY
S_TB  	equ 	(0xFFF*1000/3300) 	;// Seuil_TresBas  	0x441
S_B  	equ		(0xFFF*1100/3300)	;// Seuil_Bas 		0x5CA 
S_H  	equ 	(0xFFF*2600/3300) 	;// Seuil_Haut 		0xC41
S_TH	equ		(0xFFF*2900/3300) 	;// Seuil_TresHaut 	0xDCA

 
 
  
;______________________________________________________________________________            
 AREA    |.text|, CODE, READONLY, ALIGN=2


;Constantes pour main
AU_MIN  equ 0
MONTE   equ 1
AU_MAX  equ 2
DESCEND equ 3



;_________________________
main	PROC
; IN :  rien
; OUT : rien
; REGS :
;    R4 = vbtn_valid
;    R5 = vbtn_efface
; 	 R6 = caractère
; 	 R7 = index_tableau

 	EXPORT main             	
			BL 	Init_Baguette

			;phase = MONTE
			LDR R0,=Phase
			LDR R1,=MONTE
			STRB R1, [R0]			

			; Arme le chien de garde analogique (accelx<S_TH)
			LDR R0,=S_TH
			BL Analog_Dog_Watch_For_Higher_Than
	
			;index_tableau=0;
			LDR R7,=0

			;MAJ_trame(texte_baguette,trame);

			;MAJ_Ecran (texte_baguette,caractere);
			MOV R6,#'Q'
			LDR R0,=Texte_Baguette
			MOV R1,R6
			BL MAJ_Ecran


			;vbtn_valid = Lire_Touche(BOUTON_VALID);
			LDR R0,=BOUTON_VALIDER
			BL Lire_Touche
			MOV R3,R0

			;vbtn_efface = Lire_Touche(BOUTON_EFFACE);
			LDR R0,=BOUTON_EFFACER
			BL Lire_Touche
			MOV R4,R0

			;index_font = 0;

			;/* Lance le timer pour le clignotement des led */
			BL Demarre_Systick

Pour_Toujours
				
				;/* Recuperation du caractere selectionné apr le potentiometre */
				;potentiometre = Lire_ADC(ADC_POTENTIOMETRE);
				LDR R0,=ADC_POTENTIOMETRE
				BL Lire_ADC
				;caractere = ((potentiometre*26)/0xFFF) + 'A';
				MOV R5,R0 ;TODO

	 			;btn_valid = Lire_Touche(BOUTON_VALID);
				LDR R0,=BOUTON_VALIDER
				BL Lire_Touche
		        
				;btn_efface = Lire_Touche(BOUTON_EFFACE);
				LDR R0,=BOUTON_EFFACER
				BL Lire_Touche


				BL Ecrit_LED

			B Pour_Toujours

 ENDP


;_________________________
SysTick_Handler PROC
	EXPORT SysTick_Handler
	; code de gestion de l'interruption
	BX LR	; LR doit valoir
 ENDP
;_________________________



;_________________________
ADC_IRQHandler PROC
	EXPORT ADC_IRQHandler
	;PUSH {LR}
	BL Acquite_ADC
	LDR LR,=0xFFFFFFF9	
 	BX LR
 ENDP
;_________________________

 END
