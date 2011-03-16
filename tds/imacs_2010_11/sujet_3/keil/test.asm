
;"########################################"
; Assemblage conditionné
;"########################################"



;************************************************************************
	THUMB	
	REQUIRE8
	PRESERVE8
;************************************************************************


;************************************************************************
; 					IMPORT/EXPORT Système
;************************************************************************

	IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]


 	EXPORT main	




	
;************************************************************************
; SECTION DE DONNEES
;************************************************************************	
	AREA  mesdonnees, data, readwrite
	
Taille      EQU   5

Table		space Taille*2


Biais       DCB 127
Max         DCW 2500  
Compte      DCW -1
Resu     space 2


;************************************************************************
; SECTION DE CODE
;************************************************************************
	AREA  moncode, code, readonly


main  	PROC 

Question2    
	    
	    LDR R0,= Biais
		LDRH R3,[R0]
		LDR R0,= Max
		LDRH R5,[R0]
		SUB	 R5,R3

		LDR R0,= Compte
		LDRSH R2,[R0]
		ADD R2,#1
		STRH R2,[R0]
		
		BL Saisie
		CMP R4,#0
		BGE Positif
		NEG R4,R4
Positif	
		CMP R4,R5
		BGE SupaMax
		ADD R4,R3
		B FinFormat
SupaMax
		LDR R0,= Max
		LDRH R5,[R0]
		MOV R4,R5		
FinFormat

		LDR R1,= Table
		STRH R4,[R1,R2,LSL #1]

Question5
		LDR R0,= Compte
		LDRSH R2,[R0]
		ADD R2,#1
		STRH R2,[R0]

		BL Saisie
		CMP R4,#0			 ; Tq Nombre != 0
		BEQ FinTQ
		CMP R2,#(Taille-1)	 ; ET que Taille saisies n'ont pas été faites (Attention Taille reservation => de 0 à Taille-1)
		BGT FinTQ
		BL  Formatage
		STRH R4,[R1,R2,LSL #1]
		B Question5

FinTQ

Question6 
		LDR R0,= Compte
		LDR R1,[R0]   ; passe le nombre d'éléments dans R1
		LDR R0,= Table ; passe l'@ de Table
		BL Moyenne
		LDR R0,= Resu
		LDR R4,[R0]   ; stockage du resltat récupéré dans R4
			

		

inf     B inf	  ; boucle infinie


    	ENDP


		
Formatage PROC
		
		PUSH{R0,R3,R5,LR}
		LDR R0,= Biais
		LDRH R3,[R0]
		LDR R0,= Max
		LDRH R5,[R0]
		SUB	 R5,R3

		CMP R4,#0
		BGE Positif2
		NEG R4,R4
Positif2	
		CMP R4,R5
		BGE SupaMax2
		ADD R4,R3
		B FinFormat2
SupaMax2
		LDR R0,= Max
		LDRH R5,[R0]
		MOV R4,R5		
FinFormat2
		POP {R0,R3,R5,PC}

		ENDP



Moyenne PROC
		
		PUSH{R2,R3,LR}
		MOV R4,#0
	    MOV R3,#0   ; indice de 1 à  nombre ..bestial mais efficace... 
BouclePour
		ADD R3,#1
		CMP R3,R1
		BEQ FinPour
		LDRH R2,[R0],#2
		ADD R4,R2
		B BouclePour
FinPour
		UDIV R4,R1
		POP {R2,R5,PC}
		ENDP



;***************************************************************
; Priocédure pur faire comme si j'avais une librairie
; au point pour saisir des nombre sur une laision série..
; A simuler par poitn d'arrêt et affectation de R4 !
;***************************************************************

Saisie   PROC
		 
		 NOP
		 BX LR

		 ENDP


	END


