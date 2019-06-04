/*A* for C++ by Dustin Morrison
The following wiki articles might help you understand this code:
https://en.wikipedia.org/wiki/A*_search_algorithm
https://en.wikipedia.org/wiki/Graph_theory
https://en.wikipedia.org/wiki/Glossary_of_graph_theory_terms*/

#include "aStar.hpp"

//First, make abstract versions of key terminology to make maintenance easier. These will only be recognized in this file.
template<class vertex> using vertices = std::unordered_set<vertex*>;
template<class vertex> using edges = std::unordered_map<vertex*, vertex*>;
template<class vertex> using path = std::deque<vertex*>;
template<class vertex, class score> using scoreMap = std::map<vertex*, score>;

//This function is used to return the vertices that make up the single-pair shortest path given the edges back to the source and the destination vertex.
template<class vertex> path<vertex> reconstructPath(edges<vertex> cameFrom, vertex* current){
	path<vertex> finalPath;
	finalPath.push_front(current);
	while (cameFrom[current]!=current){
		current = cameFrom[current];
		finalPath.push_front(current);
	}
	return finalPath;
}

//This is the algorithm. It should be pretty close to the pseudo code on Wikipedia.
template<class vertex, class score> path<vertex> algorithm(vertex* source, vertex* destination){
	//The set of vertices that have yet to be evaluated.
	vertices<vertex> openSet;
	//The source vertex is the first to be evaluated.
	openSet.emplace(source);
	//The set of vertices that have been evaluated.
	vertices<vertex> closedSet;
	//The way to remember how to get back to the source.
	edges<vertex> cameFrom;
	//The way to remember the source is the source.
	cameFrom.emplace(source,source);
	//The evaluated vertices and the shortest distance back to the source.
	scoreMap<vertex, score> gScore;
	//It takes no distance to get from the source to source.
	gScore[source] = 0;
	//The evaluated vertices and the expected total distance to get to the destination by going through them.
	scoreMap<vertex, score> fScore;
	//We think it will take this much distance to get from the source to the destination.
	fScore[source] = aStar<vertex, score>::estimatedCost(source,destination);
	//The currently being evaluated vertex.
	vertex* current;
	//The shortest distance from the source to the current vertex's neighbor.
	score totalGScore;
	//The expected distance from the source to the destination going through a current vertex's neighbor.
	score totalFScore;
	//While there are still paths to consider and you have not yet found a successful route,
	while(openSet.size()!=0){
		//evaluate the vertex currently expected to have the least total distance that is also in the open set.
		current = *(std::min_element(openSet.begin(),openSet.end(),[&fScore](vertex* a,vertex* b)->bool{
			return fScore[a] < fScore[b];
		}));
		//If you've reached the destination, recall how you got there.
		if(current==destination) return reconstructPath<vertex>(cameFrom, current);
		//After evaluating this vertex, you will not need to do so again.
		openSet.erase(current); closedSet.emplace(current);
		//For all the neighbors of the currently being evaluated vertex,
		for(vertex* neighbor : vertex::getNeighbors(current)){
			//If the neighbor has already been ruled out, start evaluating the next neighbor or stop.
			if(closedSet.find(neighbor)!=closedSet.end()) continue;
			//Remember how far away this neighbor is from the source.
			totalGScore = gScore.at(current) + aStar<vertex, score>::actualCost(current,neighbor);
			//If the neighbor has not been evaluated, queue it to be.
			if(openSet.find(neighbor)==openSet.end()) openSet.emplace(neighbor);
			//If the neighbor has been evaluated, but its distance from the source is greater than a previously evaluated route, start evaluating the next neighbor or stop.
			else if(totalGScore >= gScore.at(neighbor)) continue;
			//Remember the current route. If the route already exists, remember your new way of getting there.
			cameFrom[neighbor] = current;
			//Remember the distance from the source.
			gScore[neighbor] = totalGScore;
			//Estimate the total cost of a path from here.
			totalFScore = totalGScore + aStar<vertex, score>::estimatedCost(neighbor, destination);
			//Remember the estimated total distance.
			fScore[neighbor] = totalFScore;
		}
		//Show your progress through a user defined function which is by default empty.
		aStar<vertex, score>::printProgress(openSet, closedSet, current);
	}
	//If you have made it here, there were no successful routes.
	path<vertex> dummy;
	return dummy;
}

standardVertex::standardVertex(unsigned short x, unsigned short y){
	this->x = x;
	this->y = y;
}

std::function<vertices<standardVertex>(standardVertex*)> standardVertex::getNeighbors = [](standardVertex* v){
	return v->neighbors;
};

void standardVertex::relate(standardVertex* v, float weight){
	if(v!=NULL){
		neighbors.emplace(v);
		neighborMap.emplace(v,weight);
		v->neighbors.emplace(this);
		v->neighborMap.emplace(this,weight);
	}
}

template<> float euclideanDistance(standardVertex* a,standardVertex* b){
	float cost = std::sqrt(std::pow(std::abs(b->x - a->x),2)+std::pow(std::abs(b->y - a->y),2));
	return cost;
}

template<> float manhattanDistance(standardVertex* a,standardVertex* b){
	float cost = std::abs(b->x - a->x) + std::abs(b->y - a->y);
	return cost;
}

//Below are the defaults for the aStar structure.
template<> unsigned short aStar<>::rows = 11;
template<> unsigned short aStar<>::columns = 11;
template<> standardVertex* aStar<>::source = new standardVertex(0,0);
template<> standardVertex* aStar<>::destination = new standardVertex(rows-1,columns-1);
//The actual cost is only ever called to determine the cost of traveling along one edge.
template<> std::function<float(standardVertex*,standardVertex*)> aStar<>::actualCost = [](standardVertex* a, standardVertex* b){
	return a->neighborMap.at(b);
};
//The estimated cost is only ever called to estimate the distance from the current point to the destination.
template<> std::function<float(standardVertex*,standardVertex*)> aStar<>::estimatedCost = [](standardVertex* current, standardVertex* destination){
	return manhattanDistance(current,destination);
};
template<> std::function<void(vertices<standardVertex>,vertices<standardVertex>,standardVertex*)> aStar<>::printProgress = [](vertices openSet,vertices closedSet,standardVertex* current){};
template<> std::function<void(std::vector<std::vector<unsigned short>>)> aStar<>::run = [](std::vector<std::vector<unsigned short>> array){
	//In order to make the test setup easier use the helper function to turn the array into the corresponding vertices.
	std::vector<standardVertex*> graphVector = vectorToGraph(array);
	//The default printProgress does nothing, and also doesn't know the whole graph.
	//Override it so that it shows the open and closed set, and the current vertex in place.
	aStar<>::printProgress = [array](aStar<>::vertices openSet, aStar<>::vertices closedSet, standardVertex* current){
		//For all the vertices in the graph,
		for(unsigned short i=0;i<aStar<>::rows;i++){
			for(unsigned short j=0;j<aStar<>::columns;j++){
				//if the current vertex is in the open set,
				for(standardVertex* v : openSet){
					if(v->x==i&&v->y==j){
						//mark it with a 4.
						std::cout << 4;
						goto printProgressCellFinished;
					}
				}
				//If the current vertex is in the closed set,
				for(standardVertex* v : closedSet){
					if(v->x==i&&v->y==j){
						//mark it with a 5.
						std::cout << 5;
						goto printProgressCellFinished;
					}
				}
				//For all the other vertices, use its actual number.
				//0 for unevaluated vertex, 1 for wall, 2 for source, 3 for destination.
				std::cout << array[i][j];
				printProgressCellFinished:
				//If this vertex is the same as the vertex currently being evaluated mark it with a C instead of whatever it should have been.
				if(current->x==i&&current->y==j) std::cout << "\bC";
			}
			//Print a new line after every row.
			std::cout << std::endl;
		}
		//Print a new line after displaying the whole graph.
		std::cout << std::endl;
	};
	//Run the algorithm.
	algorithm<standardVertex,float>(source,destination);
	//Clean up the memory.
	for(standardVertex* v : graphVector) delete v;
};
template<> std::function<std::vector<standardVertex*>(std::vector<std::vector<unsigned short>>)> aStar<>::vectorToGraph = [](std::vector<std::vector<unsigned short>> array){
	//A temporary vector to remember where the walls are by simply having NULL pointers like how the vertices will remember them.
	std::vector<standardVertex*> graphVector;
	//For all the vertices in the graph,
	for(unsigned short row = 0; row < rows; row++){
		for(unsigned short column = 0; column < columns; column++){
			//If the vertex is not a wall,
			if(array[row][column]!=1){
				//make a new vertex if it's not the source or destination.
				standardVertex* v;
				if(array[row][column]==0) v = new standardVertex(row,column);
				else if(array[row][column]==2){
					v = source;
					source->x = row;
					source->y = column;
				}
				else if(array[row][column]==3){
					v = destination;
					destination->x = row;
					destination->y = column;
				}
				//Relate the vertex to it's neighbors.
				//middle
				if(row>0&&row<rows-1){
					//middle
					if(column>0&&column<columns-1){
						v->relate(graphVector[graphVector.size()-columns-1],1);
						v->relate(graphVector[graphVector.size()-columns],1);
						v->relate(graphVector[graphVector.size()-columns+1],1);
						v->relate(graphVector[graphVector.size()-1],1);
					}
					//left
					else if(column==0){
						v->relate(graphVector[graphVector.size()-columns],1);
						v->relate(graphVector[graphVector.size()-columns+1],1);
					}
					//right
					else{
						v->relate(graphVector[graphVector.size()-columns-1],1);
						v->relate(graphVector[graphVector.size()-columns],1);
						v->relate(graphVector[graphVector.size()-1],1);
					}
				}
				//top
				else if(row==0){
					//middle and right
					if(column!=0){
						v->relate(graphVector[graphVector.size()-1],1);
					}
					//left
				}
				//bottom
				else{
					//middle
					if(column>0&&column<columns-1){
						v->relate(graphVector[graphVector.size()-columns-1],1);
						v->relate(graphVector[graphVector.size()-columns],1);
						v->relate(graphVector[graphVector.size()-columns+1],1);
						v->relate(graphVector[graphVector.size()-1],1);
					}
					//left
					else if(column==0){
						v->relate(graphVector[graphVector.size()-columns],1);
						v->relate(graphVector[graphVector.size()-columns+1],1);
					}
					//right
					else{
						v->relate(graphVector[graphVector.size()-columns-1],1);
						v->relate(graphVector[graphVector.size()-columns],1);
						v->relate(graphVector[graphVector.size()-1],1);
					}
				}
				graphVector.push_back(v);
			}else graphVector.push_back(NULL);
		}
	}
	return graphVector;
};