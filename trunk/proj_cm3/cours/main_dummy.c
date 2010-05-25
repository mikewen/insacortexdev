
int dummy_asm(void);
//char mante = 'Q';

struct{
	char treuse;
	short int ayeume;
} ure;
int main(void)
{

	int repide = 0;
 /*	char      	leston ;
	short int  	aillemeu ;
	int 	   	hello ;
	float	 	ille;
	 
	leston   = (char)  		dummy_asm();
	aillemeu = (short int) 	dummy_asm() + (short int) leston;
	hello    = (int)   		dummy_asm() + (int) aillemeu;
	ille	 = (float)  	dummy_asm() + (float) hello;
		 	
	return (int) ille;		
	*/
	ure.treuse = 'Z';
	ure.ayeume = 12;
	dummy_asm();
}
