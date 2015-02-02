// <author> Mahardiansyah Kartika </author>
// <email> mahardiansyahkartika@gmail.com </email>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "GeneticAlgorithm.hpp"

std::vector<Unit> loadCSV(std::string fileName, int& totalTeam) {
	std::vector<Unit> units;
	std::ifstream infile(fileName);

	bool isFirstLine = true; // flag for checking first line
	int idx = 0;
	while (infile) {
		std::string s;
		if (!std::getline(infile, s)) break;

		std::istringstream ss(s);

		bool isAssignATK = true;
		Unit unit;

		// set index
		if (!isFirstLine) {
			unit.index = idx;
			idx++;
		}

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

void createCSV(std::string fileName, std::vector<std::vector<int>> result) {
	std::ofstream myfile;
	myfile.open(fileName);
	
	std::string writtenString;

	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result[i].size(); j++) {
			writtenString += std::to_string(result[i][j]);

			if (j < result[i].size() - 1) 
				writtenString += ",";
			else // last index
				if (i < result.size() - 1)
					writtenString += "\n";
		}
	}
	
	myfile << writtenString;
	myfile.close();
}

void printInput(int totalTeam, std::vector<Unit> units) {
	std::cout << totalTeam << std::endl;
	for (unsigned int i = 0; i < units.size(); i++)
		std::cout << units[i].atk << "," << units[i].def << std::endl;
	
	std::cout << std::endl;
}

void printOutput(std::vector<std::vector<int>> result, std::vector<Unit> units) {
	int totalAtk = 0, totalDef = 0;
	
	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result[i].size(); j++) {
			int index = result[i][j];
			
			totalAtk += units[index].atk;
			totalDef += units[index].def;
			std::cout << index;

			// last index
			if (j == result[i].size() - 1) std::cout << "; totalAtk : " << totalAtk << "; totalDef : " << totalDef;
			else std::cout << ",";
		}
		totalAtk = 0; totalDef = 0;
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[])
{
	int totalTeam;
	std::vector<Unit> units = loadCSV("input.csv", totalTeam);
	printInput(totalTeam, units);

	// Genetic Algorithm
	int population = 10; // total individu in one population
	int generation = 15; // total generation to get best chromosome
	GeneticAlgorithm* ga = new GeneticAlgorithm(totalTeam, units, population, generation);
	// find optimum solution
	std::vector<std::vector<int>> result = ga->findOptimumSolution();
	// create output file
	createCSV("output.csv", result);

	// print output
	printOutput(result, units);

	getchar();
	return 0;
}

