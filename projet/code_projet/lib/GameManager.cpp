#include <iostream>

#include "GameManager.hpp"


// constructeur
GameManager::GameManager(){
}

// constructeur avec w et h :
// nombre de cases en epaisseur et hauteur
GameManager::GameManager(int w, int h){
  assert(w > 0);
  assert(h > 0);
  this->width = w;
  this->height = h;
}

// constructeur
// w et h : nombre de cases en epaisseur et hauteur
// smgr : scene manager (declare dans le main)
// timerCursorBlink : timer pour que le curseur blink
GameManager::GameManager(int w, int h, is::ISceneManager *smgr, ITimer *timerCursorBlink){
  assert(w > 0);
  assert(h > 0);
  this->width = w;
  this->height = h;
  /** construction du grid **/
  // creation d'une grid avec les dimensions (w x h) et la position du curseur par defaut (position : (0, 0))
  grid = new gridMapping (position(DEFAULT_LIGNE, DEFAULT_COLONNE), w, h, smgr, timerCursorBlink);
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
    if(x == this->personnages[i]->getPosX() && y == this->personnages[i]->getPosY()){
      return 'x';
    }
  }
  return '-';
}

bool GameManager::isSomeoneAtPosition(int x, int y){
  // retourne un char qui represente ce qui est dans cette position dans la grid
  int nombre_personnages = this->personnages.size();
  for(int i = 0; i < nombre_personnages; i++){
    if(x == this->personnages[i]->getPosX() && y == this->personnages[i]->getPosY()){
      return true;
    }
  }
  return false;
}

// affichage du grid
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

// affichage des HPs de chaque personnage
void GameManager::printHPs(){
  float hp;
  std::cout << "...printing HPs:" << std::endl;
  int nombre_personnages = this->personnages.size();
  for(int i = 0; i < nombre_personnages; i++){
    hp = this->personnages[i]->getHP();
    std::cout << " -> " << hp << std::endl;
  }
}

// add a personnage to the game
// on essaie d'ajouter le personnage
// addEnemy() renvoie false si la position est occupee
void GameManager::addPersonnage(Agent* personnage){
  if (grid->addEnemy(position(personnage->getPosX(), personnage->getPosY()))){
      personnage->setBoundaries(this->width, this->height);
      personnage->setGameManager(this);
      this->personnages.push_back(personnage);
  }
}

// avance un tour du jeu
void GameManager::step(){
  // l'ennemi ne doit pas aller la ou se trouve le curseur sinon le joueur au prochain tour
  // se trouvera a la mm position que lennemi => gros BUG
  // solution : reset la position du curseur avant chaque mouvement de l'ennemi :
  grid->mouvement(RESET);
  int nombre_personnages = this->personnages.size();
  Action a;
  int attack_x, attack_y;
  float attack_damage;

  // execution des actions des personnages
  for(int i = 0; i < nombre_personnages; i++){
    // choisir l'action
    a = this->personnages[i]->chooseAction();

    // faire l'action
    position tmp;
    switch(a){
      case UP:
        this->personnages[i]->moveUp();
        tmp = position(personnages[i]->getPosX(), personnages[i]->getPosY());
        grid->setEnemyCursor(i, tmp);
        personnages[i]->setPosX(tmp.ligne);
        personnages[i]->setPosY(tmp.colonne);
        break;
      case DOWN:
        this->personnages[i]->moveDown();
        tmp = position(personnages[i]->getPosX(), personnages[i]->getPosY());
        grid->setEnemyCursor(i, tmp);
        personnages[i]->setPosX(tmp.ligne);
        personnages[i]->setPosY(tmp.colonne);
        break;
      case LEFT:
        this->personnages[i]->moveLeft();
        tmp = position(personnages[i]->getPosX(), personnages[i]->getPosY());
        grid->setEnemyCursor(i, tmp);
        personnages[i]->setPosX(tmp.ligne);
        personnages[i]->setPosY(tmp.colonne);
        break;
      case RIGHT:
        this->personnages[i]->moveRight();
        tmp = position(personnages[i]->getPosX(), personnages[i]->getPosY());
        grid->setEnemyCursor(i, tmp);
        personnages[i]->setPosX(tmp.ligne);
        personnages[i]->setPosY(tmp.colonne);
        break;
      case ATTACK:
        attack_damage = this->personnages[i]->getAttackForce();
        attack_x = this->personnages[i]->getPosX();
        attack_y = this->personnages[i]->getPosY();
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
      grid->enemyDied(i);
      this->personnages.erase(this->personnages.begin()+i);
    }
  }
}


// cause dommage autour du point (x,y)
void GameManager::doDamageAroundPoint(int x, int y, float attack_damage){
  int nombre_personnages = this->personnages.size();
  for(int i = 0; i < nombre_personnages; i++){
    // verifie pour les 8 carres autour du attaquant
    if(this->personnages[i]->getPosX() == x + 1 && this->personnages[i]->getPosY() == y + 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->getPosX() == x + 1 && this->personnages[i]->getPosY() == y){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->getPosX() == x + 1 && this->personnages[i]->getPosY() == y - 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->getPosX() == x  && this->personnages[i]->getPosY() == y + 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->getPosX() == x  && this->personnages[i]->getPosY() == y - 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->getPosX() == x - 1 && this->personnages[i]->getPosY() == y + 1){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->getPosX() == x - 1 && this->personnages[i]->getPosY() == y){
      this->personnages[i]->takeDamage(attack_damage);
    }else if(this->personnages[i]->getPosX() == x - 1 && this->personnages[i]->getPosY() == y - 1){
      this->personnages[i]->takeDamage(attack_damage);
    }
  }
}
