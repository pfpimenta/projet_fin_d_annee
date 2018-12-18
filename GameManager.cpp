#include <iostream>

#include "GameManager.hpp"


// constructeur
GameManager::GameManager(){
}

// constructeur
GameManager::GameManager(int w, int h){
  this->width = w;
  this->height = h;
}


int GameManager::getWidth(){
  return this->width;
}

int GameManager::getHeight(){
  return this->height;
}

char GameManager::atPosition(int x, int y){
  // retourne un char qui represente ce qui est dans cette position dans la grid
  int nombre_personnages = this->personnages.size();
  for(int i = 0; i < nombre_personnages; i++){
    if(x == this->personnages[i].pos_x && y == this->personnages[i].pos_y){
      return 'x';
    }
  }
  return '-';
}

// affichage
void GameManager::printGrid(){
	char gridPoint = ' ';
	std::cout << "...printing grid:" << std::endl;
	for(int i = 0; i < this->width; i++){
		for(int j = 0; j < this->height; j++){
			gridPoint = this->atPosition(i, j);
			std::cout << gridPoint;
		}
		std::cout << std::endl;
	}
}



void GameManager::addPersonnage(Agent personnage){
  personnage.setBoundaries(this->width, this->height);
  this->personnages.push_back(personnage);
}


void GameManager::step(){
  int nombre_personnages = this->personnages.size();
  Action a;
  for(int i = 0; i < nombre_personnages; i++){
    a = this->personnages[i].chooseAction();
    switch(a){
      case UP:
	this->personnages[i].moveUp();
	break;
      case DOWN:
	this->personnages[i].moveDown();
	break;
      case LEFT:
	this->personnages[i].moveLeft();
	break;
      case RIGHT:
	this->personnages[i].moveRight();
	break;
      case ATTACK:
	break;
      default:
	break;
    }
  }

}
