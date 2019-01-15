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

  //std::cout<<"DEBUG enemy at x, y :"<<pos_x<<" , "<<pos_y<<'\n';

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
  //std::cout << "DEBUG update lastAction: " << this->lastAction << std::endl;
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

int Enemy::findClosestEnemy(std::vector<Enemy*> learners){
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


//
// setters et getters
//
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
