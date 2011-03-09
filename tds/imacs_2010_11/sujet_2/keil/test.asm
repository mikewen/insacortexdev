		

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
	
Vide		space 20
Un			fill 6,2,2
test        fill 8,0x89abcdef,4
Chaine  	DCB "TD 2",0
Table 		DCD  0x12,12,'A', 0, 0xABCDEF 
;************************************************************************
; SECTION DE CODE
;************************************************************************
	AREA  moncode, code, readonly


main  	PROC 

VersionQ3   
	    LDR R0,= Vide
		LDR R1,= Table
		MOV R3,#20
boucleQ3
	    LDRB R2,[R1]
		LSL R2,#1
		STRB R2,[R0]
		ADD R0,#1
		ADD R1,#1
		SUBS R3,#1
		BNE boucleQ3

VersionQ4   
	    LDR R0,=Vide
		LDR R1,= Table
		MOV R3,#20
		ADD R1,R3		   ; positionnement en fin de Table
		
boucleQ4
	    LDRB R2,[R1,#-1]!	 ; prédéplacement
		LSLS R2,#1
        STRB R2,[R0,R3]
		SUBS R3,#1
		BNE boucleQ4			

VersionQ5 
		LDR R0,=Vide
		LDR R1,= Table
		MOV R3,#20
		ADD R1,R3
		
boucleQ5
	    LDRB R2,[R1,#-1]!
		LSLS R2,#1
		STRB R2,[R0,R3]
        CMP R2,#127		      ; si Carac * 2 <0 sur un octet alors il est supérieur en non signé à 127...
		BGE inf
		SUBS R3,#1
		BNE boucleQ5
inf     B inf	  ; boucle infinie


    	ENDP


	END


