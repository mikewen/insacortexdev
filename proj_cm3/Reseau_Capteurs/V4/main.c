/*
 * Reseau de capteur version 4
 *
 *   Carte A transmet B en simplex (sans acquitement) l'etat de deux capteurs (Boutons TAMP => PA0; bouton WKUP => PC13) Toute les 0.5 secondes
 *   Carte B fait suivre à C en utilisant XBE, avec acquitement et retransmission.
 *   Carte C envoi un SMS contenant le numero du capteur
 *
 *       [A]---(RT606)--->[B]---(XBEE)--->[C]---(GSM)--->[Telephone]
 *                           <--(XBEE)----            
 *
 * Les clefs de compilation suivantes sont utilisées:
 *         _LCD_DEBUG_: Permet de desactiver l'ecran LCD, lors des simulations
 */

#include "uart.h"
#include "adc.h"
#include "time.h"
#include "gpio.h"
#include "lcd.h"
#include "rs606.h"

#include "STM32_Init.h"
#include <stdio.h>

#define GSM		&UART_1
#define XBEE	&UART_2
#define RS606	&UART_3

#define CARTE_ID	1

#define TAILLE_BUFFER 200

int c;
char buffer_RS606[TAILLE_BUFFER];
char buffer_XBee[TAILLE_BUFFER];
int index_buffer_RS606;
int index_buffer_XBee;
int buffer_RS606_plein;
int buffer_XBee_plein;

u8 preScaler;
void MonCallback (void)	;

int main (void)
{
int i;

	/* Init du micro et des peripheriques */
	stm32_Init ();
	uart_init();
	lcd_init();	

	/* Affichage d'un message d'accueil */
	lcd_clear();
	lcd_print("RX");
	RS606SetMode(RS606_RX);

	index_buffer_RS606 = 0;
	index_buffer_XBee = 0;
	buffer_RS606_plein = 0;
	buffer_XBee_plein =0;

	/* Demarrage du service Timer */
	//initialisation du prescaler
	preScaler = 0 ; 
	//avec appel de la routine MonCallback toutes les 10ms
	TIMEInit( MonCallback);	

	/* Recuperation des caracteres sur les liaisons series */
	while (1)
	{
		/* Remplissage du buffer pour la liaison RS606 (FM) */
		if (UART_Buffer_State(RS606) != EMPTY)
		{
			c=fgetc(RS606);
			
			if ((c=='\n') || (c=='\r'))
			{
				buffer_RS606[index_buffer_RS606]=0;
				buffer_RS606_plein=1;
			}
			else
			{
				buffer_RS606[index_buffer_RS606] = (char)c;
				index_buffer_RS606++;

				if (index_buffer_RS606>=TAILLE_BUFFER) index_buffer_RS606 = TAILLE_BUFFER-1;
			}	
		}

		/* Remplissage du buffer pour la liaison XBEE (2.4GHz) */
		if (UART_Buffer_State(XBEE) != EMPTY)
		{
			c=fgetc(XBEE);
			
			if ((c=='\n') || (c=='\r'))
			{
				buffer_XBee[index_buffer_XBee]=0;
				buffer_XBee_plein=1;
			}
			else
			{
				buffer_XBee[index_buffer_XBee] = (char)c;
				index_buffer_XBee++;

				if (index_buffer_XBee>=TAILLE_BUFFER) index_buffer_XBee = TAILLE_BUFFER-1;
			}	
		}

		/* Remplissage du buffer pour la liaison GSM (900 Mhz) */
		/* A faire !!! */
	}
}

void MonCallback (void)
{
int i;
int numero_capteur;

	preScaler++;

	if (preScaler==100)	/* Si 10 ms se sont ecoulées, execution des traitements periodiques */ 
	{
		/* Remise a zero du compteur de periode */
		preScaler = 0;

#if defined (_CARTE_A_)
   		if (!GPIOGetState(BOUTON_TAMP))
		{
			/* Le capteur a emit une alerte */
			fprintf (RS606, "%d=ALERTE\n", CARTE_ID);
			
			lcd_clear();
			sprintf (buf_temp, "Capteur %d:ALERTE", CARTE_ID);
			lcd_print(buf_temp);
		}
		else
		{
			/* Le capteur va bien */
			fprintf (RS606, "%d=OK\n", CARTE_ID);

			lcd_clear();
			sprintf (buf_temp, "Capteur %d:OK    ", CARTE_ID);
			lcd_print(buf_temp);
		}
#endif /* _CARTE_A_ */

#if defined (_CARTE_B_)		
		for (i=0; i<MAX_CHAINES; i++)
		{
			if (chaines_fm.state[i] != 0)
			{
				/* On a recu un message d'un capteur */
				chaines_fm.state[i] = 0;

				numero_capteur = chaines_fm.buffer[i][0]-'0';

				if ((numero_capteur>0) && (numero_capteur<=MAX_CAPTEURS))
				{
					if (strfind(chaines_fm.buffer[i], "OK"))
					{
						etat_capteurs[numero_capteur-1]=0;
						etat_precedent[numero_capteur-1] = ETAT_OK;
						set_cursor(0,numero_capteur-1);
						sprintf (buf_temp, "Capteur %d:OK    ", numero_capteur);
						lcd_print (buf_temp);//"Capteur :OK    ");
					}	
					else if (strfind(chaines_fm.buffer[i], "ALERTE"))
					{
						etat_capteurs[numero_capteur-1]=0;
						set_cursor(0,numero_capteur-1);
						sprintf (buf_temp, "Capteur %d:ALERTE", numero_capteur);
						lcd_print (buf_temp);//"Capteur :ALERTE");

						if (etat_precedent[numero_capteur-1] != ETAT_KO)
						{	
							fprintf (XBEE, "Capteur %d ALERTE\n", numero_capteur);
							etat_precedent[numero_capteur-1] = ETAT_KO;
						}
					}
				}
			}
		}	
		
		for (i=0; i<MAX_CAPTEURS; i++)
		{
			etat_capteurs[i]++;
		
			if (etat_capteurs[i]>1) /* Capteur HS */
			{
				set_cursor(0,i);
				sprintf (buf_temp, "Capteur %d:HS    ", i+1);
				lcd_print (buf_temp);//"Capteur :HS    ");

				if (etat_precedent[i] != ETAT_HS)
				{
					fprintf (XBEE, "Capteur %d HS\n", i+1);
					etat_precedent[i] = ETAT_HS;	
				}	
		
				etat_capteurs[i]=0;
			}
		}	
#endif /* _CARTE_B_ */

#if defined (_CARTE_C_)
		for (i=0; i<MAX_CHAINES; i++)
		{
			if (chaines_xbee.state[i] != 0)
			{
				/* On a recu un message d'un capteur */
				chaines_xbee.state[i] = 0;

				/* Affichage du message */
				lcd_clear();
				lcd_print((char *)chaines_xbee.buffer[i]);

				/* Envoi sur GSM (UART1) d'un texto contenant l'erreur */
				fprintf(GSM,"AT+CMGF=1\r\n");
				{
				int j ;
				for (j=0;j<8000000;j++);
				fprintf(GSM,"AT+CMGS=\"%s\"\r\n",NUMERO_TELEPHONE);
				for (j=0;j<8000000;j++);
				fprintf(GSM,"%s%c\n",(char *)chaines_xbee.buffer[i],0x1A);
				}
			}
		}
#endif /* _CARTE_C_ */
	}	
}

