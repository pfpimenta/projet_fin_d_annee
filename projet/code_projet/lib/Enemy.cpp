#include <iostream>

#include "Enemy.hpp"


// constructeur
Enemy::Enemy(){
  // jamais utilisee, je pense
}

// constructeur
Enemy::Enemy(int x, int y){
  this->pos_x = x;
  this->pos_y = y;
  
  this->setHP(HP_MAX);
  this->setAttackForce(DEFAULT_ATTACK_FORCE);
  
  // asserts
   assert(this->getHP() > 0 && printf("hp doit etre positif"));
   assert(this->pos_x > 0 && printf("position x doit etre positif"));
   assert(this->pos_y > 0 && printf("position y doit etre positif"));
   assert(this->getAttackForce() > 0 && printf("attack_force doit etre positive"));
}




// choose action (overcharge)
Action Enemy::chooseAction(){
  Action action;

  // action aleatoire (test)
  //int random_variable = std::rand()%5;
  //action = (Action) random_variable;

  // TODO : action avec la Q table
  
  
  // DEBUG pour tester la variable gm
  //int w = gm->getWidth();
  //gm->printHPs();
  //std::cout << "debug wwwww : " << w << std::endl;

  return action;
}