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
  this->attack_force = 15.7;
  
  // asserts
   assert(this->hp > 0 && printf("hp doit etre positif"));
   assert(this->pos_x > 0 && printf("position x doit etre positif"));
   assert(this->pos_y > 0 && printf("position y doit etre positif"));
   assert(this->attack_force > 0 && printf("attack_force doit etre positive"));
}


// take damage
void Agent::takeDamage(float damage){
  this->hp = this->hp - damage;
  //std::cout << "debug hp : " << this->hp << std::endl;
}


// choose action
Action Agent::chooseAction(){
  Action action;

  // action aleatoire (test)
  int random_variable = std::rand()%5;
  action = (Action) random_variable;

  // TODO : action avec la Q table
  // ou input du joueur
  // ===> ca va etre dans des subclasses Enemy et Player (?)
  
  // DEBUG pour tester la variable gm
  //int w = gm->getWidth();
  //gm->printHPs();
  //std::cout << "debug wwwww : " << w << std::endl;
  
  return action;
}

//  UP
void Agent::moveUp(){
  if(this->pos_x > 0)
    this->pos_x -= 1;
  return;
}
// DOWN
void Agent::moveDown(){
if(this->pos_x < this->width - 1)
  this->pos_x += 1;
 return;
}
// LEFT
void Agent::moveLeft(){
  if(this->pos_y > 0)
    this->pos_y -= 1;
  return;
}
// RIGHT
void Agent::moveRight(){
if(this->pos_y < this->height - 1)
  this->pos_y += 1;
 return;
}
// ATTACK
// TODO ?


// setters et getters

// width et height du grid actuel (taille du grid)
void Agent::setBoundaries(int w, int h){
  this->width = w;
  this->height = h;
}

// GameManager // pas necessaire?
void Agent::setGameManager(GameManager* gameManager){
  this->gm = gameManager;
}

// HP (points de vie)
void Agent::setHP(float hp){
  this->hp = hp;
}
float Agent::getHP(){
  return this->hp;
}

// attack force
void Agent::setAttackForce(float attackForce){
  this->attack_force = attackForce;
}
float Agent::getAttackForce(){
  return this->attack_force ;
}
