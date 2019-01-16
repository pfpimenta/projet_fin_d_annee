#include <iostream>

#include "Learner.hpp"


// constructeur
Learner::Learner(){
  // jamais utilisee
  std::cout << "ERROR !!! on a appele le constructeur trompe pour Learner" << std::endl;
  std::exit(EXIT_FAILURE);
}

// constructeur avec position
Learner::Learner(int x, int y){
  this->estMort = false;
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
Learner::Learner(int x, int y, Q_table* q_table){
  this->estMort = false;
  this->pos_x = x;
  this->pos_y = y;

  this->setHP(HP_MAX);
  this->setAttackForce(DEFAULT_ATTACK_FORCE);
  this->qTable = q_table;
  //this->lastAction = UP;

  //std::cout<<"DEBUG learner at x, y :"<<pos_x<<" , "<<pos_y<<'\n';

  // asserts
   assert(this->getHP() > 0); //hp doit etre positif
   assert(this->pos_x >= 0); //position x doit etre positif
   assert(this->pos_y >= 0); //position y doit etre positif
   assert(this->getAttackForce() > 0); // attack force doit etre positif
}


// choose action
Action Learner::chooseAction(){
  Action action;
  // action aleatoire (test)
  int random_variable = std::rand()%5;
  action = (Action) random_variable;
  this->lastAction = action;
  //std::cout << "DEBUG update lastAction: " << this->lastAction << std::endl;
  return action;
}


// choose action (overcharge)
Action Learner::chooseAction(int dist_x_pers, int dist_y_pers, float hp_pers){
  Action action;
  // action aleatoire (test)
  //int random_variable = std::rand()%5;
  //action = (Action) random_variable;
  int state = getState(dist_x_pers, dist_y_pers, this->getHP(), hp_pers);
  action = (Action) this->qTable->takeAction(state);
  //std::cout << "DEBUG action "<< (int)action <<'\n';

  this->lastAction = action;

  return action;
}

int Learner::findClosestEnemy(std::vector<Learner*> learners){
  int closestEnemyIndex = 0;
  int dist_x, dist_y, current_distance, current_min_distance;

  //std::cout << "DEBUG x, y :"<< this->pos_x<<" , " << this->pos_y << '\n';

  if(learners[0] != this){
    closestEnemyIndex = 0;
    dist_x = learners[0]->pos_x - this->pos_x;
    dist_y = learners[0]->pos_y - this->pos_y;
    current_min_distance = dist_x*dist_x + dist_y*dist_y;
  }else{
    closestEnemyIndex = 1;
    dist_x = learners[1]->pos_x - this->pos_x;
    dist_y = learners[1]->pos_y - this->pos_y;
    current_min_distance = dist_x*dist_x + dist_y*dist_y;
  }
  for(int i = 1; i < (int)learners.size(); i++){
    dist_x = learners[i]->pos_x - this->pos_x;
    dist_y = learners[i]->pos_y - this->pos_y;
    current_distance = dist_x*dist_x + dist_y*dist_y;
    if(current_distance < current_min_distance){
      current_min_distance = current_distance;
    }
  }
  //std::cout << "DEBUG dist_x , dist_y :"<< dist_x<<" , " <<dist_y<< '\n';
  return closestEnemyIndex;
}

// mourir
void Learner::die(){
  this->estMort = true;
}
bool Learner::isDead(){
  return this->estMort;
}

// ce learner a tue qqn
void Learner::killedSomeone(){
  this->aTue = true;
}
// return true si ce learner a tue qqn
bool Learner::hasKilledSomeone(){
  return this->aTue;
}

float Learner::getReward(){
  float reward = 0.0f;
  //reward = std::rand() / static_cast <float> (RAND_MAX); // DEBUG
  if(this->estMort){
    reward = -1;
  }else if(this->aTue){
    reward = 1;
    this->aTue = false; // reset
  }
  return reward;
}

//
// setters et getters
//
Q_table* Learner::getQTable(){
  return this->qTable;
}
void Learner::setQTable(Q_table* q_table){
  this->qTable = q_table;
}

int Learner::getLastState(){
  return this->lastState;
}
void Learner::setLastState(int state){
  this->lastState = state;
}

Action Learner::getLastAction(){
  return this->lastAction;
}
void Learner::setLastAction(Action action){
  this->lastAction = action;
}
