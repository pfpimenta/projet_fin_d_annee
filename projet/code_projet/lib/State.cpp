#include <iostream>
#include <vector>

#include "State.hpp"

// returns the state given the feature information
int getState(int dist_x_pers, int dist_y_pers, float hp_soi, float hp_pers){
// retourne l'etat
    int state = 0;

		std::vector<int> cumul_num_states;
		cumul_num_states.push_back(getNumStatesOfFeature(0));
		for(int i = 1; i < NUM_FEATURES; i++){
			cumul_num_states[i] = cumul_num_states[i-1] * getNumStatesOfFeature(i);
		}

    //std::cout << "DEBUG state dx: "<<dist_x_pers<<" dy: " <<dist_y_pers;
    //std::cout << " HPs: "<<hp_soi<<" HPp: " <<hp_pers<< '\n';

    // dist_x_pers
    if(dist_x_pers <= -2){
        state = state;
    }else if(dist_x_pers == -1){
        state = state + 1;
    }else if(dist_x_pers == 1){
        state = state + 2;
    }else if(dist_x_pers >= 2){
        state = state + 3;
    }
    // dist_y_pers
    if(dist_y_pers <= -2){
        state = state;
    }else if(dist_y_pers == -1){
        state = state + 1*cumul_num_states[0];
    }else if(dist_y_pers == 1){
        state = state + 2*cumul_num_states[0];
    }else if(dist_y_pers >= 2){
        state = state + 3*cumul_num_states[0];
    }
    // hp_soi
    if(hp_soi <= 25){
      state = state + cumul_num_states[1];
    }
    // hp_pers
    if(hp_pers <= 25){
      state = state + cumul_num_states[2];
    }
    return state;
}

// return the total number of states
int getNumStates(){
	int num_states = 1;
	for(int i = 0; i < NUM_FEATURES; i++){
		num_states = num_states * getNumStatesOfFeature(i);
	}
	return num_states;
}

// return the number of states of a given feature
int getNumStatesOfFeature(int featureIndex){

	//discretization:
	// * dist_x_pers : 4
	//une case positif,
	//une case negatif,
	//plus que une positif
	//plus que une negatif
	// * dist_y_pers : 4
	//une case positif,
	//une case negatif,
	//plus que une positif
	//plus que une negatif
	// * hp_soi : moins ou plus de 25% (2)
	// * hp_pers : moins ou plus de 25% (2)
	// ** total num etats : 4*4*2*2 =

	switch(featureIndex){
		case 0:
			return 4;
		case 1:
			return 4;
		case 2:
			return 2;
		case 3:
			return 2;
		default:
		std::cout << "ERROR !!! il n'y a pas une feature de index "<< featureIndex << std::endl;
		std::cout << "...il n'y a que "<< NUM_FEATURES <<" features" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
