#include <iostream>

#include "GameManager.hpp"


// constructeur
GameManager::GameManager(){
}



// constructeur
// w et h : nombre de cases en epaisseur et hauteur
// smgr : scene manager (declare dans le main)
// timerCursorBlink : timer pour que le curseur blink
GameManager::GameManager(int w, int h, is::ISceneManager *smgr, ITimer *timerCursorBlink, MyEventReceiver *receiver){
  assert(w > 0);
  assert(h > 0);
  this->width = w;
  this->height = h;


  /** construction du grid **/
  // creation d'une grid avec les dimensions (w x h) et la position du curseur par defaut (position : (0, 0))
  grid = new gridMapping (position(DEFAULT_LIGNE, DEFAULT_COLONNE), w, h, smgr, timerCursorBlink);
  receiver->grid = this->grid; // attachement du receiver a la grid


}


int GameManager::getWidth(){
  return this->width;
}

int GameManager::getHeight(){
  return this->height;
}

int GameManager::findClosestEnemy(int pos_x, int pos_y){
  int closestEnemyIndex = 0;
  // TODO
  return closestEnemyIndex;
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
  if (grid->addEnemy(position(personnage->pos_x, personnage->pos_y)))
  {
      personnage->setBoundaries(this->width, this->height);
      personnage->setGameManager(this);
      this->personnages.push_back(personnage);
  }
}

// avance un tour du jeu
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
    position tmp;
    switch(a){
      case UP:
        this->personnages[i]->moveUp();
        tmp = position(personnages[i]->pos_x, personnages[i]->pos_y);
        grid->setEnemyCursor(i, tmp);
        personnages[i]->pos_x = tmp.ligne;
        personnages[i]->pos_y = tmp.colonne;
        break;
      case DOWN:
        this->personnages[i]->moveDown();
        tmp = position(personnages[i]->pos_x, personnages[i]->pos_y);
        grid->setEnemyCursor(i, tmp);
        personnages[i]->pos_x = tmp.ligne;
        personnages[i]->pos_y = tmp.colonne;
        break;
      case LEFT:
        this->personnages[i]->moveLeft();
        tmp = position(personnages[i]->pos_x, personnages[i]->pos_y);
        grid->setEnemyCursor(i, tmp);
        personnages[i]->pos_x = tmp.ligne;
        personnages[i]->pos_y = tmp.colonne;
        break;
      case RIGHT:
        this->personnages[i]->moveRight();
        tmp = position(personnages[i]->pos_x, personnages[i]->pos_y);
        grid->setEnemyCursor(i, tmp);
        personnages[i]->pos_x = tmp.ligne;
        personnages[i]->pos_y = tmp.colonne;
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


// cause dommage autour du point (x,y)
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

// fonction pour entrainer les Q-tables
void GameManager::train(){
  // executes the training of the Q tables
  // without showing on screen

  // un episode est un jeu avec (max_steps_per_episode) steps
  int max_episodes = 10;
  int max_steps_per_episode = 10;

  int num_learners;
  std::vector<Enemy*> learners;
  int pos_x; // pos initialle d'un learner
  int pos_y;
  int i_closest_enemy; // index

  // informations pour le state
  int dist_x_pers;
  int dist_y_pers;
  float hp_soi = 100.0f;
  float hp_pers = 100.0f;

  int state, lastState;
  Action action;
  float reward = 0.0f;

  // table qu'on va entrainer
  int num_states = getNumStates();
  Q_table qtable = Q_table(num_states, NUM_ACTIONS);
  Q_table* qtable_pointer = &qtable;


  int episode_count, step_count;
  for( episode_count = 0; episode_count < max_episodes; episode_count ++){
    // reset personnages / learners
    learners.clear();
    // generer entre 2 et 5 learners au hasard
    // (a chaque episode il y a un nombre
    // different de learners pendant le training)
    num_learners = 2 + std::rand()%4;
    for(int i = 0; i < num_learners; i++){
      pos_x = std::rand()%this->width;
      pos_y = std::rand()%this->height;
      Enemy new_learner = Enemy(pos_x, pos_y, qtable_pointer);
      learners.push_back(&new_learner);
    }

    // reset autre chose de la grid? TODO ?

    // executer la simulation pour l'aprentissage:
    for( step_count = 0; step_count < max_steps_per_episode; step_count ++){
      //this->step(); // do one step
      this->train_step(learners); // do one step

      // update q tables for all personnages / learners
      for(int i = 0; i < num_learners; i++){
      	// calculate state information :
        // find closest mec :
        i_closest_enemy = this->findClosestEnemy(learners[i]->pos_x,learners[i]->pos_y);
        dist_x_pers = learners[i_closest_enemy]->pos_x - learners[i]->pos_x;
        dist_y_pers = learners[i_closest_enemy]->pos_y - learners[i]->pos_y;
      	hp_soi = learners[i]->getHP();
      	hp_pers = learners[i_closest_enemy]->getHP();
        // get Q-table
      	qtable_pointer = learners[i]->getQTable();
        // get action, state, lastState, and reward   to update Q-table
        action = learners[i]->getLastAction();
        state = getState(dist_x_pers, dist_y_pers, hp_soi, hp_pers);
        if(step_count!=0){
          lastState = learners[i]->getLastState();
          reward = std::rand() / static_cast <float> (RAND_MAX); // DEBUG
          std::cout << "DEBUG avant update_table, reward: "<< reward << std::endl;
          //std::cout << "DEBUG avant update_table, reward: "<< reward << std::endl;
          qtable_pointer->update_table(action, lastState, state, reward); // actualise le tableau Q
          std::cout << "DEBUG apres update_table" << std::endl;
        }
        // set lastState
        learners[i]->setLastState(state);
      }
    }
  }
  std::cout << "...training complete" << std::endl;

  // print q-table
  qtable.printTable();
}

// avance un tour du jeu
void GameManager::train_step(std::vector<Enemy*> learners){
  int nombre_personnages = learners.size();
  Action a;
  int attack_x;
  int attack_y;
  float attack_damage;

  // execution des actions des personnages
  for(int i = 0; i < nombre_personnages; i++){
    // choisir l'action
    a = learners[i]->chooseAction();

    // faire l'action
    switch(a){
      case UP:
	learners[i]->moveUp();
	break;
      case DOWN:
	learners[i]->moveDown();
	break;
      case LEFT:
	learners[i]->moveLeft();
	break;
      case RIGHT:
	learners[i]->moveRight();
	break;
      case ATTACK:
	attack_damage = learners[i]->getAttackForce();
	attack_x = learners[i]->pos_x;
	attack_y = learners[i]->pos_y;
	this->doDamageAroundPoint(attack_x, attack_y, attack_damage);
	break;
      default:
	break;
    }
  }

  // verifier si ils sont morts
  for(int i = 0; i < nombre_personnages; i++){
    if ( learners[i]->getHP() <= 0.0){
      // il est mort
      learners.erase(learners.begin()+i);
    }
  }
}
