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
  int max_episodes = DEFAULT_MAX_EPISODES;
  int max_steps_per_episode = DEFAULT_STEP_PER_EPISODE;

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
  QTableAction action;
  float reward = 0.0f;

  // table qu'on va entrainer
  int num_states = getNumStates();
  Q_table* qtable = new Q_table(num_states, NUM_ACTIONS);


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
      Learner* new_learner = new Learner(pos_x, pos_y, qtable);
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
        // get action, state, lastState, and reward   to update Q-table
        action = this->learners[i]->getLastAction();
        state = getState(dist_x_pers, dist_y_pers, hp_soi, hp_pers);
        //std::cout << "DEBUG state: "<< state << std::endl;
        if(step_count!=0){
          lastState = this->learners[i]->getLastState();
          reward = this->learners[i]->getReward();
          //std::cout << "DEBUG avant update_table, reward: "<< reward << std::endl;
          // actualise le tableau Q
          this->learners[i]->getQTable()->update_table(action, lastState, state, reward);
        }
        // set lastState
        this->learners[i]->setLastState(state);

        if(learners[i]->isDead()){
            // effacer du vecteur :
            this->learners.erase(this->learners.begin()+i);
            // ajuster index et num_learners :
            i--;
            num_learners--;
        }
      }
    }
  }
  std::cout << "...training complete" << std::endl;

  // print q-table
  //qtable->printTable();
  //qtable->printTableBestActions();
  qtable->saveTable("test_table");
  qtable->loadTable("test_table");
}

// avance un tour du jeu
void Trainer::step(){
  int num_learners = this->learners.size();
  QTableAction a;
  int pos_x, pos_y;
  float attack_damage;
  int i_closest_enemy, dist_x_pers, dist_y_pers;
  float hp_pers;

  // execution des actions des personnages
  for(int i = 0; i < num_learners; i++){

    // get informations pour choisir l'action
    i_closest_enemy = this->learners[i]->findClosestEnemy(this->learners);
    dist_x_pers = this->learners[i_closest_enemy]->getPosX() - this->learners[i]->getPosX();
    dist_y_pers = this->learners[i_closest_enemy]->getPosY() - this->learners[i]->getPosY();
    hp_pers = this->learners[i_closest_enemy]->getHP();
    // choisir l'action
    a = this->learners[i]->chooseAction(dist_x_pers, dist_y_pers, hp_pers);

    pos_x = this->learners[i]->getPosX();
    pos_y = this->learners[i]->getPosY();

    // faire l'action
    switch(a){
      case QUP:
        if(this->isSomeoneAtPosition(pos_x-1, pos_y) == false){
          this->learners[i]->moveUp();
        }
        break;
      case QDOWN:
        if(this->isSomeoneAtPosition(pos_x+1, pos_y) == false){
          this->learners[i]->moveDown();
        }
        break;
      case QLEFT:
        if(this->isSomeoneAtPosition(pos_x, pos_y-1)== false){
          this->learners[i]->moveLeft();
        }
        break;
      case QRIGHT:
        if(this->isSomeoneAtPosition(pos_x, pos_y+1)== false){
          this->learners[i]->moveRight();
        }
        break;
      case QATTACK:
        attack_damage = this->learners[i]->getAttackForce();
        this->doDamageAroundPoint(pos_x, pos_y, attack_damage);
        if(this->verifyDeadLearners()){
          // qqn est mort par cet attaque
          this->learners[i]->killedSomeone();
        }
        break;
      default:
        break;
    }
  }

}

// mettre les booleans a jour des mecs qui sont morts
bool Trainer::verifyDeadLearners(){
  bool aLearnerDied = false;
  for(int i = 0; i < (int) this->learners.size(); i++){
    // verifier si ils sont morts
    if ( this->learners[i]->getHP() <= 0.0 && this->learners[i]->isDead()==false){
      // il est mort
      //toggle boolean
      this->learners[i]->die();
      aLearnerDied = true;
    }
  }
  return aLearnerDied;
}

// getters et setters :
int Trainer::getWidth(){
  return this->width;
}

int Trainer::getHeight(){
  return this->height;
}
