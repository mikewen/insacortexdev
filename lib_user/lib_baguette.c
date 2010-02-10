/*
 * Ecrire un truc ici
 */

#include <stm32f10x_lib.h>

#include "lib_baguette.h"
#include "stm_clock.h"
#include "stm_usartx.h"
#include "stm_system.h"

#include "lcd.h"

FILE UART;

/* 
 * Fonctions pour la gestion des IT 
 */

/* 
 * Fonction: 	Init_IT
 * Role: 		Initialise les niveaux d'IT et le controleur d'interruption
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_IT(void)
{
	/* 
	 * Regle les niveaux d'IT 
	 * Par defaut, toutes les IT a niveau de prio reglable sont reglés à 0
	 * donc, niveau d'IT max
	 *
	 * On va regler ceux que l'on utilise sur des niveaux moins important
	 * -> permet le preamption + les vecteurs de faute d'etre activé
	 */
	NVIC_SET_PRIO_PERIPH(TIM1_UP, 15); 	/* L'IT timer 1 est la moins prio (affichage LCD) */
	NVIC_SET_PRIO_PERIPH(ADC1_2, 14);
	NVIC_SET_PRIO_SYSTEM(SYSTICK, 13);	/* Le clignotement des LED est le plus prioritaire */

	/* Efface les etats "PENDING" des vecteurs */
	NVIC_CLEAR_PENDING_PERIPH_IT(TIM1_UP);
	NVIC_CLEAR_PENDING_PERIPH_IT(ADC1_2);
	/* RQ: il n'existe pas d'etat "PENDING" pour les IT SYSTICK */

	/* Active la prise en compte des IT */
	NVIC_ENABLE_PERIPH_IT(TIM1_UP);
	NVIC_ENABLE_PERIPH_IT(ADC1_2);
	/* RQ: La prise en compte des IT SYSTICK est actif par defaut */

	/* Et pour le debug, on active aussi la prise en compte des vecteurs de fautes */
	NVIC_ENABLE_SYSTEM_IT(MEM_FAULT);
	NVIC_ENABLE_SYSTEM_IT(BUS_FAULT);
	NVIC_ENABLE_SYSTEM_IT(USAGE_FAULT);

	/* Autorise les IT */
	/*SYS_ENABLE_GLOBAL_INTERRUPTS();
	SYS_ENABLE_FAULT_INTERRUPTS(); */
}

/* 
 * Fonction: 	Demarre_SYSTICK
 * Role: 		Lance le timer SYSTICK ( pour le clignotement des led )
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Demarre_SYSTICK(void)
{
	SYSTICK_SET_PERIOD(1250); /* Regle une periode de 800 ns sur le timer system */

	SYSTICK_CLOCK_AHB();		/* Set AHB (CPU clock) as clock input */
	SYSTICK_ENABLE_COUNTER(); 	/* Lance le timer system */
	SYSTICK_ENABLE_IT();		/* Et autorise les IT */
}

/* 
 * Fonction: 	Arrete_SYSTICK
 * Role: 		Arrete le timer SYSTICK ( pour le clignotement des led )
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Arrete_SYSTICK(void)
{
	SYSTICK_DISABLE_IT();
	SYSTICK_DISABLE_COUNTER();
}

/*
 * Timer1 Bits definitions
 */

#define TIM_AUTO_RELOAD	(1<<7)
#define TIM_ENABLE		(1<<0)		
/* 
 * Fonction: 	Demarre_Timer1
 * Role: 		Lance le timer 1 (pour le rafraichissement de l'interface)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Demarre_Timer1(void)
{
	RCC->APB2ENR |= RCC_APB2Periph_TIM1;	/* Active l'horloge du Timer 1 */
	
	TIM1->CR1 = TIM_CounterMode_Down + TIM_AUTO_RELOAD;
	TIM1->DIER = TIM_IT_Update;

	TIM1->PSC = 65535; 	/* Fclk = Fapb2 / (65535+1) */
	TIM1->ARR = ((__TIMxCLK / 65536) /10); /* Periode souhaité : 100ms, soit 10 Hz */
	
	TIM1->CR1 |= TIM_ENABLE;
}

/* 
 * Fonction: 	Arrete_Timer1
 * Role: 		Arrete le timer 1 (pour le rafraichissement de l'interface)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Arrete_Timer1(void)
{
	TIM1->CR1 &= ~TIM_ENABLE;
}

/* 
 * Fonction: 	Acquite_Timer1
 * Role: 		Acquite la drapeau d'interuption du timer 1 
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Acquite_Timer1(void)
{
	TIM1->SR &= ~TIM_IT_Update;
}

/* 
 * Fonctions pour la gestion de l'ecran LCD 
 */

/* 
 * Fonction: 	Efface_Ecran
 * Role: 		Efface l'ecran LCD
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Efface_Ecran(void)
{
	/* Pour l'ecran */
	lcd_clear();

	/* pour la LS */
	fputc('\n',&UART);
	fputc('\n',&UART);
}

/* 
 * Fonction:	Ecrit_Ecran
 * Role: 		Ecrit un caractere sur l'ecran LCD
 * Entrée: 
 *		R0: Octet de poids faible contient l'octet a ecrire
 * Sortie: 		Rien
 */
void Ecrit_Ecran(char c)
{
	/* Pour l'ecran */
	lcd_putchar(c);

	/* pour la LS */
	fputc(c,&UART);
}

/* 
 * Fonction:	Ecrit_Chaine_Ecran
 * Role: 		Ecrit d'une chaine de caractere l'ecran LCD
 * Entrée: 
 *		R0: @ de la chaine (la chaine se termine par un caractere NULL (0x00)
 * Sortie: 		Rien
 */
void Ecrit_Chaine_Ecran(char *str)
{
	while (*str!=0)
	{
		/* Pour l'ecran */
		lcd_putchar(*str);

		/* Pour la LS */
		fputc(*str,&UART);

		str++;
	}
}

/* 
 * Fonction: 	Position_Curseur
 * Role: 		Positionne le curseur sur l'ecran LCD
 * Entrée: 
 *      R0: contient l'offset sur une ligne (coordonnée X), entre 0 et 19
 *      R1: contient l'identifiant de ligne (coordonnée Y), entre 0 et 1
 * Sortie: 		Rien
 */
void Position_Curseur(int x, int y)
{
	/* Pour l'ecran */
	set_cursor(x,y);

	/* pour la LS */
	fputc('\n',&UART);	
}

/* 
 * Fonctions pour la gestion de l'ADC 
 */
int Current_ADC;
int ADC_Array[3];

int ADC_min, ADC_max;

/* 
 * Fonction: 	Init_ADC
 * Role: 		Initialisation de l'ADC
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_ADC (void) 
{
  	Current_ADC = 0;

	ADC_min = 0xFFFF;
	ADC_max = 0;

  	RCC->APB2ENR |= RCC_APB2Periph_ADC1;            // enable peripheral clock for ADC1

  	ADC1->SQR1  = 0x00000000;                       // only one conversion
  	ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  	ADC1->SQR3  = 0x00000001;                       // set channel1 as 1st conversion

  	ADC1->CR1   = 0x00000020;                       // use independant mode, EOCIE mode
  	ADC1->CR2   = 0x000E0001;                       // use data align right,continuous conversion
                                                  	// EXTSEL = SWSTART 
                                                  	// enable ADC, no external Trigger
  	ADC1->CR2  |=  0x00500000;					  	// start SW conversion
}

/* 
 * Fonction: 	Lire_ADC
 * Role: 		Lecture de la valeur d'un canal de l'ADC
 * Entrée: 		
 *		R0:	Canal ADC à lire.
 *			0 = Potentiometre
 *			1 = Accelerometre voie X
 *			2 = Accelerometre voie Y
 * Sortie: 
 *		R0: Valeur ADC (12 bits de poids faible)
 */
int Lire_ADC(int channel)
{
volatile register int tmp;

   	tmp = ADC_Array[channel];

	return tmp;
}

/* 
 * Fonction: 	ADC_IRQHandler
 * Role: 		Vecteur d'interruption de l'ADC: appelée en fin de conertion d'un canal
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void ADC_IRQHandler	(void)
{
	ADC_Array[Current_ADC] = ADC1->DR;

	Current_ADC++;
	if (Current_ADC>=3) Current_ADC =0;
	
	if (Current_ADC == 0) ADC1->SQR3  = 1;                      // set Channel as 1st conversion
	else if (Current_ADC == 1) ADC1->SQR3  = 14;                // set Channel as 1st conversion
	else ADC1->SQR3  = 15; 

	ADC1->CR2  |=  0x00500000;					  				// start SW conversion
}

/* 
 * Fonction: 	Init_Baguette
 * Role: 		Initialisation de la baguette
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_Baguette(void)
{
	/* Init des horloges */
	Init_Clock_System();

	/* Init de l'ecran LCD */
	lcd_init();

	/* Init du port serie (utilisé pour la simu) */
	setup_usart();

	/* Init des ADC */
	Init_ADC();

	/* Init des IT */
	Init_IT();
}
