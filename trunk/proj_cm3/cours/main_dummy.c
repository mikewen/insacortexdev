#include "waits.h"

extern void dummy_asm(void);

extern unsigned char Global_Number_Of_Wait_Calls ;
extern unsigned char Number_Of_Wait_Calls ;

int main(void)
{
	 char avoile;
	 char ette;
	 char pie;
	 char meurt;

	//dummy_asm();
	
	wait_s32_u8();
	wait_u32_su8();
	wait_in_reverse_s32_su8();
	wait_glbl_su8();
	wait_volatile_su8();

	avoile = Get_Number_Of_Wait_Calls ();
	ette = Get_Number_Of_Wait_Calls ();
	pie = Global_Number_Of_Wait_Calls ;
	meurt =  Number_Of_Wait_Calls ;

	while (1) ;

}
