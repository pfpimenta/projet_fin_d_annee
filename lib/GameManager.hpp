#pragma once

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "Agent.hpp"

#include <iostream>
#include <cassert>
#include <vector>

class GameManager
{
	std::vector<Agent*> personnages;
	// objets ? pierre etc
	int width;
	int height;
	public:
	GameManager(); // constructeur
	GameManager(int width, int height); // constructeur
	int getWidth();
	int getHeight();
	char charAtPosition(int x, int y);
	//auto atPosition(int x, int y); //??
	void printGrid();
	void printHPs();
	void addPersonnage(Agent* personnage);
	void step();
	void doDamageAroundPoint(int x, int y, float attack_damage);

};

#endif
