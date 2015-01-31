#include <vector>
#include <time.h>
#include <iostream>

struct Unit{
	int atk, def;
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
	std::vector<Unit> units;
	double idealAtk, idealDef;

	// population = 10 individu
	Chromosome population[10];

public:
	GeneticAlgorithm(int totalTeam, std::vector<Unit> units);
	~GeneticAlgorithm();

	void calculateFitness(Chromosome* chromosome);
	void mutation(Chromosome* chromosome);
	void createFirstGeneration();
};

