#include <iostream>

#include "GameManager.hpp"


// constructeur
GameManager::GameManager(){
}

// constructeur
GameManager::GameManager(int w, int h){
  assert(w > 0);
  assert(h > 0);
  this->width = w;
  this->height = h;
}


int GameManager::getWidth(){
  return this->width;
}

int GameManager::getHeight(){
  return this->height;
}

char GameManager::charAtPosition(int x, int y){
  // retourne un char qui represente ce qui est dans cette position dans la grid
  int nombre_personnages = this->personnages.size();
  for(int i = 0; i < nombre_personnages; i++){
    if(x == this->personnages[i]->pos_x && y == this->personnages[i]->pos_y){
      return 'x';
    }
  }
  return '-';
}

// affichage
void GameManager::printGrid(){
	char gridPointChar = ' ';
	std::cout << "...printing grid:" << std::endl;
	for(int i = 0; i < this->width; i++){
		for(int j = 0; j < this->height; j++){
			gridPointChar = this->charAtPosition(i, j);
			std::cout << gridPointChar << ' ';
		}
		std::cout << std::endl;
	}
}

// affichage
void GameManager::printHPs(){
  float hp;
  std::cout << "...printing HPs:" << std::endl;
  int nombre_personnages = this->personnages.size();
  for(int i = 0; i < nombre_personnages; i++){
    hp = this->personnages[i]->getHP();
    std::cout << " -> " << hp << std::endl;
  }
}



void GameManager::addPersonnage(Agent* personnage){
  personnage->setBoundaries(this->width, this->height);
  personnage->setGameManager(this);
  this->personnages.push_back(personnage);
}


void GameManager::step(){
  int nombre_personnages = this->personnages.size();
  Action a;
  int attack_x;
  int attack_y;
  float attack_damage;
  
  // execution des actions des personnages
  for(int i = 0; i < nombre_personnages; i++){
    // choisir l'action
    a = this->personnages[i]->chooseAction();
    
    // faire l'action
    switch(a){
      case UP:
	this->personnages[i]->moveUp();
	break;
      case DOWN:
	this->personnages[i]->moveDown();
	break;
      case LEFT:
	this->personnages[i]->moveLeft();
	break;
      case RIGHT:
	this->personnages[i]->moveRight();
	break;
      case ATTACK:
	attack_damage = this->personnages[i]->getAttackForce();
	attack_x = this->personnages[i]->pos_x;
	attack_y = this->personnages[i]->pos_y;
	this->doDamageAroundPoint(attack_x, attack_y, attack_damage);
	break;
      default:
	break;
    }
  }
  
  // verifier si ils sont morts
  for(int i = 0; i < nombre_personnages; i++){
    if ( this->personnages[i]->getHP() <= 0.0){
      // il est mort
      this->personnages.erase(this->personnages.begin()+i);
    }
  }
}

void GameManager::doDamageAroundPoint(int x, int y, float attack_damage){
  int nombre_personnages = this->personnages.size();
  for(int i = 0; i < nombre_personnages; i++){
    // verifie pour les 8 carres autour du attaquant
    if(this->personnages[i]->pos_x == x + 1 && this->personnages[i]->pos_y == y + 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->pos_x == x + 1 && this->personnages[i]->pos_y == y){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->pos_x == x + 1 && this->personnages[i]->pos_y == y - 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->pos_x == x  && this->personnages[i]->pos_y == y + 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->pos_x == x  && this->personnages[i]->pos_y == y - 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->pos_x == x - 1 && this->personnages[i]->pos_y == y + 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->pos_x == x - 1 && this->personnages[i]->pos_y == y){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->pos_x == x - 1 && this->personnages[i]->pos_y == y - 1){
      this->personnages[i]->takeDamage(attack_damage);
    }
  }
}

void GameManager::train(){
  // executes the training of the Q tables
  // without showing on screen
  
  // un episode est un jeu avec (max_steps_per_episode) steps
  int max_episodes = 100;
  int max_steps_per_episode = 100;
  
  int num_learners;
  std::vector<Enemy*> learners;
  int pos_x; // pos initialle d'un learner
  int pos_y;
  
  // informations pour le state
  int dist_x_pers;
  int dist_y_pers;
  float hp_soi;
  float hp_pers;
  
  int episode_count, step_count;
  for( episode_count = 0; episode_count < max_episodes; episode_count ++){
    // TODO
    // reset personnages / learners
    learners.clear();
    // generer entre 2 et 5 learners
    num_learners = 2 + std::rand()%4;
    for(int i = 0; i < num_learners; i++){
      pos_x = std::rand()%this->width;
      pos_y = std::rand()%this->height;
      Enemy new_learner = Enemy(pos_x, pos_y);
      learners.push_back(&new_learner);
    }

    // reset autre chose de la grid?
    
    // executer la simulation pour l'aprentissage:
    for( step_count = 0; step_count < max_steps_per_episode; step_count ++){
      this->step(); // do one step
      // update q tables for all personnages / learners
      for(int i = 0; i < num_learners; i++){
	// calculate state information
	//dist_x_pers;
	//dist_y_pers;
	//hp_soi;
	//hp_pers;
	//learners[i]->updateQTable(dist_x_pers, dist_y_pers, hp_soi, hp_pers);
      }
    } 
  }
  std::cout << "...training complete" << std::endl;
  // print all q-tables
  Q_table qtable;
  for(int i = 0; i < num_learners; i++){
	qtable = learners[i]->getQTable();
	qtable.printTable();
  }
}
