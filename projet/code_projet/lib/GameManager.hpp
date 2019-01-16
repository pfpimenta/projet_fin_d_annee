#pragma once

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP



#include "Agent.hpp"
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
    	GameManager(); // constructeur
      GameManager(int w, int h);
      GameManager(int width, int height, scene::ISceneManager *smgr, ITimer *timerCursorBlink); // constructeur
      //la grille dans laquelle se passe l'action
      gridMapping *grid;
      void makeCurseurBlink(bool val)
      {
          grid->makeCurseurBlink(val);
      }
      // returns a character according to what is in the point (pos_x, pos_y) :
    	char charAtPosition(int x, int y);
      // returns true if there's a learner at (x,y), and false otherwise
      bool isSomeoneAtPosition(int x, int y);
    	//auto atPosition(int x, int y); //??
    	// print the grid at the current moment
    	void printGrid();
    	// print the current HPs of all personnages
    	void printHPs();
    	// add a personnage to the game :
    	void addPersonnage(Agent* personnage);
    	void step();
    	void doDamageAroundPoint(int x, int y, float attack_damage);
      int getWidth();
      int getHeight();
};

#endif
