unsigned char Nombre_D_Appels =0;


void wait_s32_u8(void)	__pure
{
	int i=0;
	unsigned char Number_Of_Wait_Calls = 0;

	Nombre_D_Appels ++;
	Number_Of_Wait_Calls ++;

	for (i=0;i<0xF000;i++) ;

}

void wait_vs32_vu8(void)
{
	volatile int i;
	volatile unsigned char Number_Of_Wait_Calls = 0;

	Nombre_D_Appels ++;
	Number_Of_Wait_Calls ++;

	for (i=0;i<0xF000;i++) ;

}

void wait_vu32_su8(void)
{
	volatile unsigned int i;
	static unsigned char Number_Of_Wait_Calls = 0;

	Nombre_D_Appels ++;
	Number_Of_Wait_Calls ++;

	for (i=0;i<0xF000;i++) ;

}

void wait_in_reverse_vu32_su8(void)
{
	volatile unsigned int i;
	static unsigned char Number_Of_Wait_Calls = 0;

	Nombre_D_Appels ++;
	Number_Of_Wait_Calls ++;

	for (i=0xF000;i>0;i--) ;

}

unsigned char Get_Number_Of_Wait_Calls (void)
{
	static unsigned char Number_Of_Wait_Calls;
	return Number_Of_Wait_Calls;
}

unsigned char Get_Nombre_D_Appels (void)
{

	return Nombre_D_Appels;
}
