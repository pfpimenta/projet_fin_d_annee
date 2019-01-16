#pragma once

#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "Agent.hpp"
#include "Learner.hpp"
#include "Q_table.hpp"

#include <iostream>
#include <cassert>
#include <vector>


class Trainer
{
      std::vector<Learner*> learners;
    	// objets ? pierre etc
    	int width;
    	int height;
      // returns a character according to what is in the point (pos_x, pos_y) :
      char charAtPosition(int x, int y);
      // returns true if there's a learner at (x,y), and false otherwise
      bool isSomeoneAtPosition(int x, int y);
      void step();
      void doDamageAroundPoint(int x, int y, float attack_damage);
      bool verifyDeadLearners();
	public:
      //la grille dans laquelle se passe l'action
      Trainer(); // constructeur
      Trainer(int w, int h);
      // print the grid at the current moment
      void printGrid();
      // print the current HPs of all personnages
      void printHPs();
      // commence l'entrainement
      void train();
      int getWidth();
      int getHeight();
};

#endif
