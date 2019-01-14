#pragma once

#ifndef Q_TABLE_HPP
#define Q_TABLE_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>

#define NUM_STATES 5 // changer
#define NUM_ACTIONS 5 // changer
#define TAUX_EXPLORATION 0.1 // changer
#define LEARNING_RATE 0.05 // changer
#define DISCOUNT_VALUE 0.9 // changer



class Q_table
{
	float taux_exploration; // pourcentage de prendre une action au hasard
	float learning_rate;
	float gamma; // hyperparametre : discount value
	//float q_table[NUM_STATES][NUM_ACTIONS]; // la q-table en soi
	float** q_table; // la q-table en soi
	int num_states;
	int num_actions;

	public:
	Q_table(); // constructeur
	Q_table(int n_states, int n_actions); // constructeur avec taille variable
	int takeAction(int state); // retourne la meilleure action dans un etat donne
	void printTable(); // affiche la Q table
	void update_table(int action, int etat_courrant, int etat_suivant, float recompense); // actualise le tableau Q
};

#endif
