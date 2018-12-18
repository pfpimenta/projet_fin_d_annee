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