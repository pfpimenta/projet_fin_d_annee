#include <iostream>

#include "Enemy.hpp"


// constructeur
Enemy::Enemy(){
  // jamais utilisee
  std::cout << "ERROR !!! on a appele le constructeur trompe pour Enemy" << std::endl;
  std::exit(EXIT_FAILURE);
}

// constructeur
Enemy::Enemy(int x, int y){
  this->pos_x = x;
  this->pos_y = y;
  
  this->setHP(HP_MAX);
  this->setAttackForce(DEFAULT_ATTACK_FORCE);
  //this->qTable = Q_table(); // nombre d'actions et states par default
    
  // asserts
   assert(this->getHP() > 0); //hp doit etre positif
   assert(this->pos_x >= 0); //position x doit etre positif
   assert(this->pos_y >= 0); //position y doit etre positif
   assert(this->getAttackForce() > 0); // attack force doit etre positif
}




// choose action (overcharge)
Action Enemy::chooseAction(int dist_x_pers, int dist_y_pers, float hp_pers){
  Action action;

  // action aleatoire (test)
  //int random_variable = std::rand()%5;
  //action = (Action) random_variable;

  // TODO : action avec la Q table
  int state = this->qTable.getState(dist_x_pers, dist_y_pers, this->getHP(), hp_pers);
  action = (Action) this->qTable.takeAction(state);
  
  
  // DEBUG pour tester la variable gm
  //int w = gm->getWidth();
  //gm->printHPs();
  //std::cout << "debug wwwww : " << w << std::endl;

  return action;
}


Q_table Enemy::getQTable(){
  return this->qTable;
}
