#ifndef _DRIVEURS_H
#define _DRIVEURS_H
//=================================================================================================
// auteurs: T.Rocacher, P. Acco, H. Malgouyres
// Périph pour le BE Métrottinettes
// Biliothèque qui permet de commander la trottinette (PWM1), de lire la vitesse (ADC P5.5)
// et de lire la position (deux optocoupleurs en quadrature  sur P2.8 et P2.9)
//__________________________________________________________________________________________________
// VERSIONS
//__________________________________________________________________________________________________
//[Rocacher 26 sept 2006] création de lib_capcom_v3 
//+ [Acco sept 2006] Inclusion des LEDS et de lib__capcom (thierry v3_1) => driveurs.c/h 
//+ [Malgouyres dec 2006] Rend le biblithèque de périph indépendante de l'application 	
//   Lire_Position => retourne un entier nbr pas et plus distance en float (on ne connait pas le pas)
//   Fixe_Vitesse -> Fixe_Rapport => prend le rapport cyclique au format +-1 et non plus la vitesse equiv.
//+ [Acco sept 2007]
//   renomme  initLEDS au format ADA Init_Led : initialise les 8 Leds P2.0 à P2.7 éteintes
//   suprime modifierLED1 et 2  pour mettre à la place :
//        Toggle_Led(unsigned char Num_Led)
// 		  Set_Led(unsigned char Num_Led)
//        Reset_Led(unsigned char Num_Led)
//   ajout config des IT pour les OPTOS	dans Config_Capture
//   ajout de void Configurer_Tout_Les_Periphs(void); 
//=================================================================================================

// DEFINISSEZ ICI le niveau et le groupe des fonctions d'IT des OPTOS 
//    IT_CC8 (no 18) et IT_CC9 (no 19)
// même niveau pour les deux fonctions et GLVL IT_CC9 = GLVL IT_CC8 +1
#define OPTO_IT_ILVL 4	 // niveau entre 0 et 15
#define OPTO_IT_GLVL 0   // doit être 0,1 ou 2


float Lire_Vitesse();
//________________________________________________________________
// ROLE : renvoie la vitesse en m/s (float). ADC en scrutation
// DUREE : 33us environ
// TEST (résumé) : SIMU seule. Testée pour 0V, 2.5V et 5V
//________________________________________________________________


int Lire_Position(void);
//________________________________________________________________
// ROLE :  permettent de lire position en nbr de pas. Elle est 
//         donnée modulo 30000 pas 	
//        1 pas vaut 2.PI/96 d'angle de roue soit environ 4.25mm  pour une roue de 13cm de diametre.
// DUREE : 23us	
// TEST (résumé) : SIMU seule, test2.ini pour les stimuli d'optocoupleur, 
//			opto() pour le sens up
//			optorev() pour le sens down. Pour plus d'info sur les tests voir directement 
//			les commentaires du .c.
//________________________________________________________________

void Fixe_Rapport(float Sigma);
//________________________________________________________________
// ROLE : donne la consigne de courrant à la trottinette [-1 à +1]
// 		  saturation non gérée sur le float. La saturation de sortie est gérée [1 1023] 
//        L'arret de la tritinnette est commandé par "Fixe_Courrant(0);"
// DUREE : 50us environ (du au calcul flottant vers int)
// TEST (résumé) :testé en simu. conforme pour des valeurs de -1 à +1 OK
// testé pour -50.0 et +50.0 (n'importe quoi !) ok, saturation correcte de la PWM 
//________________________________________________________________


//________________________________________________________________
// ROLE : Change l'état des leds
// modifie le led indiquée par Num_Led
//     Num_Led = 0 à 7  => P2.0 à P2.7
// Set => Allume, Reset=> Etteint, Toggle => état opposé  
// DUREE : ?
//________________________________________________________________
void Set_Led(unsigned char Num_Led);
void Reset_Led(unsigned char Num_Led);
void Toggle_Led(unsigned char Num_Led); 


//________________________________________________________________
// ROLE : Initialise tous les periph de cette librairie,
//      Initialise la position à 0 pas
//      Fixe le rapport à 0 : courant nul dans le moteur 
//      puis  autorise les interruptions des optocoupleurs
// DUREE : ?
//________________________________________________________________
void Configurer_Tout_Les_Periphs(void);


void Init_Position(int Position_Initiale);
//________________________________________________________________
// ROLE : Positionne la trottinette, valeur en pas obligatoirement positive et < 30000
//        1 pas vaut environ 4.25mm  .
//		  Si la valeur n'est pas dans ce cadre, on procède à une saturation (0 et 30000) 
// DUREE : 67us	
// TEST (résumé) : SIMU seule, fonctionne correctement, pour plus s'infos voir directement 
//			      les commentaires du .c.
//________________________________________________________________


void Config_ADConverter(void);
//________________________________________________________________
// ROLE : Règle l'ADC en single, sur la voie P5.5, règle l'aquisition  
//		  aux evirons de 10us.
// DUREE : non mesurée, pas important
// TEST (résumé) : voir fonction suivante
//________________________________________________________________


void Config_Capture(void);
//________________________________________________________________
// ROLE : initialise l'unité CAPCOM sur les voies P2.8 et P2.9
//		  En interne 2 fonctions d'interruption gèrent les captures CC8 et CC9
//		   !! gérer complètement les IT dans le main !!
// DUREE : non mesurée, pas important
// TEST (résumé) : voir fonction suivante
//________________________________________________________________

void Config_PWM(void);
//________________________________________________________________
// ROLE : Configure l'unité PWM 1  (P7.1 ), à 20kHz, PWM de 1 à 1023 inclus
// DUREE : non mesurée, pas important	
// TEST (résumé) : Voir fonction suivante 
//________________________________________________________________


void Init_Led(void);
//________________________________________________________________
// ROLE : Initialise le port 2 pour les 2 LEDS. Les 2 LEDs sont 
//   éteintes au départ
// DUREE : ?
//________________________________________________________________

														



//=================================================================================================
//
//       Commentaires théoriques (rapport entre tr/min, m.s, km/h opto....
//
//=================================================================================================
 

//___________________Capture___________________________________
//
// calcul théorique
//------------------
// La fréquence maximale des pulses issus des opto est définie de la manière suivante :
// diamètre de la roue # 13cm.
// nombre de pas : 24 lignes noires, 24 balanches => 24 périodes.
//                 résolution * 4 en prenant tous les fronts, sur les 2 signaux
//					= 96 pulses par tours
// vitesse maxi = 12km/h
// 
// => Froue = Oméga/2pi = Vlin/(2.pi.Droue/2)= Vlin/(pi.Droue)
// et Vlin (m/s) = V(km/h). 1000/3600 = V(km/h)/3.6
// => Froue = Vlin(km/h)/(pi.Droue.3,6) 
// AN à 12km/h, Froue = 8,16 Hz.
//
// Le nombre de pas étant 96, la fréquence du signal issu des opto vaut 784Hz environ.
// Cela signifie qu'à plein vitesse, il s'écoule 1/784 = 1,275 ms entre deux pulses.
//
// Un pulse correspond à une avancée de pi.D/96 = 4,25mm.
// Le comptage d'impulsion se fera modulo 30 000 (127,6m)

// valeurs à préciser sur les pas

// La mesure se fait sur IT fronts desc et montant de P2.8 et P2.9.






#endif