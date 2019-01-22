#include <iostream>

#include "enemy.hpp"


// constructeur
//enemy::enemy(){}


// constructeur sans model 3D
// constructeur
enemy::enemy(position p, int HP, int id)
:person(p, HP, id)
{
     this->attack_force = 15.5;
}


// constructeur avec model 3D
enemy::enemy(position p, int HP, irr::scene::ISceneManager *smgr, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D, int id)
    :person(p, HP, smgr, mesh, texture, pos3D, id)
{}

std::string enemy::personAction(Action act)
{
    switch (act)
    {
        case UP:
            std::cout << "ENEMY UP" << std::endl;
            break;
        case DOWN:
            std::cout << "ENEMY DOWN" << std::endl;
            break;
        case LEFT:
            std::cout << "ENEMY LEFT" << std::endl;
            break;
        case RIGHT:
            std::cout << "ENEMY RIGHT" << std::endl;
            break;
        case ATTACK:
            std::cout << "ENEMY ATTACK" << std::endl;
            break;
        default:
            std::cout << "ENEMY Nothing to do" << std::endl;
            break;
    }
    return ("Done");
}

void enemy::move(QTableAction action)
{
    switch (action)
    {
        case QUP:
						this->p.ligne +=1;
            break;
        case QDOWN:
						this->p.ligne -=1;
            break;
        case QLEFT:
						this->p.colonne -=1;
            break;
        case QRIGHT:
						this->p.colonne -=1;
            break;
        case QATTACK:
            std::cout << "ERROR enemy::move( ATTACK) ne marche pas" << std::endl;
						exit(1);
            break;
        default:
						std::cout << "ERROR enemy::move("<<(int)action<<") ne marche pas" << std::endl;
						exit(1);
    }
}


QTableAction enemy::chooseAction(int dist_x_pers, int dist_y_pers, float hp_pers){
	QTableAction action;
  int state = getState(dist_x_pers, dist_y_pers, this->HP, hp_pers);
  action = (QTableAction) this->qTable->takeAction(state);
	//std::cout << "DEBUG action "<< (int)action <<'\n';
  this->lastAction = action;
  return action;
}




float enemy::getAttackForce(){
  return this->attack_force ;
}
void enemy::setQTable(Q_table* q_table){
	this->qTable = q_table;
}
