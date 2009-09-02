#include <stm32f10x_lib.h>
#include "Metro_Librairie_Version2009b.h"

int main(void)
{
  Init_Periphs();
  Fixe_Rapport(-2000);					  //% min pour assurer le demerrage du train est 24,5% ==> (-1000,1000)
  while(1)
  {
  
  }


}
