/*
 * Reseau de capteur version 3
 *
 *   Carte A transmet B en simplex (sans acquitement) l'etat de deux capteurs (Boutons TAMP => PA0; bouton WKUP => PC13) Toute les 0.5 secondes
 *   Carte B fait suivre à C en utilisant XBE, avec acquitement et retransmission.
 *   Carte C envoi un SMS contenant le numero du capteur
 *
 *       [A]---(RT606)--->[B]---(XBEE)--->[C]---(GSM)--->[Telephone]
 *                           <--(XBEE)----            
 *
 * Les clefs de compilation suivantes sont utilisées:
 *         _CARTE_A_: Permet de compiler le programme destiné à la carte A (surveillance capteurs)
 *         _CARTE_B_: Permet de compiler le programme destiné à la carte B (Passerelle RT606 -> XBEE)
 *         _CARTE_C_: Permet de compiler le programme destiné à la carte C (Passerelle XBEE -> GSM)
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

#define GSM_NB		&UART_1_NB
#define XBEE_NB		&UART_2_NB
#define RS606_NB	&UART_3_NB

#define CARTE_ID	1

#define NUMERO_TELEPHONE 	"0601020304"
u8 preScaler;

#define MAX_CHAINES	10
#define MAX_LONGUEUR_CHAINE	100
#define MAX_CAPTEURS 2

#define ETAT_OK	0
#define ETAT_KO 1
#define ETAT_HS 2

char buf_temp[200];

struct buf_chaine_st
{
	char buffer[MAX_CHAINES][MAX_LONGUEUR_CHAINE];
	int state[MAX_CHAINES];
	int index_chaine;
	int index_caractere;
}; 

struct buf_chaine_st chaines_xbee;
struct buf_chaine_st chaines_fm;

int etat_capteurs[MAX_CAPTEURS];
int etat_precedent[MAX_CAPTEURS];
  
void MonCallback (void)	;

int strfind(char *str, char *tok)
{
char *p_tok, *p_str;

	p_tok = tok;
	p_str = str;

etape_1:
	/* Recherche du debut de tok dasn str */
	while (*p_str != 0)
	{
		if (*p_str==*p_tok) goto etape_2;

		p_str++;
	}

etape_2:
	/* Verification que tok se trouve bien la */
	while ((*p_tok != 0) && (*p_str != 0))
	{
		if (*p_str!=*p_tok) 
		{
			p_tok=tok;
			goto etape_1;
		}

		p_str++;
		p_tok++;
	} 

	if (*p_tok == 0) return 1;
	else return 0;
}

int main (void)
{
int i;

#if defined (_CARTE_B_) || defined (_CARTE_C_)
char caractere;
#endif

#if !defined (_CARTE_A_) && !defined (_CARTE_B_) && !defined (_CARTE_C_)
	#error Vous devez definir au moins une des clefs suivantes: _CARTE_A_, _CARTE_B_, _CARTE_C_
#endif

	/* Init du micro et des peripheriques */
	stm32_Init ();
	uart_init();
	lcd_init();	

	/* Affichage d'un message d'accueil */
	lcd_clear();
#if defined (_CARTE_A_)
	lcd_print("CARTE A");
	RS606SetMode(RS606_TX);
#endif /* _CARTE_A_ */
#if defined (_CARTE_B_)
	lcd_print("CARTE B");
	RS606SetMode(RS606_RX);

#endif /* _CARTE_B_ */
#if defined (_CARTE_C_)
	lcd_print("CARTE C");
	RS606SetMode(RS606_OFF);
#endif /* _CARTE_C_ */ 

	for (i=0; i<MAX_CHAINES; i++)
	{
		chaines_xbee.buffer[i][0]=0;
		chaines_xbee.state[i]=0;
		chaines_fm.buffer[i][0]=0;	
		chaines_fm.state[i]=0;
	}

	chaines_xbee.index_chaine = 0;
	chaines_xbee.index_caractere = 0;
	chaines_fm.index_chaine = 0;
	chaines_fm.index_caractere = 0;

	for (i=0; i<MAX_CAPTEURS; i++)
	{
		etat_capteurs[i]=0;
		etat_precedent[i]=ETAT_OK;
	}

	/* Demarrage du service Timer */
	//initialisation du prescaler
	preScaler = 0 ; 
	//avec appel de la routine MonCallback toutes les 10ms
	TIMEInit( MonCallback);	

	while (1)
	{
#if defined (_CARTE_B_)
		if (UART_Buffer_State(RS606) != EMPTY)
		{
			caractere=fgetc(RS606);
			
			if ((caractere=='\n') || (caractere=='\r'))
			{
				chaines_fm.buffer[chaines_fm.index_chaine][chaines_fm.index_caractere] = 0;
				chaines_fm.index_caractere=0;
				chaines_fm.state[chaines_fm.index_chaine]=1;
				chaines_fm.index_chaine++;
				if (chaines_fm.index_chaine>(MAX_CHAINES-1)) chaines_fm.index_chaine=0;
			}
			else
			{
				chaines_fm.buffer[chaines_fm.index_chaine][chaines_fm.index_caractere] = caractere;
				chaines_fm.index_caractere++;	
			}	
		}
#endif /* _CARTE_B_ */
#if defined (_CARTE_C_) || defined (_CARTE_B_) 
		if (UART_Buffer_State(XBEE) != EMPTY)
		{
			caractere=fgetc(XBEE);
			
			if (caractere=='\n')
			{
				chaines_xbee.buffer[chaines_xbee.index_chaine][chaines_xbee.index_caractere] = 0;
				chaines_xbee.index_caractere=0;
				chaines_xbee.state[chaines_xbee.index_chaine]=1;
				chaines_xbee.index_chaine++;
				if (chaines_xbee.index_chaine>(MAX_CHAINES-1)) chaines_xbee.index_chaine=0;
			}
			else
			{
				chaines_xbee.buffer[chaines_xbee.index_chaine][chaines_xbee.index_caractere] = caractere;
				chaines_xbee.index_caractere++;	
			}	
		}
#endif /* _CARTE_C_ */ 
	}
}

void MonCallback (void)
{
#if !defined (_CARTE_A_)
int i;
int numero_capteur;
#endif

	preScaler++;

	if (preScaler==100)
	{
		preScaler = 0;

#if defined (_CARTE_A_)
   		if (!GPIOGetState(BOUTON_TAMP))
		{
			/* Le capteur a emit une alerte */
			fprintf (RS606, "%d=ALERTE\n", CARTE_ID);
			
			lcd_clear();
			lcd_print("Capteur: ALERTE");
		}
		else
		{
			/* Le capteur va bien */
			fprintf (RS606, "%d=OK\n", CARTE_ID);

			lcd_clear();
			lcd_print("Capteur: OK");
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
						//sprintf (buf_temp, "Capteur %d:OK    ", numero_capteur);
						lcd_print ("Capteur :OK    ");
					}	
					else if (strfind(chaines_fm.buffer[i], "ALERTE"))
					{
						etat_capteurs[numero_capteur-1]=0;
						set_cursor(0,numero_capteur-1);
						//sprintf (buf_temp, "Capteur %d:ALERTE", numero_capteur);
						lcd_print ("Capteur :ALERTE");

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
				//sprintf (buf_temp, "Capteur %d:HS    ", i+1);
				lcd_print ("Capteur :HS    ");

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
				fprintf(GSM,"AT+CMGS=\"%s\"\r\n",NUMERO_TELEPHONE);
				fprintf(GSM,"%s\0x1A\n",(char *)chaines_xbee.buffer[i]);
			}
		}
#endif /* _CARTE_C_ */
	}	
}

