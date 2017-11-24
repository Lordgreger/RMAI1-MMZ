#pragma once
#include <string>
#include <vector>
#include <iostream>
/*
Environment should contain:
String representing the environment - string
Function to get new environments based on a position and orientation of the robot
Function to check if problem is solved

NB regarding maps:
space = nontravesal
X = wall = nontravesal
J = diamond
G = destination
dot = traversal
M = starting pos

our definitions:
F = diamond on diamond destination

NB regarding rot:
	0
3	x	1
	2

NB regarding path:
1 = forward
2 = turn left
3 = turn right
5 = push diamond forward a bit
*/

class Environment {

public:
	Environment();
	Environment(std::string iSE, int ilenght, int iwidth, int ipos, int irot);
	Environment(std::string iSE, int ilenght, int iwidth, int icost, std::vector<int> ipath, int ipos, int irot);
	~Environment();

	std::vector<Environment> getActions() const; // Generates new environments based on available actions
	std::vector<Environment> explore() const;
	bool isSolved() const; // Returns true when the environment is a valid solution

	// Get functionality
	int getCost() const { return cost; }
	int getPos() const { return pos; }
	int getRot() const { return rot; }
	void setRot(const int x) { rot = x; }
	int getLenght() const { return lenght; }
	int getWidth() const { return width; }
	std::string getSE() const { return SE; }
	std::vector<int> getPath() const { return path; }

	// Misc functionality
	bool indexInBound(int pos, std::string SE) const;
	void printPath() const;

	// Exploration functions
	Environment exploreUp(int forwardCost, int turnCost) const;
	Environment exploreDown(int forwardCost, int turnCost) const;
	Environment exploreRight(int forwardCost, int turnCost) const;
	Environment exploreLeft(int forwardCost, int turnCost) const;

private:
	int cost, pos, rot, lenght, width;
	std::vector<int> path;
	std::string SE;

	

};

