#include <iostream>

#include "Agent.hpp"


// constructeur
Agent::Agent(){
  // jamais utilise, je pense
}

// constructeur
Agent::Agent(int x, int y){
  this->pos_x = x;
  this->pos_y = y;
  
  this->hp = HP_MAX;
}



// take damage
void Agent::takeDamage(float damage){
  this->hp = this->hp - damage;
}


// choose action
Action Agent::chooseAction(){
  Action action = UP;
  return action;
}

// choose action
void Agent::moveUp(){
  this->pos_x -= 1;
  return;
}