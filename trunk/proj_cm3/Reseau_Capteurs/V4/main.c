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
#include <string.h>

#define GSM		&UART_1
#define XBEE	&UART_2
#define RS606	&UART_3

#define LCD		&UART_1

#define CARTE_ID	1

#define TAILLE_BUFFER 200

int c;
char buffer_RS606[TAILLE_BUFFER];
char buffer_XBEE[TAILLE_BUFFER];
int index_buffer_RS606;
int index_buffer_XBEE;
int buffer_RS606_plein;
int buffer_XBEE_plein;

u8 preScaler;
void MonCallback (void)	;

#define TEMPO_RS_MAX 100
#define TEMPO_XBEE_MAX 100
int tempo_envoi_RS;
int tempo_envoi_XBEE;
char caractere_tx_RS;
char caractere_tx_XBEE;
char relache_bouton_RS;
char relache_bouton_XBEE;

int main (void)
{
	/* Init du micro et des peripheriques */
	stm32_Init ();
	uart_init();
	lcd_init();	

	/* Affichage d'un message d'accueil */
	lcd_clear();
	lcd_print("RX");
	RS606SetMode(RS606_RX);

	index_buffer_RS606 = 0;
	index_buffer_XBEE = 0;
	buffer_RS606_plein = 0;
	buffer_XBEE_plein =0;

	tempo_envoi_RS = 0;
	tempo_envoi_XBEE = 0;
	caractere_tx_RS ='A';
	caractere_tx_XBEE = 'A';

	relache_bouton_RS =0;
	relache_bouton_XBEE=0;

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
				buffer_XBEE[index_buffer_XBEE]=0;
				buffer_XBEE_plein=1;
			}
			else
			{
				buffer_XBEE[index_buffer_XBEE] = (char)c;
				index_buffer_XBEE++;

				if (index_buffer_XBEE>=TAILLE_BUFFER) index_buffer_XBEE = TAILLE_BUFFER-1;
			}	
		}

		/* Remplissage du buffer pour la liaison GSM (900 Mhz) */
		/* A faire !!! */
	}
}

void MonCallback (void)
{
	preScaler++;

	if (preScaler==100)	/* Si 10 ms se sont ecoulées, execution des traitements periodiques */ 
	{
		/* Remise a zero du compteur de periode */
		preScaler = 0;

	   	/* Gestion du module RS606 */
   		if (!GPIOGetState(BOUTON_TAMP))	
		{
			/* Passe le module RS606 en transmission */
			RS606SetMode(RS606_TX);
			relache_bouton_RS = 1;

			/* Envoi de caractere sur RS606 */
			tempo_envoi_RS++;

			if (tempo_envoi_RS>=TEMPO_RS_MAX)
			{
				caractere_tx_RS++;

				if (caractere_tx_RS>'Z') caractere_tx_RS = 'A';
			}

			set_cursor(0,0);
			fprintf(LCD,"RS-T: MSG %c   ",caractere_tx_RS);

			fprintf (RS606,"MSG %c\r",caractere_tx_RS);
		}
		else
		{
			if (relache_bouton_RS==1)
			{
				if (UART_RS606TransmissionTerminee==1)
				{
					/*  passe le RS606 en mode reception ou OFF, selon l'etat de CD*/
					RS606SetMode(RS606_RX);
	
					relache_bouton_RS=0;
	
					set_cursor(0,0);
					fprintf(LCD,"RS-R:                ");
				}
			}

			/* si une trame à été recue */
			if (buffer_RS606_plein == 1)
			{
				buffer_RS606_plein =0;
				index_buffer_RS606 = 0;

				if (strlen (buffer_RS606) >1)
				{
					set_cursor(0,0);
					fprintf(LCD,"RS-R: %s           ",buffer_RS606+1); 	/* suppression du premier caractere */
				}
				else
				{
					set_cursor(0,0);
					fprintf(LCD,"RS-R:              ");
				}
			}
		}

	   	/* Gestion du module XBEE */
   		if (!GPIOGetState(BOUTON_WKUP))	
		{
			/* Envoi de caractere sur XBEE */
			tempo_envoi_XBEE++;
			relache_bouton_XBEE = 1;

			if (tempo_envoi_XBEE>=TEMPO_XBEE_MAX)
			{
				caractere_tx_XBEE++;

				if (caractere_tx_XBEE>'Z') caractere_tx_XBEE = 'A';
			}

			set_cursor(1,0);
			fprintf(LCD,"XB-T: MSG %c   ",caractere_tx_XBEE);

			fprintf (XBEE,"MSG %c\r",caractere_tx_XBEE);
		}
		else
		{
			if (relache_bouton_XBEE==1)
			{
				relache_bouton_XBEE=0;

				set_cursor(1,0);
				fprintf(LCD,"XB-R:                ");
			}

			/* Si une trame à été recue */
			if (buffer_XBEE_plein == 1)
			{
				buffer_XBEE_plein =0;
				index_buffer_XBEE = 0;

				set_cursor(1,0);
				fprintf(LCD,"XB-R: %s         ",buffer_XBEE); 
			}
		}
	}	
}

