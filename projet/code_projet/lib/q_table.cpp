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
          // TODO ???
            action_score[j] = this->q_table[state][j];
        }
        return 0;
    }else{
        // prendre l'action au hasard
        return 0;
    }
}

int Q_table::getState(int dist_x_pers, int dist_y_pers, float hp_soi, float hp_pers){
// retourne l'etat

    //discretization:
    // dist_x_pers : une case, entre deux e cinc, plus que cinc (3)
    // dist_y_pers : une case, entre deux e cinc, plus que cinc (3)
    // hp_soi : moins ou plus de 25% (2)
    // hp_pers : moins ou plus de 25% (2)
    // total num etats : 3*3*2*2 = 36

    int state = 0;
    // dist_x_pers
    if(dist_x_pers >= 2 && dist_x_pers <= 5){
        state = state + 1;
    }else if(dist_x_pers > 5){
        state = state + 2;
    }
    // dist_y_pers
    if(dist_y_pers >= 2 && dist_y_pers <= 5){
        state = state + 3;
    }else if(dist_y_pers > 5){
        state = state + 6;
    }
    // hp_soi
    if(hp_soi <= 25){
        state = state + 9;
    }
    // hp_pers
    if(hp_pers <= 25){
        state = state + 18;
    }

    return state;
}
