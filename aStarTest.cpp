//To compile on Windows with mingw-w64:
//g++ -std=c++17 aStarTest.cpp aStar.cpp -Wall -lmingw32 -o aStar
#include "aStar.hpp"
int main(int argc, char** args){
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
	aStar<>::run(test);
	return 0;
}