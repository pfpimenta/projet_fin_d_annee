#pragma once

#ifndef AGENT_HPP
#define AGENT_HPP

#define HP_MAX 100.0f

#include <iostream>

class Agent
{
	float hp; //health point
	//vec3 position;
	public:
	Agent(); // constructeur
	Agent(int x, int y); // constructeur
	int pos_x;
	int pos_y;
	void takeDamage(float damage); // take damage

};

#endif
