unsigned char Global_Nb_Calls =0;

void wait_s32_u8(void)
{	int i=0;
	unsigned char Nb_Calls = 0;
	Global_Nb_Calls ++;
	Nb_Calls ++;
	for (i=0;i<0xF0000000;i++) ;
}

void wait_u32_su8(void)
{	unsigned int i=0;
	static unsigned char Nb_Calls = 0;
	Global_Nb_Calls ++;
	Nb_Calls ++;
	for (i=0;i<0xF0000000;i++) ;
}

void wait_vu32_vu8(void)
{	volatile unsigned int i=0;
	static unsigned char Nb_Calls = 0;
	Global_Nb_Calls ++;
	Nb_Calls ++;
	for (i=0;i<0xF0000000;i++) ;
}

void wait_in_reverse_vu32_su8(void)
{	volatile register unsigned int i=0;
	static unsigned char Nb_Calls = 0;
	Global_Nb_Calls ++;
	Nb_Calls ++;
	for (i=0xF0000000;i>0;i--) ;
}


unsigned char Get_Nb_Calls (void)
{
	static unsigned char Nb_Calls;
	return Nb_Calls;
}

unsigned char Get_Global_Nb_Calls (void)
{
	return Global_Nb_Calls;
}
