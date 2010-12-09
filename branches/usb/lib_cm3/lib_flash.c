/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * lib_cm3 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * lib_cm3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with lib_cm3; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm_regs.h"
#include "stm_clock.h"
#include "lib_flash.h"

static void FLASH_delay(void);

#define EraseTimeout             ((unsigned int)0x00000FFF)
#define ProgramTimeout           ((unsigned int)0x0000000F)

int FLASH_GetStatus(void);
int FLASH_WaitForLastOperation(unsigned int Timeout);
int FLASH_Program16(unsigned int addr, u16 data);

int FLASH_EraseAllPages(void)
{
int status = FLASH_COMPLETE;
  
  	/* Wait for last operation to be completed */
  	status = FLASH_WaitForLastOperation(EraseTimeout);
  
	/* unlock the FPEC */
	if (FLASH->CR & FLASH_LOCK)
	{
		FLASH->KEYR = FLASH_KEY_KEY1;
  		FLASH->KEYR = FLASH_KEY_KEY2;
	}

  	if(status == FLASH_COMPLETE)
  	{
    	/* if the previous operation is completed, proceed to erase all pages */
     	FLASH->CR |= FLASH_MER;
     	FLASH->CR |= FLASH_STRT;
    
    	/* Wait for last operation to be completed */
    	status = FLASH_WaitForLastOperation(EraseTimeout);

      	/* if the erase operation is completed, disable the MER Bit */
      	FLASH->CR &= ~FLASH_MER;
  	}	   

	/* Relock the FPEC */
	FLASH->CR |= FLASH_LOCK;

  	/* Return the Erase Status */
  	return status;
}

int FLASH_ErasePage(unsigned int addr)
{
int status = FLASH_COMPLETE;

  	/* Wait for last operation to be completed */
  	status = FLASH_WaitForLastOperation(EraseTimeout);
  
  	/* unlock the FPEC */
	if (FLASH->CR & FLASH_LOCK)
	{
		FLASH->KEYR = FLASH_KEY_KEY1;
  		FLASH->KEYR = FLASH_KEY_KEY2;
	}

  	if(status == FLASH_COMPLETE)
  	{ 
    	/* if the previous operation is completed, proceed to erase the page */
    	FLASH->CR|= FLASH_PER;
    	FLASH->AR = addr; 
    	FLASH->CR|= FLASH_STRT;
    
    	/* Wait for last operation to be completed */
    	status = FLASH_WaitForLastOperation(EraseTimeout);

      	/* if the erase operation is completed, disable the PER Bit */
      	FLASH->CR &= ~FLASH_PER;
  	}

	/* Relock the FPEC */
	FLASH->CR |= FLASH_LOCK;

  	/* Return the Erase Status */
  	return status;
}

int FLASH_Program(unsigned int addr, u16 *data, int length)
{
int status = FLASH_COMPLETE;
int index =0;

	/* unlock the FPEC */
	if (FLASH->CR & FLASH_LOCK)
	{
		FLASH->KEYR = FLASH_KEY_KEY1;
  		FLASH->KEYR = FLASH_KEY_KEY2;
	}

	while ((status == FLASH_COMPLETE ) && ( index <length))
	{
		status = FLASH_Program16(addr+(2*index), data[index]);
		index ++;	
	}

	/* Relock the FPEC */
	FLASH->CR |= FLASH_LOCK;

	return status;
}
 
int FLASH_Program16(unsigned int addr, u16 data)
{
int status = FLASH_COMPLETE;

  	/* Wait for last operation to be completed */
  	status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  	if(status == FLASH_COMPLETE)
  	{
    	/* if the previous operation is completed, proceed to program the new data */
    	FLASH->CR |= FLASH_PG;
  
    	*(volatile u16*)addr = data;
    	/* Wait for last operation to be completed */
    	status = FLASH_WaitForLastOperation(ProgramTimeout);
    
	   	if (status == FLASH_COMPLETE)
		{ 
			/* check if data was correctly written */
			if (*(volatile u16*)addr != data) status = FLASH_ERROR_PG;
		}

      	/* disable the PG Bit */
      	FLASH->CR &= ~FLASH_PG;
 	} 
  
  	/* Return the Program Status */
  	return status;
}

int FLASH_GetStatus(void)
{
int flashstatus = FLASH_COMPLETE;
  
  	if((FLASH->SR & FLASH_BSY) == FLASH_BSY) 
  	{
    	flashstatus = FLASH_BUSY;
  	}
  	else 
  	{  
    	if(FLASH->SR & FLASH_PGERR)
    	{ 
      		flashstatus = FLASH_ERROR_PG;
    	}
    	else 
    	{
      		if(FLASH->SR & FLASH_WRPRTERR)
      		{
        		flashstatus = FLASH_ERROR_WRP;
     		}
      		else
      		{
        		flashstatus = FLASH_COMPLETE;
      		}
    	}
  	}
  
  	/* Return the Flash Status */
  	return flashstatus;
}

int FLASH_WaitForLastOperation(unsigned int Timeout)
{ 
int status = FLASH_COMPLETE;
   
  	/* Check for the Flash Status */
  	status = FLASH_GetStatus();

  	/* Wait for a Flash operation to complete or a TIMEOUT to occur */
  	while((status == FLASH_BUSY) && (Timeout != 0x00))
  	{
    	FLASH_delay();
    	status = FLASH_GetStatus();
    	Timeout--;
  	}

  	if(Timeout == 0x00 )
  	{
    	status = FLASH_TIMEOUT;
  	}

  	/* Return the operation status */
  	return status;
}

/*
 * Inserts a time delay.
 */
static void FLASH_delay(void)
{
volatile unsigned int i = 0;

  	for(i = 0xFF; i != 0; i--)
  	{
  	}
}