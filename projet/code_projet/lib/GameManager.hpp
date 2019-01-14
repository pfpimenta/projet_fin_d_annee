#pragma once

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP



#include "Agent.hpp"
#include "Enemy.hpp"
#include "Q_table.hpp"

#include <iostream>
#include <cassert>
#include <vector>



#include "./libGridMapping/globalHeader.hpp"



class GameManager
{
public:
    std::vector<Agent*> personnages; // ennemis
	// objets ? pierre etc
	int width;
	int height;

    //TODO player

	public:

    //la grille dans laquelle se passe l'action
    gridMapping *grid;
    void makeCurseurBlink(bool val)
    {
        grid->makeCurseurBlink(val);
    }


	GameManager(); // constructeur
    GameManager(int width, int height, scene::ISceneManager *smgr, ITimer *timerCursorBlink); // constructeur
	int getWidth();
	int getHeight();
	// the returns a character according to what is in the point (pos_x, pos_y) :
	char charAtPosition(int x, int y);
	// returns the index of the closest enemy to the point (pos_x, pos_y) :
	int findClosestEnemy(int pos_x, int pos_y);
	//auto atPosition(int x, int y); //??
	// print the grid at the current moment
	void printGrid();
	// print the current HPs of all personnages
	void printHPs();
	// add a personnage to the game :
	void addPersonnage(Agent* personnage);
	void step();
	void doDamageAroundPoint(int x, int y, float attack_damage);
	void train();
	void train_step(std::vector<Enemy*> learners);

};

#endif
