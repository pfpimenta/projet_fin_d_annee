#pragma once

#ifndef ENEMY_HPP
#define ENEMY_HPP


#define DEFAULT_ATTACK_FORCE 15.7

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "Q_table.hpp"
#include "Agent.hpp"
#include "State.hpp"




class GameManager;

class Enemy :  public Agent
{
	Q_table* qTable;
	int lastState;
	Action lastAction;
	public:
	Enemy(); // constructeur
	Enemy(int x, int y); // constructeur avec position
	Enemy(int x, int y, Q_table* q_table); // constructeur avec position et Qtable
	void takeDamage(float damage); // take damage
	Action chooseAction(int dist_x_pers, int dist_y_pers, float hp_pers);
	Action chooseAction();
	// return index of the closest enemy to this mec
	int findClosestEnemy(std::vector<Enemy*> learners);
	// getters et setters :
	Q_table* getQTable();
	void setQTable(Q_table* q_table);
	int getLastState();
	void setLastState(int state);
	Action getLastAction();
	void setLastAction(Action action);
};

#endif
