CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

projetFA: main.o q_table.o
	g++ $^ -o $@ -lIrrlicht

main.o q_table.o: q_table.hpp
clean:
	@rm -f projetFA *.o
