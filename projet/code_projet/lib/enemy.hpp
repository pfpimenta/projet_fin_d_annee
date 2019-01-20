#pragma once

#ifndef ENEMY_HPP
#define ENEMY_HPP


#include "utils.hpp"
#include "Q_table.hpp"


class enemy : public person
{
    Q_table* qTable;
    int lastState;
    Action lastAction;
    bool estMort; // true s'il est mort
    bool aTue; // true s'il a tue qqn dans ce step
	public:
		//enemy();
		enemy(position p, int HP, int id);
		enemy(position p, int HP, irr::scene::ISceneManager *smgr, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D, int id);
		//virtual
		std::string personAction(Action act);
		void move(QTableAction action);

};
#endif
