 PRESERVE8
 THUMB
 IMPORT |Lib$$Request$$armlib| ; assure l'initialisation des variables apr�s un reset
;___________________________________________________________________________________________________
;                  fichier dummy.s servant d'exemple de TD asm 
; voir sujet http://www.lesia.insa-toulouse.fr/~acco/acco_wiki/
;___________________________________________________________________________________________
;  Cr�� par P. ACC0
; 
;___________________________________________________________________________________________________

;___________________________________________________________________________________________________
;_______________________________CONSTANTES ETIQUETTES_______________________________________________
CHAR_FIN equ 'Z'
TAILLE 	equ  15            
;___________________________________________________________________________________________________
;______________________________VARIABLES initialis�es_______________________________________________           			
 AREA    RESET, DATA
Caracteres		DCB 	'B','A',"BA",41
Nombre			DCW		0xFFFE,-1,128
Copy_Texte		SPACE   TAILLE
;___________________________________________________________________________________________________
;______________________________CONSTANTES MEMOIRE___________________________________________________            			
 AREA     DATA, READONLY
Texte 			DCB 	"Salut les geeks",CHAR_FIN
Taille_Texte	DCW 	TAILLE  
ID_PUCE			SPACE	15



;___________________________________________________________________________________________________
;____________________________CODE___________________________________________________________________           
 AREA    |.text|, CODE, READONLY, ALIGN=2





;_________________________
dummy_asm	PROC
;_________________________
 	EXPORT dummy_asm             	
; IN :  rien
; OUT : rien
; REGS :
;_________________________

	LDR R6,=Texte

|Pas_Fini|

		LDR R1,= CHAR_FIN
		LDRB R7,[R6],#1
	CMP R1,R7
	BNE |Pas_Fini|

	BX LR

;_________________________
 ENDP  ; main
;_________________________



 END ; module asm
;___________________________________________________________________________________________________

 Ici on peut �crire de la p��sie par exemple :-)

