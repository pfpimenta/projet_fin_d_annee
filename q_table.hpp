#pragma once

#ifndef Q_TABLE_HPP
#define Q_TABLE_HPP

#include <iostream>

#define NUM_STATES 5 // changer
#define NUM_ACTIONS 5 // changer

class Q_table
{
	float taux_exploration; // pourcentage de prendre une action au hasard
	float gamma; // hyperparametre
	float q_table[NUM_STATES][NUM_ACTION]; // la q-table en soi
	public:
	Q_table(); // constructeur
	int takeAction(int state); // retourne le meilleur etat
	void update_table(int action, int etat_courrant, int etat_suivant, float recompense); // actualise le tableau Q
	//int getState("toutes informations qui l'agent utilise") // a definir
}

#endif
