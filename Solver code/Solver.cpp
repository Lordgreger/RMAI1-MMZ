#include "Solver.h"

struct compare
{
	bool operator()(Environment l, Environment r)
	{
		return l.getCost() > r.getCost();
	}
};

Solver::Solver()
{
}

Solver::Solver(Environment ie)
{
	e = ie;
}


Solver::~Solver()
{
}

Environment Solver::getSolution() {
	
	// Test starting configuration far valid solution
	if (e.isSolved())
		return e;

	// Setup
	std::priority_queue<Environment, std::vector<Environment>, compare> minHeap;
	std::map<std::string, bool> closedSet;

	// Add initial branching to min heap
	std::vector<Environment> newE = e.explore();
	for (size_t i = 0; i < newE.size(); i++) {
		std::string closedSE = newE[i].getSE() + std::to_string(newE[i].getPos());
		minHeap.push(newE[i]);
		closedSet[closedSE] = true;
	}

	// Run until solution is found
	int iterations = 0; // Iterator for information output
	while (true) {
		
		iterations += 1;
		std::cout << std::endl << "ITERATION: " << iterations << std::endl;/*
		std::cout << "SE: " << minHeap.top().getSE() << std::endl;
		std::cout << "POS: " << minHeap.top().getPos() << std::endl;
		std::cout << "COST: " << minHeap.top().getCost() << std::endl;
		std::cout << "PATH: ";
		minHeap.top().printPath();
		std::cout << std::endl;
		*/

		if (minHeap.top().isSolved()) { // Catch for solution
			std::cout << "Iterations: " << iterations << std::endl;
			return minHeap.top();
		}

		// Generate new environments
		std::vector<Environment> newEForInspection = minHeap.top().explore();
		for (size_t i = 0; i < newEForInspection.size(); i++) {
			std::string closedSE = newEForInspection[i].getSE() + std::to_string(newEForInspection[i].getPos()); // Create a string representing the necessary information for the closed set
			auto search = closedSet.find(closedSE);
			if (search == closedSet.end()) { // Did not find closedSE in closedSet, meaning new configuration
				minHeap.push(newEForInspection[i]); // Add to open set
				closedSet[closedSE] = true; // Add to closed set
			}
			else {
				//std::cout << "found baddie" << std::endl;
			}
			
		}

		minHeap.pop();
	}
	return minHeap.top();
}

bool Solver::vectorEqual(std::vector<int> l, std::vector<int> r)
{
	if (l.size() == r.size()) {
		for (size_t i = 0; i < l.size(); i++) {
			if (l[i] != r[i])
				return false;
		}
		return true;
	}
	return false;
}
