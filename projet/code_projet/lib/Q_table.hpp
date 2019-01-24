#pragma once

#ifndef Q_TABLE_HPP
#define Q_TABLE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "utils.hpp"

#define NUM_STATES 100
#define NUM_ACTIONS 5
#define TAUX_EXPLORATION 0.1
#define LEARNING_RATE 0.05
#define DISCOUNT_VALUE 0.99 // changer

enum QTableAction {QUP, QDOWN, QLEFT, QRIGHT, QATTACK};



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
			QTableAction takeAction(int state); // retourne la meilleure action dans un etat donne
			QTableAction takeActionTraining(int state); // retourne la meilleure action dans un etat donne + exploration
			void printTable(); // affiche la Q table
			void printTableBestActions(); // afficher la "meilleure" action pour chaque etat
			void update_table(int action, int lastState, int currentState, float recompense); // actualise le tableau Q
			void saveTable(std::string tableName);
			void loadTable(std::string tableName);
};

#endif
