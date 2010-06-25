 PRESERVE8
 THUMB
 IMPORT |Lib$$Request$$armlib| ; assure l'initialisation des variables après un reset
;___________________________________________________________________________________________________
;                  fichier dummy.s servant d'exemple de TD asm 
; voir sujet http://www.lesia.insa-toulouse.fr/~acco/acco_wiki/
;___________________________________________________________________________________________
;  Créé par P. ACC0
; 
;___________________________________________________________________________________________________

;___________________________________________________________________________________________________
;_______________________________CONSTANTES ETIQUETTES_______________________________________________
CHAR_FIN equ 'Z'
TAILLE 	equ  15            
;___________________________________________________________________________________________________
;______________________________VARIABLES initialisées_______________________________________________           			
  AREA    DummyVars, DATA, READWRITE
Caracteres		DCB 	'B','A',"BA",41
Nombre			DCW		0xFFFE,-1,128
Copy_Texte		SPACE   TAILLE
;___________________________________________________________________________________________________
;______________________________CONSTANTES MEMOIRE___________________________________________________            			
				
  AREA     DummyConsts, DATA, READONLY
Texte 			DCB 	"Salut les geeks",CHAR_FIN
Taille_Texte	DCW 	TAILLE  
ID_PUCE			SPACE	15

mountain equ 4
  AREA MesData,  DATA, READWRITE 
        DCB     0xAC,0xDC,-1,128,2_0010
        DCW    12,-2,0xACDC
Tab  SPACE  mountain
FinTab
        DCW    'a'                   ; code ascii de 'a'=0x61


;___________________________________________________________________________________________________
;____________________________CODE___________________________________________________________________           
 AREA    |.textDummy|, CODE, READONLY, ALIGN=2



;_________________________
dummy_asm	PROC
;_________________________
 	EXPORT dummy_asm   
; fonction mystérieuse pour TD assembleur          	
; IN :  rien
; OUT : R0 : somme des caractères
; REGS ALTERES : R0-R1, R6-R8, R15(PC)
;_________________________


;bucephale
;jumper 
;	LDR		R0,	=0xFFFFFFFF
;	ADDS	R0,	R0,	#1
;	ADC		R1,	R0
;
;petittonnerre
;	LDR		R0,	=0x7FFFFFFF
;	ADDS	R0,	R0,	#1
;
;	LDR		R0,	=0xFFFFFFFE
;joly 
;		ADDS		R0,	R0,	#1
;	BEQ		jumper
;	BMI	    petittonnerre
; 	B			joly	
;
;	LDR	R0,	=0x0FFFFFFE
;
;joly1
;;		ADDS		R0, #1
;	BCS		jumper
;	CMP 		R0,	#0x10000000
;	BGT	 bucephale
;	B 			joly1
; B Tot
; ENDP
;Prout PROC
; BX LR
;Tot
; LDR R7,=Caracteres
; BL.W Prout
; B TuosDes
; ENDP

;TuosDes PROC
;  	SUB R14,#6
; BX LR


;     LSRS R0,#1 ; / par 2

;   ITEE CC;  Carry Clear (pair)
;   LSLCC  R0,#1 ; recale
;   LSLCS  R0,#1 
;   ADDCS R0,#1 ; rend pair

;    LSRS R0,#1 ; / par 2
;	LSLCC R0,#1
;	LSLCS R0,#1
;	LSLNE R0,#1
;	ADDCS R0,#1	

;	LSRS R0,#1 ; / par 2
;	LSLCC R0,#1
;	LSLCS R0,#1
;	ADDCS R0,#1	
;	LSLNE R0,#1

;interro ecrite
;qu1
;	MOVS R0,#-1
;	ADD  R0,#1
;	BMI xeu
;   		MOV R0,#3
;xeu
;qu2
;	MOV R2,#0
;boucle
;	MOV R1,#0x40000000
;	ADDS R1,#0x40000000
;	ADD  R2,#1
;	BCC  boucle  

;  LDR R0,=10
;;  LDR R1,=-8
;  ADDS R2,R0,R1
 ; PUSH {R0}
 ; PUSH {R1}
 ; ITTEE	 VS
 ; ;then
 ;    POPVS {R1}
 ;    POPVS {R0}
  ;else
 ;    POPVC {R0}
 ;    POPVC {R1}


	LDR R7,=Caracteres
	LDR	R7,=Nombre
	LDR R6,=Texte
	LDR R8,=Copy_Texte
	LDR R0,=0

|Pas_Fini|

		LDR R1,= CHAR_FIN
		LDRB R7,[R6],#1
		STRB R7,[R8],#1
		ADD R0,R0,R7

	CMP R1,R7
	BNE |Pas_Fini|

	BX LR  ;retour à l'envoyeur

;_________________________
 ENDP  ; dummy_asm
;_________________________



 END ; module asm
;___________________________________________________________________________________________________

 Ici on peut écrire de la pôésie par exemple :-)

