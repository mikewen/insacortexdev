/* 
 * Fichier main, en assembleur 
 */
	
	.section ".text"
	.align	2

	.global	main
	.type	main, %function

main:
/* Debut de la routine main */
	
/* 
 * Code de l'appli a mettre ici 
 */

/*
 * Boucle infinie, pour ne pas sortir de la routine main
 */

	B		.  

/*
 * Constantes utilisées par main	
 * Stockées en section .text, après la routine
 */
	/* .align	2 inutile, normalement */
	.section ".text"	
main_constants:
	/* Constantes a mettre ici */
	
	.end
