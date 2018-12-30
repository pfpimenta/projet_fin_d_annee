#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>


#include "../lib/q_table.hpp"
#include "../lib/Agent.hpp"
#include "../lib/GameManager.hpp"

int main(){
  
  std::srand(std::time(nullptr)); // use current time as seed for random generator

  std::cout << "test main Q_table" << std::endl;
  
  // pour tester la Q_table
  Q_table qtable = Q_table();
  qtable.printTable();
  
  // pour tester le GameManager
  GameManager gameManager = GameManager(7, 7);
  
  // ajouter des personnage
  Agent personnage_test = Agent(2, 5);
  gameManager.addPersonnage(&personnage_test);
  Agent personnage_test2 = Agent(5, 2);
  gameManager.addPersonnage(&personnage_test2);
  Agent personnage_test3 = Agent(2, 2);
  gameManager.addPersonnage(&personnage_test3);
  

  // tester le jeu avec l'affichage au terminal
  while(true){
      std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // clear
      gameManager.printGrid();
      gameManager.printHPs();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
      gameManager.step();
  }
  return 0;
}

