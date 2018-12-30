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
	attack_damage = this->personnages[i]->attack();
	attack_x = this->personnages[i]->pos_x;
	attack_y = this->personnages[i]->pos_y;
	this->doDamageAroundPoint(attack_x, attack_y, attack_damage);
	break;
      default:
	break;
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