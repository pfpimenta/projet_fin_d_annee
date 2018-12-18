#include <iostream>

#include "q_table.hpp"

int main(){
  std::cout << "test main Q_table" << std::endl;
  Q_table qtable = Q_table();
  qtable.printTable();
  return 0;
}
