#include "stm_clock.h"
#include "stm_usartx.h"
#include "stm_adc.h"

void Adc_Eoc (void)
{

}
void Adc_Jeoc (void)
{

}
void Adc_Awd (void)
{

}
void Adc_Dma_Pannic (void)
{

}
void Adc_Dma_Half (void)
{

}
void Adc_Dma_Finished (void)
{


}

int main(void)
{
  	Init_Clock_System() ;
	setup_usart();
	Init_Adc();

	printf("Coucou\n");
	ADC1_START_CONV;
	while (1)
	{
	} 
}