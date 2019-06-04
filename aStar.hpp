#ifndef aStar_h
#define aStar_h
#include <cmath> //Euclidean distance
#include <cstdio> //Print to console
#include <algorithm> //Smallest element in fScore
#include <deque> //To hold paths
#include <functional> //Function pointers for functions the user can redefine
#include <iostream> //Print to console
#include <map> //To keep score
#include <unordered_map> //To hold edges
#include <unordered_set> //To hold vertices
#include <vector> //To take in arrays
//This is the default vertex.
//The user can use whatever vertex they want along with whatever variable types in it they want. This is an interface.
struct standardVertex {
	typedef std::unordered_set<standardVertex*> vertices;
	unsigned short x;
	unsigned short y;
	vertices neighbors;
	std::unordered_map<standardVertex*, float> neighborMap;
	standardVertex(unsigned short x, unsigned short y);
	static std::function<vertices(standardVertex*)> getNeighbors;
	void relate(standardVertex* v, float weight);
};
template<class vertex=standardVertex,class score=float>
score euclideanDistance(vertex* a, vertex* b);
template<class vertex=standardVertex,class score=float>
score manhattanDistance(vertex* a, vertex* b);
//This is the structure that will hold information about how the algorithm should run.
//The user can override everything in it.
template<class vertex=standardVertex,class score=float>
struct aStar {
	typedef std::unordered_set<vertex*> vertices;
	static unsigned short rows;
	static unsigned short columns;
	static vertex* source;
	static vertex* destination;
	//This is the distance actually traveled from one point to another.
	//It is only used to find the distance to a neighbor in the algorithm. Precomputed distances to neighbors could speed up the algorithm.
	static std::function<score(vertex*,vertex*)> actualCost;
	//This is the distance estimated to be traveled from one point to another.
	//It is only used to find the distance to the goal in the algorithm.
	static std::function<score(vertex*,vertex*)> estimatedCost;
	static std::function<void(vertices,vertices,vertex*)> printProgress;
	static std::function<void(std::vector<std::vector<unsigned short>>)> run;
	static std::function<std::vector<vertex*>(std::vector<std::vector<unsigned short>>)> vectorToGraph;
};
#endif