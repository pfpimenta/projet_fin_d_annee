#include <iostream>

#include "Q_table.hpp"


// constructeur
Q_table::Q_table(): num_states(NUM_STATES){
	this->taux_exploration = TAUX_EXPLORATION;
	this->learning_rate = LEARNING_RATE;
	this->gamma = DISCOUNT_VALUE;
	//this->num_states = NUM_STATES;
	this->num_actions = NUM_ACTIONS;
    std::cout << "DEBUG NUM_STATES: " << NUM_STATES << std::endl;
    std::cout << "DEBUG NUM_ACTIONS: " << NUM_ACTIONS << std::endl;
    std::cout << "DEBUG num_states: " <<this->num_states << std::endl;
    std::cout << "DEBUG num_actions: " <<this->num_actions << std::endl;

	// set table size
	this->q_table = new float * [NUM_STATES];
	for (int i = 0; i < NUM_STATES; ++i)
	    this->q_table[i] = new float[NUM_ACTIONS];
	// initialize with zeros
	for(int i = 0; i < NUM_STATES; i++){
		for(int j = 0; j < NUM_ACTIONS; j++){
			this->q_table[i][j] = 0.0f;
		}
	}
	// seed pour des nombres aleatoires ?
}

// constructeur avec taille variable
Q_table::Q_table(int n_states, int n_actions){
	this->taux_exploration = TAUX_EXPLORATION;
	this->learning_rate = LEARNING_RATE;
	this->gamma = DISCOUNT_VALUE;
	this->num_states = n_states;
	this->num_actions = n_actions;
  std::cout << "DEBUG num_states: " << this->num_states << std::endl;
  std::cout << "DEBUG num_actions: " << this->num_actions << std::endl;

	// set table size
	this->q_table = new float *[this->num_states];
	for (int i = 0; i < this->num_states; ++i)
	    this->q_table[i] = new float[this->num_actions];
	// initialize with zeros
	for(int i = 0; i < this->num_states; i++){
		for(int j = 0; j < this->num_actions; j++){
			this->q_table[i][j] = 0.0f;
		}
	}
	// seed pour des nombres aleatoires ?
}

// affichage
void Q_table::printTable(){
	std::cout << "...printing Q_table ("<<this->num_states<<" states X "<<this->num_actions<<" actions):" << std::endl;
	for(int i = 0; i < this->num_states; i++){
		for(int j = 0; j < this->num_actions; j++){
			std::cout << this->q_table[i][j] << ",";
		}
		std::cout << std::endl;
	}
}

// actualise le tableau Q
void Q_table::update_table(int action, int etat_courrant, int etat_suivant, float recompense)
{

  float lr = this->learning_rate;
	std::cout << "DEBUG debut update_table: " << etat_courrant<< "  " << (int)action << std::endl;

  float old_value = this->q_table[etat_courrant][action];

  // calculer le max value du next state
  float max_next_state = this->q_table[etat_suivant][0];
  for(int i = 1; i < this->num_actions; i++)
  {
    if(this->q_table[etat_suivant][i] > max_next_state)
      max_next_state = this->q_table[etat_suivant][i];
  }
  // formule du Q-learning
  this->q_table[etat_courrant][action] =  (1-lr)*old_value + lr*(recompense + this->gamma * max_next_state);
}

Action Q_table::takeAction(int state){
		// retourne le meilleur etat
    float rand_num = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX); // nb entre 0 et 1

    float max_action_score;
		int action;
    if(rand_num > this->taux_exploration){
        // choisir la meilleure action
				max_action_score = this->q_table[state][0];
				action = 0;
        for(int j = 1; j < this->num_actions; j++){
					if(this->q_table[state][j] > max_action_score){
            max_action_score = this->q_table[state][j];
						action = j;
					}
        }
    }else{
        // prendre l'action au hasard
				int random_variable = std::rand()%5;
				action = random_variable;
    }
		return (Action) action;
}
