#include <stm32f10x_lib.h>
#include "at.h"
#include "uart.h"
#include "time.h"

#include <stdio.h>

typedef enum {COMMAND_MODE=1, DATA_MODE=2} AT_MODE;
AT_MODE ATMode;

FILE ATCOMChannel;

void ATInit(u8 channel)
{
	ATMode = DATA_MODE;
	ATCOMChannel._file = UART_2._file;
}

AT_STATUS ATGetStatus(void)
{
u8 c;

	//c= getchar(ATCOMChannel, 120);
	c= getchar();
	//putchar(UART_1, c);
	putchar(c);	

	if ((c=='O') || (c=='o'))
	{
		/* On part vers la reponse OK */
		//c= getchar(ATCOMChannel,2); /* Ca doit etre un K */
		//putchar(UART_1, c);
		//c= getchar(ATCOMChannel,2); /* Ca doit etre un CR */
		//putchar(UART_1, c);
		c= getchar(); /* Ca doit etre un K */
		putchar(c);
		c= getchar(); /* Ca doit etre un CR */
		putchar(c);
		return AT_SUCCESS;
	}
	else
	{
		/* Ca sent la reponse ERROR */
//	    c= getchar(ATCOMChannel,2); /* Ca doit etre un R */
//		putchar(UART_1, c);
//		c= getchar(ATCOMChannel,2); /* Ca doit etre un R */
//		putchar(UART_1, c);
//		c= getchar(ATCOMChannel,2); /* Ca doit etre un O */
//		putchar(UART_1, c);
//		c= getchar(ATCOMChannel,2); /* Ca doit etre un R */
//		putchar(UART_1, c);
//		c= getchar(ATCOMChannel,2); /* Ca doit etre un CR */
//		putchar(UART_1, c);
		c= getchar(); /* Ca doit etre un R */
		putchar(c);
		c= getchar(); /* Ca doit etre un R */
		putchar(c);
		c= getchar(); /* Ca doit etre un O */
		putchar(c);
		c= getchar(); /* Ca doit etre un R */
		putchar(c);
		c= getchar(); /* Ca doit etre un CR */
		putchar(c);
		return AT_ERROR;
	} 
}

void ATEnterMode(void)
{
	if (ATMode!= COMMAND_MODE)
	{
		TIMEResetCounter();
		while (TIMEGetCounter()<120); // Attente de 1.2 seconde (Xbee demande 1 seconde)

//		printf (ATCOMChannel, "+++");	/* Passage en mode commande */
//		printf (UART_1, "+++");
		printf ("+++");	/* Passage en mode commande */
		printf ("+++");

		/*TIMEResetCounter();
		while (TIMEGetCounter()<120);*/ // Attente de 1.2 seconde (Xbee demande 1 seconde)
		if (ATGetStatus() == AT_SUCCESS)
		{
			ATMode = COMMAND_MODE;		
		}
	}
}

void ATExitMode(void)
{
	if (ATMode != DATA_MODE)
	{
		//Envoi de la command ATCN
//		printf (ATCOMChannel, "ATCN\r");
//		printf (UART_1, "ATCN\r");
		printf ("ATCN\r");
		printf ("ATCN\r");
		/*TIMEResetCounter();
		while (TIMEGetCounter()<120);*/ // Attente de 1.2 seconde (Xbee demande 1 seconde)
		ATGetStatus();
	}

	ATMode = DATA_MODE;
}

AT_STATUS ATSetChannel(u8 channel)
{
	// Les canaux autorisés par le xbee sont entre 0B et 1A, avec 0C la valeur par defaut
	if ((channel <0x0B) || (channel >0x1A))
	{
		return AT_ERROR;
	}

	// Envoi de la commande
//	printf (ATCOMChannel, "ATCH %x\r",channel);
//	printf (UART_1, "ATCH %x\r",channel);
	printf ("ATCH %x\r",channel);
	printf ("ATCH %x\r",channel);

	return ATGetStatus();
}

AT_STATUS ATSetPanID(u16 panId)
{
	// Envoi de la commande
//	printf (ATCOMChannel, "ATID %x\r",panId);
	printf ("ATID %x\r",panId);

	return ATGetStatus();
}

AT_STATUS ATSetDestinationLow(u32 destLow)
{
	// Envoi de la commande
	//printf (ATCOMChannel, "ATDL %x\r",destLow);
	printf ("ATDL %x\r",destLow);
	return ATGetStatus();
}

AT_STATUS ATSetDestinationHigh(u32 destHigh)
{
	// Envoi de la commande
	//printf (ATCOMChannel, "ATDH %x\r",destHigh);
	printf ("ATDH %x\r",destHigh);

	return ATGetStatus();
}

AT_STATUS ATSetShortSourceAddr(u16 addr)
{
	// Envoi de la commande
	//printf (ATCOMChannel, "ATMY %x\r",addr);
	printf ("ATMY %x\r",addr);

	return ATGetStatus();
}
