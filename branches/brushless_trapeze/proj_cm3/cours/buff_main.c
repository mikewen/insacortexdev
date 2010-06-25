#include "stdio.h"
#include "stm_clock.h"
#include "stm_usartx.h"
#include "waits.h"
extern unsigned int ELFHash(char* str, unsigned int len)  ;
 
// 	  overflow_it(0xFF,16+4+4,0x08000216)


int check_passwd (void)
{
	struct {
		char word[16];
		char ok ;
	} pass;
	
	unsigned int somme ;
	

		pass.ok = 0;

		scanf("%s",pass.word);
		
		somme = ELFHash(pass.word,16);
		
		if (somme == 0x66)
		{
 			pass.ok= 0xFF	 ;
		}

	return  pass.ok ;
}

void authentified_code(void)
{
	while (1) printf("Welcome Mr President\n");
}

void dangereous_action(void)
{
	while (1) printf("Raise all employee's pay\n");
}

void dummy_asm(void);
		
int main(void)

{
  const char MaxCalls =3;

	Init_Clock_System() ;
		
	setup_usart();

   while(Get_Global_Nb_Calls()<MaxCalls)
   {
		printf("Enter pass phrase:\n");
   		if (check_passwd()) 
			authentified_code() ;
		else
		{
			printf("Wrong password\n");
			wait_in_reverse_vu32_su8();

		}
	}
	return 0;		
}
