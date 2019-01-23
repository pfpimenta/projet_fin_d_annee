#pragma once

#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "Agent.hpp"
#include "Learner.hpp"
#include "Q_table.hpp"

#include <iostream>
#include <cassert>
#include <vector>

#define DEFAULT_MAX_EPISODES 100000
#define DEFAULT_STEP_PER_EPISODE 10000


class Trainer
{
      std::vector<Learner*> learners;
    	int width;
    	int height;
      // returns a character according to what is in the point (pos_x, pos_y) :
      char charAtPosition(int x, int y);
      // returns true if there's a learner at (x,y), and false otherwise
      bool isSomeoneAtPosition(int x, int y);
      void step();
      void doDamageAroundPoint(int x, int y, float attack_damage);
      bool verifyDeadLearners();
      void executerAction(int learnerIndex, QTableAction a);
      QTableAction deciderAction(int learnerIndex);
      void resetLearners(Q_table* qtable);
	public:
      Trainer(); // constructeur
      Trainer(int w, int h);
      // print the grid at the current moment :
      void printGrid();
      // print the current HPs of all personnages :
      void printHPs();
      // commence l'entrainement :
      void train();
      // pour tester la q-table :
      void test(Q_table* q_table);
      int getWidth();
      int getHeight();
};

#endif
