#include <vector>
#include <time.h>
#include <algorithm>
#include <iostream>

struct Unit{
	int atk, def;
	int index;

	Unit() : atk(0), def(0), index(0) {}
	Unit(int _atk, int _def, int _index) : atk(_atk), def(_def), index(_index) {}
};

class Chromosome 
{
public:
	Chromosome();
	~Chromosome();

	// team1 = {a,b,c}; team2 = {d,e}; team3 = {f,g,h,i};
	// chromosome representation
	// teamSize[] = {3,2,4};
	// units = {a,b,c,d,e,f,g,h,i};
	std::vector<int> teamSize; // store how many unit in every team
	std::vector<Unit> unitList; // array of unit
	double fitness; // fitness value (the smaller the better)
};

class GeneticAlgorithm
{
private:
	int totalTeam;
	int sizePopulation, sizeGeneration;
	std::vector<Unit> units;
	double idealAtk, idealDef;

	// population
	std::vector<Chromosome*> population;

public:
	GeneticAlgorithm(int totalTeam, std::vector<Unit> units, int sizePopulation, int sizeGeneration);
	~GeneticAlgorithm();

	void calculateFitness(Chromosome* chromosome);
	void createFirstGeneration();
	void sortPopulation();
	void setTeamSize(Chromosome* chromosome);
	Chromosome* copyChromosome(Chromosome* chromosome);
	Chromosome* mutation(Chromosome* chromosome);
	Chromosome* createOneChromosome(); // create one random chromosome
	std::vector<std::vector<int>> findOptimumSolution();
	std::vector<std::vector<int>> convertToOutputList(Chromosome* choromosome); // convert chromosome data structure to output list
};

