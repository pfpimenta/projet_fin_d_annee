#include <iostream>

#include "enemy.hpp"


// constructeur
//enemy::enemy(){}


// constructeur sans model 3D
// constructeur
enemy::enemy(position p, int HP, int id)
:person(p, HP, id)
{}


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
