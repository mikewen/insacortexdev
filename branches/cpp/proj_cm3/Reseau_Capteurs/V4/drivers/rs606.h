/*
 * Projet: Reseau de capteur
 *
 * Rôle: Gestion du module FM RS606
 * Version: 4.0
 */
#ifndef __RS606_H__
#define __RS606_H__

#include <stm32f10x_lib.h>

/* 
 * Definition des identifiants des differents mode de fonctionnement du RS606
 */
typedef enum 
{
	RS606_OFF, 
	RS606_TX, 
	RS606_RX, 
	RS606_TX_LOOPBACK
} RS606_MODE;

/*
 * Fonctions exportées (publiques)
 */
void RS606SetMode(RS606_MODE mode);

#endif /* __RS606_H__ */				
