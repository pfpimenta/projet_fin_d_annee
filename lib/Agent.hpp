#pragma once

#ifndef AGENT_HPP
#define AGENT_HPP

#define HP_MAX 100.0f

#include <iostream>
#include <cassert>
#include <string>
#include "q_table.hpp"

class GameManager;

enum Action { UP, DOWN, LEFT, RIGHT, ATTACK};
 
class Agent
{
	float hp; //health point
	float attack_force; // force du attaque
	Q_table qTable;
	//vec3 position;
	// width et height de la grid ou on est
	GameManager *gm;
	int width;
	int height;
	public:
	Agent(); // constructeur
	Agent(int x, int y); // constructeur
	int pos_x;
	int pos_y;
	void takeDamage(float damage); // take damage
	Action chooseAction(); // take damage
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	float attack();
	void setBoundaries(int w, int h);
	void setGameManager(GameManager* gameManager);
	float getHP();

};

#endif
