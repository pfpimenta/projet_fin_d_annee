#pragma once

#ifndef ENEMY_HPP
#define ENEMY_HPP

#define DEFAULT_ATTACK_FORCE 15.7

#include <iostream>
#include <cassert>
#include <string>
#include "Q_table.hpp"
#include "Agent.hpp"
#include "State.hpp"


class GameManager;

class Enemy :  public Agent
{
	Q_table* qTable;
	public:
	Enemy(); // constructeur
	Enemy(int x, int y); // constructeur avec position
	Enemy(int x, int y, Q_table* q_table); // constructeur avec position et Qtable
	void takeDamage(float damage); // take damage
	Action chooseAction(int dist_x_pers, int dist_y_pers, float hp_pers);
	void updateQTable(int dist_x_pers, int dist_y_pers, float hp_soi, float hp_pers);
	Q_table* getQTable();
	void setQTable(Q_table* q_table);
	int lastAction;
};

#endif
