#include <iostream>

#include "q_table.hpp"
#include "Agent.hpp"
#include "GameManager.hpp"

int main(){
  std::cout << "test main Q_table" << std::endl;
  
  // pour tester la Q_table
  Q_table qtable = Q_table();
  qtable.printTable();
  
  // pour tester le GameManager
  GameManager gameManager = GameManager(20, 20);
  
  // ajouter des personnage
  Agent personnage_test = Agent(12, 5);
  gameManager.addPersonnage(personnage_test);
  Agent personnage_test2 = Agent(2, 15);
  gameManager.addPersonnage(personnage_test2);
  Agent personnage_test3 = Agent(2, 2);
  gameManager.addPersonnage(personnage_test3);
  
  
  gameManager.printGrid();
  
  
  return 0;
}
