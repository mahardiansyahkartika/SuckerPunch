#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "GeneticAlgorithm.hpp"

std::vector<Unit> loadCSV(std::string fileLocation, int& totalTeam) {
	std::vector<Unit> units;
	std::ifstream infile(fileLocation);

	bool isFirstLine = true; // flag for checking first line
	while (infile) {
		std::string s;
		if (!std::getline(infile, s)) break;

		std::istringstream ss(s);

		bool isAssignATK = true;
		Unit unit;

		while (ss) {
			std::string s;
			if (!std::getline(ss, s, ',')) break;

			if (isFirstLine) { // first line for totalTeam
				totalTeam = std::stoi(s);
			}
			else { // assign to vector
				if (isAssignATK) { // assign for atk
					unit.atk = std::stoi(s);
					isAssignATK = false;
				}
				else { // assign for def
					unit.def = std::stoi(s);
				}
			}
		}

		// insert unit to units
		if (!isFirstLine)
			units.push_back(unit);
		else
			isFirstLine = false;
	}

	if (!infile.eof())
		std::cerr << "error loading file" << std::endl;

	return units;
}

void printCSV(int totalTeam, std::vector<Unit> units) {
	std::cout << totalTeam << std::endl;
	for (unsigned int i = 0; i < units.size(); i++)
		std::cout << units[i].atk << "," << units[i].def << std::endl;
}

int main(int argc, char* argv[])
{
	int totalTeam;
	std::vector<Unit> units = loadCSV("input.csv", totalTeam);
	printCSV(totalTeam, units);

	// Genetic Algorithm
	int population = 10; // total individu in one population
	int generation = 20;
	GeneticAlgorithm* ga = new GeneticAlgorithm(totalTeam, units, population, generation);
	ga->findOptimumSolution();

	getchar();
	return 0;
}

