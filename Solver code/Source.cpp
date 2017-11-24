/*
NB:
Path is saved in the actions

TODO:

1) Respresent the environment
	1.1) Easier representation
	1.2) Check for solution
	1.3) Generate actions based on position

2) Solve using Dijsktra's algorithm (Least-cost)
	2.1) Get new envirionments from representation
	2.2) Add environments to a heap sorted with regards to cost
	2.3) Pop environment with least cost and apply
	2.4) Repeat this until valid solution is found
		2.4.1) Check for valid solution
		2.4.2) Get new environments from popped environment
		2.4.3) Add current path and current cost to these environments
		2.4.4) Add new environments to the heap
		2.4.5) Pop environment with least cost
	2.5) Extract path from the environment that generated the solution

CLASSES:
	environment
	solver
*/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Environment.h"
#include "Solver.h"

int main() {

	/* 
	e:
	05 06 02 23
	X X X X X		0  1  2  3  4
	X . J G X		5  6  7  8  9
	X . J . X		10 11 12 13 14
	X . G . X		15 16 17 18 19
	X . . . X		20 21 22 23 24
	X X X X X		25 26 27 28 29
	result: "XXXXXX.JGXX.J.XX.G.XX...XXXXXX"

	eT:
	05 06 02 23
	X X X X X		0  1  2  3  4
	X X . G X		5  6  7  8  9
	X . . J X		10 11 12 13 14
	X . J . X		15 16 17 18 19
	X . . G X		20 21 22 23 24
	X X X X X		25 26 27 28 29
	result: "XXXXXXX.GXX..JXX.J.XX..GXXXXXX"

	e2:
	07 12 04 47
	XXXXXXX
	X..XXXX
	X..J..X
	X.JJ..X
	XX.J..X
	X...XXX
	X..X..X
	X...G.X
	XXXGGGX
	XXXX..X
	XXXX..X
	XXXXXXX
	result: "XXXXXXXX..XXXXX..J..XX.JJ..XXX.J..XX...XXXX..X..XX...G.XXXXGGGXXXXX..XXXXX..XXXXXXXX"
	
	*/

	//std::string e = "XXXXXXX.GXX..JXX.J.XX..GXXXXXX";
	std::string e = "XXXXXXX.GXX..JXX.J.XX..GXXXXXX";

	int w = 5;
	int l = 6;
	int pos = 23;
	int rot = 0;

	Environment env(e, l, w, pos, rot);
	Solver s(env);
	Environment solution = s.getSolution();

	std::cout << "Solution: " << std::endl;
	std::cout << "SE: " << solution.getSE() << std::endl;
	std::cout << "POS: " << solution.getPos() << std::endl;
	std::cout << "ROT: " << solution.getRot() << std::endl;
	std::cout << "COST: " << solution.getCost() << std::endl;
	std::cout << "PATH: ";
	solution.printPath();
	std::cout << std::endl;

	// Print solution path to file
	std::ofstream path;
	path.open("path.txt");
	path << "{";
	for (size_t i = 0; i < (solution.getPath().size() - 1); i++) {
		path << solution.getPath()[i] << ", ";
	}
	path << solution.getPath().back() << "};";


	/* Test of Environment class
	std::cout << "Is e solved: " << env.isSolved() << std::endl;

	std::vector<Environment> newEnv = env.getActions();

	std::cout << "generated envs from pos " << pos << " and rot " << rot << ":" << std::endl;
	std::cout << "generated " << newEnv.size() << " new envs" << std::endl << std::endl;

	for (size_t i = 0; i < newEnv.size(); i++) {
		std::cout << "new env " << i + 1 << std::endl;
		std::cout << "path ";
		std::cout << std::endl;
		newEnv[i].printPath();
		std::cout << "cost " << newEnv[i].getCost();
		std::cout << std::endl;

	}
	*/


	return 0;
}