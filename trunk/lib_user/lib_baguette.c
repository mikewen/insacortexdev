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

#define AU_MIN 0
#define MONTE  1
#define AU_MAX   2
#define DESCEND  3
#define S_TB  ((short int)(1.0/3.3*4096.0))
#define S_B  ((short int)(1.1/3.3*4096.0))
#define S_H  ((short int)(2.6/3.3*4096.0))
#define S_TH  ((short int)(2.9/3.3*4096.0))
char phase ;

int ADC_min, ADC_max;

/* 
 * Fonction: 	Init_ADC
 * Role: 		Initialisation de l'ADC
 * Entrée: 		Rien
 * Sortie: 		Rien
 */

//___________________________________________
//ADC1->CR1 bit description
#define EOCIE 	(1<<5)
#define AWDIE 	(1<<6)
#define JEOCIE 	(1<<7)
#define SCAN 	(1<<8)
#define AWDSGL 	(1<<9)
#define JAUTO 	(1<<10)
#define DISCEN 	(1<<11)
#define JDISCEN (1<<12)
#define DISCNUM_SHIFT (13)
#define DUALMOD_SHIFT (16)
#define JAWDEN  (1<<22)
#define AWDEN  (1<<23)

//ADC1->CR2 bit description
#define ADON 	(1<<0)
#define CONT 	(1<<1)
#define CAL 	(1<<2)
#define RSTCAL 	(1<<3)
#define DMA 	(1<<8)
#define ALIGN 	(1<<11)
#define JEXTSEL_SHIFT 	(12)
#define JEXTTRIG (1<<15)
#define EXTSEL_SHIFT (17)
#define EXTSEL_ON_SWSTART (7<<EXTSEL_SHIFT)
#define EXTTRIG  (1<<20)
#define JSWSTART (1<<21)
#define SWSTART  (1<<22)
#define TSVREFE  (1<<23)

//ADC1-SQR1 bit description
#define SQX_MASK (0x001F)
#define SQ13_SHIFT (0)
#define SQ14_SHIFT (5)
#define SQ15_SHIFT (10)
#define SQ16_SHIFT (15)
#define L_MASK (0x000F)
#define L_SHIFT (20)

//ADC1-SQR2 bit description
#define SQ7_SHIFT (0)
#define SQ8_SHIFT (5)
#define SQ9_SHIFT (10)
#define SQ10_SHIFT (15)
#define SQ11_SHIFT (20)
#define SQ12_SHIFT (25)

//ADC1-SQR3 bit description
#define SQ1_SHIFT (0)
#define SQ2_SHIFT (5)
#define SQ3_SHIFT (10)
#define SQ4_SHIFT (15)
#define SQ5_SHIFT (20)
#define SQ6_SHIFT (25)

//ADC1->SR bit description
#define AWD    (1<<0)
#define EOC    (1<<1)
#define JEOC   (1<<2)
#define JSTRT  (1<<3)
#define STRT   (1<<4)

//DMAx_Channelx->CCR bit description
#define EN	(1<<0)
#define TCIE	(1<<1)
#define HTIE	(1<<2)
#define TEIE	(1<<3)
#define DIR	    (1<<4)
#define CIRC	(1<<5)
#define PINC	(1<<6)
#define MINC	(1<<7)
#define PSIZE_SHIFT	(8)
#define PSIZE_IS_8BITS (0 << PSIZE_SHIFT)
#define PSIZE_IS_16BITS (1 << PSIZE_SHIFT)
#define PSIZE_IS_32BITS (2 << PSIZE_SHIFT)
#define MSIZE_SHIFT	(10)
#define MSIZE_IS_8BITS (0 << MSIZE_SHIFT)
#define MSIZE_IS_16BITS (1 << MSIZE_SHIFT)
#define MSIZE_IS_32BITS (2 << MSIZE_SHIFT)
#define PL_SHIFT	(12)
#define PL_IS_LOW   (0<<PL_SHIFT)
#define PL_IS_MEDIUM   (1<<PL_SHIFT)
#define PL_IS_HIGH   (2<<PL_SHIFT)
#define PL_IS_VERYHIGH   (3<<PL_SHIFT)
#define MEM2MEM	(1<<14)


void Watch_For_Lower_Than(short int gap);
void Watch_For_Higher_Than(short int gap);

void Init_ADC (void) 
{
  	Current_ADC = 0;

	ADC_min = 0xFFFF;
	ADC_max = 0;

  	RCC->APB2ENR |= RCC_APB2Periph_ADC1;            // enable peripheral clock for ADC1
	// switch on adc module
	ADC1->CR2 |= ADON ;

 	// continuous scan of channels 1,14,15
  	ADC1->CR1  |= (SCAN);                      
  	//ADC1->CR1  |= EOCIE;                       // use independant mode, EOCIE mode
  	ADC1->CR2  |= (EXTSEL_ON_SWSTART | CONT | DMA);  // use data align right,continuous conversion
                                               // EXTSEL = SWSTART 
                                               // enable ADC, no external Trigger
											   // send DMA request
 
  	ADC1->SQR1 |= ((3-1) & L_MASK)<<L_SHIFT;        // sequence of 3 conversion
  	//	ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  	
	// channel 1 then 14 then 15
	ADC1->SQR3  |= ((1 & SQX_MASK)<<SQ1_SHIFT) | ((14 & SQX_MASK)<<SQ2_SHIFT) | ((15 & SQX_MASK)<<SQ3_SHIFT);                       // set channel1 as 1st conversion

  

	// setup ADC guard on channel 14
	ADC1->CR1 |= 14;
	
	
	// select single regular channel for guard
	ADC1->CR1 |= (AWDSGL | AWDEN);
	ADC1->CR1 &= ~(JAWDEN);
	
	// valid guard interrupt flag
	//ADC1->CR1 |= (AWDIE);


	//DMA configuration
	RCC->AHBENR |=0x00000001;// DMA1 CLOCK ENABLE
	DMA1_Channel1->CPAR =(u32)&(ADC1->DR); // @ source 
 	DMA1_Channel1->CCR |= (PSIZE_IS_16BITS | MSIZE_IS_16BITS);   //ADC1-CR is 16 bits 
  	DMA1_Channel1->CMAR =(u32) ADC_Array;   // @destination
	DMA1_Channel1->CCR &= ~(DIR); //from periph to memory		
  	DMA1_Channel1->CCR |=CIRC;   //Circular mode 
 	DMA1_Channel1->CNDTR=3;
  	DMA1_Channel1->CCR |=MINC;   //Circular mode 
  	DMA1_Channel1->CCR |=PL_IS_MEDIUM;	// priority level
 	
  	DMA1_Channel1->CCR |= EN;	// l'DMA bazar est en marche
 
 	ADC1->CR2  |=  (ADON);				   // start SW conversion

	phase = MONTE;
	Watch_For_Higher_Than(S_TH);
		

}

void Watch_For_Higher_Than(short int gap)
{
	// invalid guard interrupt flag
	ADC1->CR1 &= ~(AWDIE);

	//High and low guard value
	ADC1->HTR = (int)(gap) & 0xFFF;
	ADC1->LTR = 0;

	// valid guard interrupt flag
	ADC1->CR1 |= (AWDIE);

}
void Watch_For_Lower_Than(short int gap)
{
	// invalid guard interrupt flag
	ADC1->CR1 &= ~(AWDIE);

	//High and low guard value
	ADC1->HTR = 0xFFF;
	ADC1->LTR = (int)(gap) & 0xFFF;

	// valid guard interrupt flag
	ADC1->CR1 |= (AWDIE);

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
	
	if (ADC1->SR & AWD)
	{
		switch (phase)
		{
			case MONTE : 
				phase = AU_MAX;
				Watch_For_Lower_Than(S_H);
				break;
			case AU_MAX : 
				phase = DESCEND;
				Watch_For_Lower_Than(S_TB);
				break;
			case DESCEND : 
				phase = AU_MIN;
				Watch_For_Higher_Than(S_B);
				break;
			case AU_MIN : 
				phase = MONTE;
				Watch_For_Higher_Than(S_TH);
				break;
			default :
				break; 
		}
		
		ADC1->SR &= ~AWD;
	
	}

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
