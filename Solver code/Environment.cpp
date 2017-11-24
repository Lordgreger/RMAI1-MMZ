#include "Environment.h"

#define goForward 2
#define turnRight 3
#define turnLeft 4
#define pushExtra 5


Environment::Environment()
{
}

Environment::Environment(std::string iSE, int ilenght, int iwidth, int ipos, int irot)
{
	SE = iSE;
	lenght = ilenght;
	width = iwidth;
	pos = ipos;
	rot = irot;
	cost = 0;
	path.clear();
}

Environment::Environment(std::string iSE, int ilenght, int iwidth, int icost, std::vector<int> ipath, int ipos, int irot)
{
	SE = iSE;
	lenght = ilenght;
	width = iwidth;
	pos = ipos;
	rot = irot;
	cost = icost;
	path = ipath;
}

Environment::~Environment()
{
}

std::vector<Environment> Environment::getActions() const
{
	int costForward = 1;
	int costTurn = 2;
	std::vector<Environment> out;


	// Add the forward part
	int newPos;
	int newRot;
	int indexManipulator;
	std::vector<int> newPath;
	std::string newSE;

	switch (rot) {

		case 0: // The robot is pointing up
			indexManipulator = - width;
			newPos = pos + indexManipulator;
			break;

		case 1:
			indexManipulator = 1;
			newPos = pos + indexManipulator;
			break;

		case 2:
			indexManipulator = width;
			newPos = pos + indexManipulator;
			break;

		case 3:
			indexManipulator = -1;
			newPos = pos + indexManipulator;
			break;

		default:
			break;
	}

	if (indexInBound(newPos, SE)) { // Test for indexing error
		if (SE[newPos] == '.' || SE[newPos] == 'G') { // Traversable space infront of robot without diamonds
			//std::cout << "FREE PATH FORWARD " << newPos << std::endl;
			newPath = path;
			newPath.push_back(1);
			Environment newForwardEnvironment(SE, lenght, width, cost + costForward, newPath, newPos, rot);
			out.push_back(newForwardEnvironment);
			//std::cout << "newForwardEnvironment SE: " << newForwardEnvironment.getSE() << std::endl;
			//std::cout << "newForwardEnvironment COST: " << newForwardEnvironment.getCost() << std::endl;
			//std::cout << "newForwardEnvironment PATH: ";
			//newForwardEnvironment.printPath();
			//std::cout << std::endl;
		}
		else if (SE[newPos] == 'J' || SE[newPos] == 'F') { // Diamond infront
			//std::cout << "DIAMOND INFRONT" << std::endl;
			if (indexInBound(newPos + indexManipulator, SE)) { // Test for indexing error
				if (SE[newPos + indexManipulator] == '.' || SE[newPos + indexManipulator] == 'G') { // Is the spot behind diamond free?
					if (SE[newPos] == 'J' && SE[newPos + indexManipulator] == '.') { // Diamond and normal free space
						newSE = SE;
						newSE[newPos] = '.';
						newSE[newPos + indexManipulator] = 'J';
					}
					else if (SE[newPos] == 'J' && SE[newPos + indexManipulator] == 'G') { // Diamond and diamond placement
						newSE = SE;
						newSE[newPos] = '.';
						newSE[newPos + indexManipulator] = 'F';
					}
					else if (SE[newPos] == 'F' && SE[newPos + indexManipulator] == '.') { // Diamond at placement and free space
						newSE = SE;
						newSE[newPos] = 'G';
						newSE[newPos + indexManipulator] = 'J';
					}
					else { // Diamond at placement and diamond placement
						newSE = SE;
						newSE[newPos] = 'G';
						newSE[newPos + indexManipulator] = 'F';
					}

					newPath = path; // Create propper path based on diamond movement

					if (!newPath.empty()) { // Check if path is empty
						if (newPath.back() == 5) { // Rework previous forward movement
							newPath.back() = 1;
							newPath.push_back(5);
						}
						else { // Add forward movement and add extra push for diamond
							newPath.push_back(1);
							newPath.push_back(5);
						}
					}
					else { // path is empty
						newPath.push_back(1);
						newPath.push_back(5);
					}


					Environment newForwardEnvironment(newSE, lenght, width, (cost + costForward), newPath, newPos, rot);
					out.push_back(newForwardEnvironment);
					//std::cout << "newForwardEnvironment SE: " << newForwardEnvironment.getSE() << std::endl;
					//std::cout << "newForwardEnvironment COST: " << newForwardEnvironment.getCost() << std::endl;
					//std::cout << "newForwardEnvironment PATH: ";
					//newForwardEnvironment.printPath();
					//std::cout << std::endl;
				}
			}
		}
	}

	// Add the turn left part (can always rotate)
	newPath = path;
	newPath.push_back(2); // Rotate left
	newRot = rot - 1;
	if (rot < 0) // create rotation loop
		newRot = 4;
	Environment newLeftEnvironment(SE, lenght, width, (cost + costTurn), newPath, pos, newRot);
	out.push_back(newLeftEnvironment);

	// Add the turn right part (can always rotate)
	newPath = path;
	newPath.push_back(3); // Rotate right
	newRot = rot + 1;
	if (rot > 4) // create rotation loop
		newRot = 0;
	Environment newRightEnvironment(SE, lenght, width, (cost + costTurn), newPath, pos, newRot);
	out.push_back(newRightEnvironment);

	return out;
}

std::vector<Environment> Environment::explore() const
{
	// New version taking moving only instead of turning aswell
	// Path needs to be generated based on direction of movement and rotation of robot
	// Params
	int forwardCost = 1;
	int turnCost = 2;
	std::vector<Environment> out;
	

	// Up
	Environment up = exploreUp(1, 2);
	if (up.getSE() != "") { // Returned valid exploration
		out.push_back(up);
	}

	// Right
	Environment right = exploreRight(1, 2);
	if (right.getSE() != "") { // Returned valid exploration
		out.push_back(right);
	}

	// Down
	Environment down = exploreDown(1, 2);
	if (down.getSE() != "") { // Returned valid exploration
		out.push_back(down);
	}

	// Left
	Environment left = exploreLeft(1, 2);
	if (left.getSE() != "") { // Returned valid exploration
		out.push_back(left);
	}

	return out;
}

bool Environment::isSolved() const
{
	if (SE.find("J") == std::string::npos) { // No diamonds at non valid place
		if (SE.find("G") == std::string::npos) { // No diamond placements missing diamond
			return true;
		}
	}
	return false;
}

bool Environment::indexInBound(int pos, std::string SE) const
{
	if (pos < 0 || pos > SE.size() - 1)
		return false;
	return true;
}

void Environment::printPath() const
{
	for (size_t i = 0; i < path.size(); i++) {
		std::cout << path[i] << " ";
	}
}

Environment Environment::exploreUp(int forwardCost, int turnCost) const
{
	char nextPlace;
	bool movementPossible = false;
	bool diamondPush = false;
	std::string newSE;

	// Is up valid operation and generate new SE if it is
	if (indexInBound(pos - width, SE)) { // Can the robot move up?
		nextPlace = SE[pos - width];
		if (nextPlace == '.' || nextPlace == 'G') { // Next place is free space or next place is diamond destination
			newSE = SE;
			movementPossible = true;
		}

		else if (nextPlace == 'J') { // Next place is diamond
			if (indexInBound(pos - width - width, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos - width - width];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos - width - width] = 'J';
					newSE[pos - width] = '.';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos - width - width] = 'F';
					newSE[pos - width] = '.';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}

		else if (nextPlace == 'F') { // Next place is diamond on diamond destination
			if (indexInBound(pos - width - width, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos - width - width];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos - width - width] = 'J';
					newSE[pos - width] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos - width - width] = 'F';
					newSE[pos - width] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}
	}

	// Calculate path and cost based on rot
	if (movementPossible) {
		std::vector<int> newPath = path;
		int newCost = cost;
		int newRot = 0;
		if (rot == 0) {
			newPath.push_back(goForward); // forward
			newCost += forwardCost; // add cost of actions
		}
		else if (rot == 1) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += turnCost + forwardCost; // add cost of actions
		}
		else if (rot == 2) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += turnCost + turnCost + forwardCost; // add cost of actions
		}
		else if (rot == 3) {
			newPath.push_back(turnRight); // right
			newPath.push_back(goForward); // forward
			newCost += turnCost + forwardCost; // add cost of actions
		}

		if (diamondPush) {
			newPath.push_back(pushExtra);
			newRot = 2;
		}

		int newPos = pos - width;
		return Environment(newSE, lenght, width, newCost, newPath, newPos, newRot); // Crate and return new environment
	}
		return Environment();
}

Environment Environment::exploreDown(int forwardCost, int turnCost) const
{
	char nextPlace;
	bool movementPossible = false;
	bool diamondPush = false;
	std::string newSE;

	// Is down valid operation and generate new SE if it is
	if (indexInBound(pos + width, SE)) { // Can the robot move down?
		nextPlace = SE[pos + width];
		if (nextPlace == '.' || nextPlace == 'G') { // Next place is free space or next place is diamond destination
			newSE = SE;
			movementPossible = true;
		}

		else if (nextPlace == 'J') { // Next place is diamond
			if (indexInBound(pos + width + width, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos + width + width];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos + width + width] = 'J';
					newSE[pos + width] = '.';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos + width + width] = 'F';
					newSE[pos + width] = '.';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}

		else if (nextPlace == 'F') { // Next place is diamond on diamond destination
			if (indexInBound(pos + width + width, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos + width + width];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos + width + width] = 'J';
					newSE[pos + width] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos + width + width] = 'F';
					newSE[pos + width] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}
	}

	// Calculate path and cost based on rot
	if (movementPossible) {
		std::vector<int> newPath = path;
		int newCost = cost;
		int newRot = 2;
		if (rot == 0) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += turnCost + turnCost + forwardCost; // add cost of actions
		}
		else if (rot == 1) {
			newPath.push_back(turnRight); // right
			newPath.push_back(goForward); // forward
			newCost += turnCost + forwardCost; // add cost of actions
		}
		else if (rot == 2) {
			newPath.push_back(goForward); // forward
			newCost += forwardCost; // add cost of actions
		}
		else if (rot == 3) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += turnCost + forwardCost; // add cost of actions
		}

		if (diamondPush) {
			newPath.push_back(pushExtra);
			newRot = 0;
		}

		int newPos = pos + width;
		return Environment(newSE, lenght, width, newCost, newPath, newPos, newRot); // Crate and return new environment
	}
	return Environment();
}

Environment Environment::exploreRight(int forwardCost, int turnCost) const
{
	char nextPlace;
	bool movementPossible = false;
	bool diamondPush = false;
	std::string newSE;

	// Is down valid operation and generate new SE if it is
	if (indexInBound(pos + 1, SE)) { // Can the robot move down?
		nextPlace = SE[pos + 1];
		if (nextPlace == '.' || nextPlace == 'G') { // Next place is free space or next place is diamond destination
			newSE = SE;
			movementPossible = true;
		}

		else if (nextPlace == 'J') { // Next place is diamond
			if (indexInBound(pos + 1 + 1, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos + 1 + 1];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos + 1 + 1] = 'J';
					newSE[pos + 1] = '.';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos + 1 + 1] = 'F';
					newSE[pos + 1] = '.';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}

		else if (nextPlace == 'F') { // Next place is diamond on diamond destination
			if (indexInBound(pos + 1 + 1, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos + 1 + 1];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos + 1 + 1] = 'J';
					newSE[pos + 1] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos + 1 + 1] = 'F';
					newSE[pos + 1] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}
	}

	// Calculate path and cost based on rot
	if (movementPossible) {
		std::vector<int> newPath = path;
		int newCost = cost;
		int newRot = 1;
		if (rot == 0) {
			newPath.push_back(turnRight); // right
			newPath.push_back(goForward); // forward
			newCost += turnCost + forwardCost; // add cost of actions
		}
		else if (rot == 1) {
			newPath.push_back(goForward); // forward
			newCost += forwardCost; // add cost of actions
		}
		else if (rot == 2) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += forwardCost + turnCost; // add cost of actions
		}
		else if (rot == 3) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += turnCost + turnCost + forwardCost; // add cost of actions
		}

		if (diamondPush) {
			newPath.push_back(pushExtra);
			newRot = 3;
		}

		int newPos = pos + 1;
		return Environment(newSE, lenght, width, newCost, newPath, newPos, newRot); // Crate and return new environment
	}
	return Environment();
}

Environment Environment::exploreLeft(int forwardCost, int turnCost) const
{
	char nextPlace;
	bool movementPossible = false;
	bool diamondPush = false;
	std::string newSE;

	// Is up valid operation and generate new SE if it is
	if (indexInBound(pos - 1, SE)) { // Can the robot move up?
		nextPlace = SE[pos - 1];
		if (nextPlace == '.' || nextPlace == 'G') { // Next place is free space or next place is diamond destination
			newSE = SE;
			movementPossible = true;
		}

		else if (nextPlace == 'J') { // Next place is diamond
			if (indexInBound(pos - 1 - 1, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos - 1 - 1];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos - 1 - 1] = 'J';
					newSE[pos - 1] = '.';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos - 1 - 1] = 'F';
					newSE[pos - 1] = '.';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}

		else if (nextPlace == 'F') { // Next place is diamond on diamond destination
			if (indexInBound(pos - 1 - 1, SE)) { // Can the diamond be moved and still be inside bounds?
				nextPlace = SE[pos - 1 - 1];
				if (nextPlace == '.') { // Place behind diamond is free space
					newSE = SE;
					newSE[pos - 1 - 1] = 'J';
					newSE[pos - 1] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
				else if (nextPlace == 'G') { // Place behind diamond is diamond destination
					newSE = SE;
					newSE[pos - 1 - 1] = 'F';
					newSE[pos - 1] = 'G';
					movementPossible = true;
					diamondPush = true;
				}
			}
		}
	}

	// Calculate path and cost based on rot
	if (movementPossible) {
		std::vector<int> newPath = path;
		int newCost = cost;
		int newRot = 3;
		if (rot == 0) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += forwardCost + turnCost; // add cost of actions
		}
		else if (rot == 1) {
			newPath.push_back(turnLeft); // left
			newPath.push_back(turnLeft); // left
			newPath.push_back(goForward); // forward
			newCost += turnCost + forwardCost + turnCost; // add cost of actions
		}
		else if (rot == 2) {
			newPath.push_back(turnRight); // right
			newPath.push_back(goForward); // forward
			newCost += turnCost + forwardCost; // add cost of actions
		}
		else if (rot == 3) {
			newPath.push_back(goForward); // forward
			newCost += forwardCost; // add cost of actions
		}

		if (diamondPush) {
			newPath.push_back(pushExtra);
			newRot = 1;
		}

		int newPos = pos - 1;
		return Environment(newSE, lenght, width, newCost, newPath, newPos, newRot); // Crate and return new environment
	}
	return Environment();
}
