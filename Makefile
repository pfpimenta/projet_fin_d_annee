CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

all: projetFA

projetFA: main.o q_table.o Agent.o GameManager.o
	g++ $^ -o $@ -lIrrlicht

main.o q_table.o Agent.o GameManager.o: q_table.hpp Agent.hpp GameManager.hpp


clean:
	@rm -f projetFA *.o
