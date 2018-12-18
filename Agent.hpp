#pragma once

#ifndef AGENT_HPP
#define AGENT_HPP

#define HP_MAX 100.0f

#include <iostream>
#include <cstdlib>
#include "q_table.hpp"

enum Action { UP, DOWN, LEFT, RIGHT, ATTACK};
 
class Agent
{
	float hp; //health point
	Q_table qTable;
	//vec3 position;
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
	void attack();
	void setBoundaries(int w, int h);

};

#endif