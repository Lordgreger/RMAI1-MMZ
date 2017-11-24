#pragma once
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include "Environment.h"

class Solver {

public:
	Solver();
	Solver(Environment ie);
	~Solver();

	Environment getSolution();

private:
	Environment e;

	bool vectorEqual(std::vector<int> l, std::vector<int> r);
};

