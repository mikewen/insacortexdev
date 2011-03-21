;__________________________________________________
	THUMB	
	REQUIRE8
	PRESERVE8
; 	IMPORT/EXPORT Système
	IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
;__________________________________________________


 	EXPORT main	

CAKE   equ 10
;___________________________________________
	AREA    DummyVars,		DATA,		READWRITE
Caracteres	DCB		12,-1,'A'
Nombre		DCW		-2,0xACDC, ((CAKE<<12)+ (0xC<<8) + (12<<0x4))
Miam_Un		SPACE	CAKE
Fin_Cake

;____________________________________________            			
	AREA 	DummyCode,		CODE,		READONLY

main PROC

	MOV		R2, #CAKE
	LDR		R1, =Fin_Cake
	LDR		R0,	=DummyVars
____Repete
		LDRH 	R7 , [R0]
		LDRSH 	R8 , [R0]
		ADD		R0,	#2
	B ____Repete
			
	

 ENDP ; main
 	
	END