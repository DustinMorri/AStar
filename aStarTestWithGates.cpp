//g++ -std=c++17 aStarTestWithGates.cpp aStar.cpp -Wall -lmingw32 -o aStar
#include "aStar.hpp"
#include <set>
int main(int argc, char** args){
	//The anchor problem.
	std::vector<std::vector<unsigned short>> test {
		{0,0,0,0,0,2,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0},
		{0,1,0,0,0,1,0,0,0,1,0},
		{0,1,0,0,0,1,0,0,0,1,0},
		{0,1,0,0,0,1,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,3,0,0,0,0,0}
	};
	//Make it to where the algorithm has to go through two gates.
	std::set<standardVertex*> gates;
	//These are hand input gates, but you can use whatever methods you want to find them.
	gates.emplace(new standardVertex(5,10));
	gates.emplace(new standardVertex(10,10));
	aStar<>::estimatedCost = [&gates](standardVertex* current,standardVertex* destination){
		float cost = 0;
		if(gates.size()!=0){
			standardVertex* gate = *gates.begin();
			if(current->x==gate->x&&current->y==gate->y) gates.erase(gate);
			for(standardVertex* v : gates){
				cost += manhattanDistance(current,v);
				current = v;				
			}
		}
		cost += manhattanDistance(current,destination);
		return cost;
	};
	aStar<>::run(test);
	return 0;
}