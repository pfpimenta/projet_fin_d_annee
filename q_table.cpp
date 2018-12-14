#include <iostream>

#include "q_table.h"

class Q_table
{
	public:
	Q_table(){
	// constructeur
		for(int i = 0; i < NUM_STATES; i++){
			for(int j = 0; j < NUM_ACTIONS; J++){
				this->q_table[i][j] = 0.0f;
			}
		}
	}

	int takeAction(int state){
	// retourne le meilleur etat
		//rand_num = ?  
		if(rand_num > this->taux_exploration){
			// choisir la meilleure action
			int action_score[NUM_ACTIONS] = this->q_table[state];
			return 0;
		}else{
			// prendre l'action au hasard
			return 0;
		}
	}

}
