#pragma once

#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>

#define NUM_FEATURES 4 // dist_x_pers, dist_y_pers, hp_soi, hp_pers

// returns the state given the feature information
int getState(int dist_x_pers, int dist_y_pers, float hp_soi, float hp_pers);
// return the total number of states
int getNumStates();
// return the number of states of a given feature
int getNumStatesOfFeature(int featureIndex);

#endif
