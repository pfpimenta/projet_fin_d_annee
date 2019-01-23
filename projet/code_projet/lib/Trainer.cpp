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
      //std::cout << "DEBUG isSomeoneAtPosition : returning true" << '\n';
      return true;
    }
  }
  return false;
}

char Trainer::charAtPosition(int x, int y){
  // retourne un char qui represente ce qui est dans cette position dans la grid
  this->num_learners = this->learners.size();
  for(int i = 0; i < this->num_learners; i++){
    if(x == this->learners[i]->getPosX() && y == this->learners[i]->getPosY()){
      if(this->learners[i]->isTrained) return 'Q'; // avec Q-table
      else return 'x'; // sans Q-table
    }
  }
  return '-';
}

// affichage du grid
void Trainer::printGrid(){
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
void Trainer::printHPs(){
  float hp;
  std::cout << "...printing HPs:" << std::endl;
  this->num_learners = this->learners.size();
  for(int i = 0; i < this->num_learners; i++){
    hp = this->learners[i]->getHP();
    if(this->learners[i]->isTrained)
        std::cout << " -> " << hp <<" (trained)"<<std::endl;
    else
        std::cout << " -> " << hp <<" (random)"<<std::endl;
  }
}

// cause dommage autour du point (x,y)
void Trainer::doDamageAroundPoint(int x, int y, float attack_damage){
  int nombre_personnages = this->learners.size();
  for(int i = 0; i < nombre_personnages; i++){
    // verifie pour les 8 carres autour du attaquant
    if(this->learners[i]->getPosX() == x + 1 && this->learners[i]->getPosY() == y){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x  && this->learners[i]->getPosY() == y + 1){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x  && this->learners[i]->getPosY() == y - 1){
      this->learners[i]->takeDamage(attack_damage);
    }else if(this->learners[i]->getPosX() == x - 1 && this->learners[i]->getPosY() == y){
      this->learners[i]->takeDamage(attack_damage);
    }
  }
}

// fonction pour faire le update de la QTable
void Trainer::updateTable( int step_count){
  int i_closest_enemy; // index
  // informations pour le state
  int dist_x_pers;
  int dist_y_pers;
  float hp_soi = 100.0f;
  float hp_pers = 100.0f;

  int currentState, lastState;
  QTableAction action;
  float reward = 0.0f;

  for(int learnerIndex = 0; learnerIndex < this->num_learners; learnerIndex++){
      // calculate state information :
      // find closest mec :
      //i_closest_enemy = this->findClosestEnemy(learners[i]->pos_x,learners[i]->pos_y);
      i_closest_enemy = this->learners[learnerIndex]->findClosestEnemy(this->learners);
      dist_x_pers = this->learners[i_closest_enemy]->getPosX() - this->learners[learnerIndex]->getPosX();
      dist_y_pers = this->learners[i_closest_enemy]->getPosY() - this->learners[learnerIndex]->getPosY();
      hp_soi = this->learners[learnerIndex]->getHP();
      hp_pers = this->learners[i_closest_enemy]->getHP();
      // get action, state, lastState, and reward   to update Q-table
      action = this->learners[learnerIndex]->getLastAction();
      currentState = getState(dist_x_pers, dist_y_pers, hp_soi, hp_pers);
      //std::cout << "DEBUG state: "<< state << std::endl;
      if(step_count!=0){
        lastState = this->learners[learnerIndex]->getLastState();
        reward = this->learners[learnerIndex]->getReward();
        //std::cout << "DEBUG avant update_table, reward: "<< reward << std::endl;
        // actualise le tableau Q
        if(this->learners[learnerIndex]->isTrained)
          this->learners[learnerIndex]->getQTable()->update_table(action, lastState, currentState, reward);
      }
      // set lastState
      this->learners[learnerIndex]->setLastState(currentState);
  }
}

// efface tous les learners et genere entre 2 et 5 learners au hasard
void Trainer::resetLearners(Q_table* qtable){
  //int num_learners;
  int pos_x; // pos initialle d'un learner
  int pos_y;

  // reset personnages / learners
  this->learners.clear();
  // generer entre 2 et 5 learners au hasard
  // (a chaque episode il y a un nombre
  // different de learners pendant le training)
  this->num_learners = 2 + std::rand()%4;
  for(int i = 0; i < this->num_learners; i++){
    // positions aleatoires uniques pour chaque mec
    do{
      pos_x = std::rand()%this->width;
      pos_y = std::rand()%this->height;
    }while(this->isSomeoneAtPosition(pos_x, pos_y));
    Learner* new_learner = new Learner(pos_x, pos_y, qtable);
    new_learner->setBoundaries(this->width, this->height);
    new_learner->isTrained = true;
    this->learners.push_back(new_learner);
  }
}


// fonction pour entrainer les Q-tables // VERSION 2
void Trainer::train_v2(){
  // executes the training of the Q tables
  // without showing on screen

  std::srand(std::time(nullptr)); // use current time as seed for random generator

  // un episode est un jeu avec (max_steps_per_episode) steps :
  int max_episodes = DEFAULT_MAX_EPISODES;
  int max_steps_per_episode = DEFAULT_STEP_PER_EPISODE;
  // table qu'on va entrainer :
  int num_states = getNumStates();
  Q_table* qtable = new Q_table(num_states, NUM_ACTIONS);

  int pos_x, pos_y;

  int episode_count, step_count;
  for( episode_count = 0; episode_count < max_episodes; episode_count ++){

    // reset personnages / learners
    // generer 2 learners:
    // 1 avec Q table
    // et 1 autre avec des actions au hasard
    this->num_learners = 2;
    // clear personnages / learners
    this->learners.clear();

    //  mec 1 :
    // positions aleatoires uniques pour chaque mec
    pos_x = std::rand()%this->width;
    pos_y = std::rand()%this->height;
    Learner* mec_entrainne = new Learner(pos_x, pos_y, qtable);
    mec_entrainne->isTrained = true;
    mec_entrainne->setBoundaries(this->width, this->height);
    this->learners.push_back(mec_entrainne);
    //  mec 2 :
    // positions aleatoires uniques pour chaque mec
    do{
      pos_x = std::rand()%this->width;
      pos_y = std::rand()%this->height;
    }while(this->isSomeoneAtPosition(pos_x, pos_y));
    Learner* mec_random = new Learner(pos_x, pos_y);
    mec_random->setBoundaries(this->width, this->height);
    this->learners.push_back(mec_random);

    // executer la simulation pour l'aprentissage:
    for( step_count = 0; step_count < max_steps_per_episode && num_learners !=1; step_count ++){
      // do one step
      this->step();
      this->num_learners = learners.size(); //update num_learners
      this->updateTable(step_count);
      this->eraseDeadLearners();
    }
  }
  std::cout << "...training complete" << std::endl;

  // print q-table
  qtable->printTable();
  qtable->printTableBestActions();
  //qtable->saveTable("q_table");
  //qtable->loadTable("q_table");
  this->test(qtable); // DEBUG
}


// fonction pour entrainer les Q-tables
void Trainer::train(){
  // executes the training of the Q tables
  // without showing on screen

  std::srand(std::time(nullptr)); // use current time as seed for random generator

  // un episode est un jeu avec (max_steps_per_episode) steps :
  int max_episodes = DEFAULT_MAX_EPISODES;
  int max_steps_per_episode = DEFAULT_STEP_PER_EPISODE;
  // table qu'on va entrainer :
  int num_states = getNumStates();
  Q_table* qtable = new Q_table(num_states, NUM_ACTIONS);

  int episode_count, step_count;
  for( episode_count = 0; episode_count < max_episodes; episode_count ++){

    // reset personnages / learners
    this->resetLearners(qtable);
    this->num_learners = learners.size(); //update num_learners

    // executer la simulation pour l'aprentissage:
    for( step_count = 0; step_count < max_steps_per_episode && num_learners !=1; step_count ++){
      // do one step
      this->step();
      this->num_learners = learners.size(); //update num_learners
      this->updateTable(step_count);
      this->eraseDeadLearners();
    }
  }
  std::cout << "...training complete" << std::endl;

  // print q-table :
  //qtable->printTable();
  //qtable->printTableBestActions();

  // save q-table :
  qtable->saveTable("q_table");
  //qtable->loadTable("q_table"); // DEBUG

  // test q-table :
  //this->test(qtable); // DEBUG
}

// avance un tour du jeu
void Trainer::step(){
  this->num_learners = this->learners.size();
  QTableAction a;
  // execution des actions des personnages
  for(int i = 0; i < this->num_learners; i++){
    // choisir l'action
    a = this->deciderAction(i);
    this->executerAction(i, a);
  }
}

void Trainer::executerAction(int learnerIndex, QTableAction a){
  float attack_damage;
  int pos_x = this->learners[learnerIndex]->getPosX();
  int pos_y = this->learners[learnerIndex]->getPosY();
  // faire l'action
  switch(a){
    case QUP:
      if(this->isSomeoneAtPosition(pos_x-1, pos_y) == false){
        this->learners[learnerIndex]->moveUp();
      }
      break;
    case QDOWN:
      if(this->isSomeoneAtPosition(pos_x+1, pos_y) == false){
        this->learners[learnerIndex]->moveDown();
      }
      break;
    case QLEFT:
      if(this->isSomeoneAtPosition(pos_x, pos_y-1)== false){
        this->learners[learnerIndex]->moveLeft();
      }
      break;
    case QRIGHT:
      if(this->isSomeoneAtPosition(pos_x, pos_y+1)== false){
        this->learners[learnerIndex]->moveRight();
      }
      break;
    case QATTACK:
      attack_damage = this->learners[learnerIndex]->getAttackForce();
      this->doDamageAroundPoint(pos_x, pos_y, attack_damage);
      if(this->verifyDeadLearners()){
        // qqn est mort par cet attaque
        this->learners[learnerIndex]->killedSomeone();
      }
      break;
    default:
      break;
  }
}

// choix et execution de l'action du learner[learnerIndex]
QTableAction Trainer::deciderAction(int learnerIndex){
  int i_closest_enemy = this->learners[learnerIndex]->findClosestEnemy(this->learners);
  int dist_x_pers = this->learners[i_closest_enemy]->getPosX() - this->learners[learnerIndex]->getPosX();
  int dist_y_pers = this->learners[i_closest_enemy]->getPosY() - this->learners[learnerIndex]->getPosY();
  float hp_pers = this->learners[i_closest_enemy]->getHP();
  // choisir l'action
  QTableAction a = this->learners[learnerIndex]->chooseAction(dist_x_pers, dist_y_pers, hp_pers);
  return a;
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

// erase dead learners from the learners vector
void Trainer::eraseDeadLearners(){
  for(int i = 0; i < this->num_learners; i++){
    if(learners[i]->isDead()){
        // effacer du vecteur :
        this->learners.erase(this->learners.begin()+i);
        // ajuster index et num_learners :
        i--;
        this->num_learners--;
    }
  }
}

// fonction pour entrainer les Q-tables
void Trainer::test(Q_table* q_table){
  //
  //int num_learners;
  int pos_x; // pos initialle d'un learner
  int pos_y;
  // pour la q table qu'on va tester
  QTableAction action;

  // generer 2 learners:
  // 1 avec Q table
  // et 1 autre avec des actions au hasard
  this->num_learners = 2;
  // clear personnages / learners
  this->learners.clear();

  //  mec 1 :
  // positions aleatoires uniques pour chaque mec
  pos_x = std::rand()%this->width;
  pos_y = std::rand()%this->height;
  Learner* mec_entrainne = new Learner(pos_x, pos_y, q_table);
  mec_entrainne->isTrained = true;
  mec_entrainne->setBoundaries(this->width, this->height);
  this->learners.push_back(mec_entrainne);
  //  mec 2 :
  // positions aleatoires uniques pour chaque mec
  do{
    pos_x = std::rand()%this->width;
    pos_y = std::rand()%this->height;
  }while(this->isSomeoneAtPosition(pos_x, pos_y));
  Learner* mec_random = new Learner(pos_x, pos_y);
  mec_random->setBoundaries(this->width, this->height);
  this->learners.push_back(mec_random);

  // loop du combat:
  while(this->num_learners !=1){
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // clear
    this->printGrid();
    this->printHPs();
    // action du mec 1 ( q table )
    action = this->deciderAction(0);
    this->executerAction(0, action);
    //std::cout << "DEBUG action: " <<action<< '\n';
    // action du mec 2 ( au hasard )
    action = this->learners[1]->chooseRandomAction();
    this->executerAction(1, action);
    this->num_learners = learners.size();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' ); // wait for key
  }

}


// getters et setters :
int Trainer::getWidth(){
  return this->width;
}

int Trainer::getHeight(){
  return this->height;
}
