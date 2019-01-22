#pragma once

#ifndef LEARNER_HPP
#define LEARNER_HPP


#define DEFAULT_ATTACK_FORCE 15.7

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "Q_table.hpp"
#include "Agent.hpp"
#include "State.hpp"




class GameManager;

class Learner :  public Agent
{
			Q_table* qTable;
			int lastState;
			QTableAction lastAction;
			bool estMort; // true s'il est mort
			bool aTue; // true s'il a tue qqn dans ce step
	public:
			Learner(); // constructeur
			Learner(int x, int y); // constructeur avec position
			Learner(int x, int y, Q_table* q_table); // constructeur avec position et Qtable
			QTableAction chooseAction(int dist_x_pers, int dist_y_pers, float hp_pers);
			QTableAction chooseRandomAction();
			// return index of the closest mec to this mec
			int findClosestEnemy(std::vector<Learner*> learners);
			void die(); // pour bool isDead = true
			bool isDead(); // pour bool isDead = true
			void killedSomeone(); // pour bool aTue
			bool hasKilledSomeone(); // pour bool aTue
			float getReward();
			// getters et setters :
			Q_table* getQTable();
			void setQTable(Q_table* q_table);
			int getLastState();
			void setLastState(int state);
			QTableAction getLastAction();
			void setLastAction(QTableAction action);
};

#endif
