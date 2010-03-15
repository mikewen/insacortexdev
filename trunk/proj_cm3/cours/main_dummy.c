
int dummy_asm(void);
		
int main(void)
{
 	char      	leston ;
	short int  	aillemeu ;
	int 	   	hello ;
	float	 	ille;
	 
	leston   = (char)  		dummy_asm();
	aillemeu = (short int) 	dummy_asm() + (short int) leston;
	hello    = (int)   		dummy_asm() + (int) aillemeu;
	ille	 = (float)  	dummy_asm() + (float) hello;
		 	
	return (int) ille;		
}
