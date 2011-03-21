#define IN 0x8 // input with pulldown
#define OUT 0x2 // output 2MHz with push/pull

unsigned int * Cr = (unsigned int *)  0x40010C00; // CRL of GPIOB (STM32f10x) 
unsigned int * Idr = (unsigned int *) 0x40010C08; // ODR
unsigned int * Odr = (unsigned int *) 0x40010C0C; // IDR

unsigned char Boutons_Lus;

void Wait(int);

int main(void)
{
	unsigned char Pas_Encore ; 
	unsigned int mask;

	// configure |PORT.3 	  |PORT.2 	    |PORT.1 	  |PORT.0
	//    as     |In	  	  |Out    	    |Out    	  |In
	//___________|____________|_____________|_____________|___________
	//  for      |Player1  	  |Player1  	|Player 2     |Player 2
	//           |Button      |Led 		    |Led		  |Button
	mask = 	      (IN<<(3*4)) +(OUT<<(2*4)) +(OUT<<(1*4)) +(IN<<(0*4));
	*Cr = mask;



	while(1)
	{
		Pas_Encore = 'R'; // "Run" car les joueurs n'ont pas encore appuyé sur une touche

		// Fait clignoter les LEDs en attendants que les players appuient
		while (Pas_Encore)  // rappel : 0=> false ; tout le reste (et donc 'R')  => true 
		{
			 
			// Teste si au moins un bouton est appuyé :
			// on teste donc si IDR.0 (bouton player 1) 
			//    ou IDR.3 (bouton player 2 ) est à 1
			// gràce au masque binaire 2_00001001 = 0x9
			Boutons_Lus = *Idr & 0x9 ;  // mets tous les bits à 0 sauf les bits .0 et .3
			
			// Boutons_Lus vaut 0 (!False) si aucun bouton appuyé  et true sinon  
			if (Boutons_Lus) Pas_Encore =0;

			// Change état des diodes avec un XOR bit à bit
			//  du masque 2_00000110
			*Odr ^= 0x6   ;
		}

		//etteinds les diodes
		*Odr &=  ~(6); //~  est l'inversion de chaque bits

		// affiche le vainqueur ou ex-aequo
		if (Boutons_Lus & (1<<0)) *Odr |= (1<<1);
		if (Boutons_Lus & (1<<3)) *Odr |= (1<<2);
		  
		// attends qu'ils aient vu le resultat
		Wait(1000000) ;

		//etteinds les diodes
		*Odr &=  ~(6); //~  est l'inversion de chaque bits

		// les joueurs attendent le signal pour appuyer
		Wait(3000000) ;


	}
  
}

// Boucle de temporisation 
void Wait( int N)
{ 
	volatile int cpt ;
	
	cpt=0;
	for (cpt=0;cpt<N;cpt++);

}

