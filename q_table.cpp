#include <iostream>

#include "q_table.hpp"


// constructeur
Q_table::Q_table(){
	this->taux_exploration = TAUX_EXPLORATION;
	this->learning_rate = LEARNING_RATE;
	this->gamma = DISCOUNT_VALUE;
	for(int i = 0; i < NUM_STATES; i++){
		for(int j = 0; j < NUM_ACTIONS; j++){
			this->q_table[i][j] = 0.0f;
		}
	}
	
	// seed pour des nombres aleatoires
}

// affichage
void Q_table::printTable(){
	std::cout << "...printing Q_table:" << std::endl;
	for(int i = 0; i < NUM_STATES; i++){
		for(int j = 0; j < NUM_ACTIONS; j++){
			std::cout << this->q_table[i][j] << ",";
		}
		std::cout << std::endl;
	}
}

// actualise le tableau Q
void Q_table::update_table(int action, int etat_courrant, int etat_suivant, float recompense)
{
  float lr = this->learning_rate;
  float old_value = this->q_table[etat_courrant][action];
  // calculer le max value du next state
  float max_next_state = this->q_table[etat_suivant][0];
  for(int i = 1; i < NUM_ACTIONS; i++)
  {
    if(this->q_table[etat_suivant][i] > max_next_state)
      max_next_state = this->q_table[etat_suivant][i];
  }
  this->q_table[etat_courrant][action] =  (1-lr)*old_value + lr*(recompense + this->gamma * max_next_state);
}

int Q_table::takeAction(int state){
// retourne le meilleur etat
	float rand_num = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // nb entre 0 et 1
	std::cout << "...printing Q_table:" << std::endl;

	float action_score[NUM_ACTIONS];
	if(rand_num > this->taux_exploration){
		// choisir la meilleure action
	  	for(int j = 0; j < NUM_ACTIONS; j++){
			action_score[j] = this->q_table[state][j];
		}
		return 0;
	}else{
		// prendre l'action au hasard
		return 0;
	}
}
