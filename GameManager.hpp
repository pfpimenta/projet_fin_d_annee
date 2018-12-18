#pragma once

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "Agent.hpp"

#include <iostream>
#include <vector>

class GameManager
{
	std::vector<Agent> personnages;
	// objets ? pierre etc
	int width;
	int height;
	public:
	GameManager(); // constructeur
	GameManager(int width, int height); // constructeur
	int getWidth();
	int getHeight();
	char atPosition(int x, int y);
	void printGrid();
	void addPersonnage(Agent personnage);
};

#endif
