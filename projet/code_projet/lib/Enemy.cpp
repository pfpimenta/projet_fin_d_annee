#include <iostream>

#include "Enemy.hpp"


// constructeur
Enemy::Enemy(){
  // jamais utilisee
  std::cout << "ERROR !!! on a appele le constructeur trompe pour Enemy" << std::endl;
  std::exit(EXIT_FAILURE);
}

// constructeur avec position
Enemy::Enemy(int x, int y){
  this->pos_x = x;
  this->pos_y = y;

  this->setHP(HP_MAX);
  this->setAttackForce(DEFAULT_ATTACK_FORCE);
  Q_table q_table = Q_table(); // nombre d'actions et states par default
  this->qTable = &q_table; // nombre d'actions et states par default
  this->lastAction = UP; // TODO enlever cette ligne

  // asserts
   assert(this->getHP() > 0); //hp doit etre positif
   assert(this->pos_x >= 0); //position x doit etre positif
   assert(this->pos_y >= 0); //position y doit etre positif
   assert(this->getAttackForce() > 0); // attack force doit etre positif
}

// constructeur avec position et q_table pointer
Enemy::Enemy(int x, int y, Q_table* q_table){
  this->pos_x = x;
  this->pos_y = y;

  this->setHP(HP_MAX);
  this->setAttackForce(DEFAULT_ATTACK_FORCE);
  this->qTable = q_table;
  //this->lastAction = UP;

  // asserts
   assert(this->getHP() > 0); //hp doit etre positif
   assert(this->pos_x >= 0); //position x doit etre positif
   assert(this->pos_y >= 0); //position y doit etre positif
   assert(this->getAttackForce() > 0); // attack force doit etre positif
}




// choose action
Action Enemy::chooseAction(){
  Action action;
  // action aleatoire (test)
  int random_variable = std::rand()%5;
  action = (Action) random_variable;
  this->lastAction = action;
  std::cout << "DEBUG update lastAction: " << this->lastAction << std::endl;
  return action;
}

// choose action (overcharge)
Action Enemy::chooseAction(int dist_x_pers, int dist_y_pers, float hp_pers){
  Action action;

  // action aleatoire (test)
  //int random_variable = std::rand()%5;
  //action = (Action) random_variable;

  // TODO : action avec la Q table
  int state = getState(dist_x_pers, dist_y_pers, this->getHP(), hp_pers);
  action = (Action) this->qTable->takeAction(state);

  // DEBUG pour tester la variable gm
  //int w = gm->getWidth();
  //gm->printHPs();
  //std::cout << "debug wwwww : " << w << std::endl;

  this->lastAction = action;

  return action;
}

void Enemy::updateQTable(int dist_x_pers, int dist_y_pers, float hp_soi, float hp_pers){
    //updates the Q-table associated with this mec
    // TODO
  int state = getState(dist_x_pers, dist_y_pers, hp_soi, hp_pers);
  //this->qTable->update_table(action, etat_courrant, etat_suivant, recompense); // actualise le tableau Q

}

// setters et getters
Q_table* Enemy::getQTable(){
  return this->qTable;
}
void Enemy::setQTable(Q_table* q_table){
  this->qTable = q_table;
}

int Enemy::getLastState(){
  return this->lastState;
}
void Enemy::setLastState(int state){
  this->lastState = state;
}

Action Enemy::getLastAction(){
  return this->lastAction;
}
void Enemy::setLastAction(Action action){
  this->lastAction = action;
}
