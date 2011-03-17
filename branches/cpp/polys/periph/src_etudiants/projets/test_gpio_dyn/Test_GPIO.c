#include "../../pilotes/GPIO_dyn.h" 


int Num_Broche = 8;
//=======================================================================================
int main(void)
{
	char flipflop = 1;
	Port_IO_Init(GPIOB, 8, 'o', 0);

	while(1)
	{
			
			flipflop=!flipflop;
			if (flipflop)
			{
				Port_IO_Clr(GPIOB,Num_Broche);
			}
			else
			{
				Port_IO_Set(GPIOB,Num_Broche);
			}
	}
	
}

