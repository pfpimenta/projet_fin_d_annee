#pragma once

#ifndef ENEMY_HPP
#define ENEMY_HPP

#define DEFAULT_ATTACK_FORCE 15.7

#include <iostream>
#include <cassert>
#include <string>
#include "q_table.hpp"
#include "Agent.hpp"


class GameManager;

class Enemy :  public Agent
{
	Q_table qTable;
	public:
	Enemy(); // constructeur
	Enemy(int x, int y); // constructeur
	void takeDamage(float damage); // take damage
	Action chooseAction(); 
};

#endif
