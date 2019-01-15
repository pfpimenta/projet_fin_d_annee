#include <iostream>

#include "Trainer.hpp"


// constructeur
Trainer::Trainer(){
}

// constructeur avec w et h :
// nombre de cases en epaisseur et hauteur
Trainer::Trainer(int w, int h){
  assert(w > 0);
  assert(h > 0);
  this->width = w;
  this->height = h;
}

bool Trainer::isSomeoneAtPosition(int x, int y){
  // retourne un char qui represente ce qui est dans cette position dans la grid
  for(int i = 0; i < (int) learners.size(); i++){
    if(x == learners[i]->getPosX() && y == learners[i]->getPosY()){
      return true;
    }
  }
  return false;
}

// affichage des HPs de chaque personnage
void Trainer::printHPs(){
  float hp;
  std::cout << "...printing HPs:" << std::endl;
  int num_learners = this->learners.size();
  for(int i = 0; i < num_learners; i++){
    hp = this->learners[i]->getHP();
    std::cout << " -> " << hp << std::endl;
  }
}

// cause dommage autour du point (x,y)
void Trainer::doDamageAroundPoint(int x, int y, float attack_damage){
  int nombre_personnages = this->learners.size();
  for(int i = 0; i < nombre_personnages; i++){
    // verifie pour les 8 carres autour du attaquant
    if(this->learners[i]->getPosX() == x + 1 && this->learners[i]->getPosY() == y + 1){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x + 1 && this->learners[i]->getPosY() == y){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x + 1 && this->learners[i]->getPosY() == y - 1){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x  && this->learners[i]->getPosY() == y + 1){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x  && this->learners[i]->getPosY() == y - 1){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x - 1 && this->learners[i]->getPosY() == y + 1){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x - 1 && this->learners[i]->getPosY() == y){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x - 1 && this->learners[i]->getPosY() == y - 1){
      this->learners[i]->takeDamage(attack_damage);
    }
  }
}

// fonction pour entrainer les Q-tables
void Trainer::train(){
  // executes the training of the Q tables
  // without showing on screen

  std::srand(std::time(nullptr)); // use current time as seed for random generator

  // un episode est un jeu avec (max_steps_per_episode) steps
  int max_episodes = 100;
  int max_steps_per_episode = 1000;

  int num_learners;
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
    this->learners.clear();
    // generer entre 2 et 5 learners au hasard
    // (a chaque episode il y a un nombre
    // different de learners pendant le training)
    num_learners = 2 + std::rand()%4;
    for(int i = 0; i < num_learners; i++){
      // positions aleatoires uniques pour chaque mec
      do{
        pos_x = std::rand()%this->width;
        pos_y = std::rand()%this->height;
      }while(this->isSomeoneAtPosition(pos_x, pos_y));
      Enemy* new_learner = new Enemy(pos_x, pos_y, qtable_pointer);
      new_learner->setBoundaries(this->width, this->height);
      this->learners.push_back(new_learner);
    }

    // reset autre chose de la grid? TODO ?

    // executer la simulation pour l'aprentissage:
    for( step_count = 0; step_count < max_steps_per_episode && num_learners !=1; step_count ++){
      //this->step(); // do one step
      this->step(); // do one step

      num_learners = learners.size();
      // update q tables for all personnages / learners
      for(int i = 0; i < num_learners; i++){
      	// calculate state information :
        // find closest mec :
        //i_closest_enemy = this->findClosestEnemy(learners[i]->pos_x,learners[i]->pos_y);
        i_closest_enemy = this->learners[i]->findClosestEnemy(this->learners);
        dist_x_pers = this->learners[i_closest_enemy]->getPosX() - this->learners[i]->getPosX();
        dist_y_pers = this->learners[i_closest_enemy]->getPosY() - this->learners[i]->getPosY();
      	hp_soi = this->learners[i]->getHP();
      	hp_pers = this->learners[i_closest_enemy]->getHP();
        // get Q-table
      	qtable_pointer = this->learners[i]->getQTable();
        // get action, state, lastState, and reward   to update Q-table
        action = this->learners[i]->getLastAction();
        state = getState(dist_x_pers, dist_y_pers, hp_soi, hp_pers);
        //std::cout << "DEBUG state: "<< state << std::endl;
        if(step_count!=0){
          lastState = this->learners[i]->getLastState();
          reward = std::rand() / static_cast <float> (RAND_MAX); // DEBUG
          //std::cout << "DEBUG avant update_table, reward: "<< reward << std::endl;
          //std::cout << "DEBUG avant update_table, reward: "<< reward << std::endl;
          qtable_pointer->update_table(action, lastState, state, reward); // actualise le tableau Q
          //std::cout << "DEBUG apres update_table" << std::endl;
        }
        // set lastState
        this->learners[i]->setLastState(state);
      }
    }
  }
  std::cout << "...training complete" << std::endl;

  // print q-table
  qtable.printTable();
}

// avance un tour du jeu
void Trainer::step(){
  int num_learners = this->learners.size();
  Action a;
  int pos_x, pos_y;
  float attack_damage;

  // execution des actions des personnages
  for(int i = 0; i < num_learners; i++){

    // choisir l'action
    a = this->learners[i]->chooseAction();
    pos_x = this->learners[i]->getPosX();
    pos_y = this->learners[i]->getPosY();


    // faire l'action
    switch(a){
      case UP:
        if(this->isSomeoneAtPosition(pos_x-1, pos_y) == false){
          this->learners[i]->moveUp();
        }
        break;
      case DOWN:
        if(this->isSomeoneAtPosition(pos_x+1, pos_y) == false){
          this->learners[i]->moveDown();
        }
        break;
      case LEFT:
        if(this->isSomeoneAtPosition(pos_x, pos_y-1)== false){
          this->learners[i]->moveLeft();
        }
        break;
      case RIGHT:
        if(this->isSomeoneAtPosition(pos_x, pos_y+1)== false){
          this->learners[i]->moveRight();
        }
        break;
      case ATTACK:
        attack_damage = this->learners[i]->getAttackForce();
        this->doDamageAroundPoint(pos_x, pos_y, attack_damage);
        break;
      default:
        break;
    }
  }

  // verifier si ils sont morts
  for(int i = 0; i < num_learners; i++){
    if ( this->learners[i]->getHP() <= 0.0){
      // il est mort
      this->learners.erase(this->learners.begin()+i);
      i--;
      num_learners--;
    }
  }
}

// getters et setters :
int Trainer::getWidth(){
  return this->width;
}

int Trainer::getHeight(){
  return this->height;
}
