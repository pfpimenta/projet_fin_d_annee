#pragma once

#ifndef AGENT_HPP
#define AGENT_HPP

#define HP_MAX 100.0f

#include <iostream>
#include <cassert>
#include <string>
#include "q_table.hpp"
//#include "GameManager.hpp"


class GameManager;

enum Action { UP, DOWN, LEFT, RIGHT, ATTACK};
 
class Agent
{
	float hp; //health point
	float attack_force; // force du attaque
	GameManager *gm;
	int width;
	int height;
	public:
	Agent(); // constructeur
	Agent(int x, int y); // constructeur
	int pos_x;
	int pos_y;
	void takeDamage(float damage); // take damage
	Action chooseAction();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void setBoundaries(int w, int h);
	void setGameManager(GameManager* gameManager);
	void setHP(float hp);
	float getHP();
	void setAttackForce(float attackForce);
	float getAttackForce();

};

#endif
