#include <iostream>
#include <ctime>
#include <cstdlib>

#include "q_table.hpp"
#include "Agent.hpp"
#include "GameManager.hpp"

int main(){
  
  std::srand(std::time(nullptr)); // use current time as seed for random generator

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
  

  int random_variable;
  
  // tester le jeu avec l'affichage au terminal
  while(true){
      gameManager.printGrid();
      std::cin.ignore();
      gameManager.step();
  }
  
  
  return 0;
}
