#include "GeneticAlgorithm.hpp"

Chromosome::Chromosome() {

}

Chromosome::~Chromosome() {
	
}

GeneticAlgorithm::GeneticAlgorithm(int totalTeam, std::vector<Unit> units) {
	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	this->totalTeam = totalTeam;
	this->units = units;

	int totalAtk = 0; int totalDef = 0;
	// calculate total atk & def from all units
	for (unsigned int i = 0; i < this->units.size(); i++) {
		totalAtk += this->units[i].atk;
		totalDef += this->units[i].def;
	}

	// ideal total atk & def for everyteam
	idealAtk = (double)totalAtk / (double)this->totalTeam;
	idealDef = (double)totalDef / (double)this->totalTeam;

	// generate first generation
	createFirstGeneration();
}

GeneticAlgorithm::~GeneticAlgorithm() {}

void GeneticAlgorithm::calculateFitness(Chromosome* chromosome) {
	// initialize index to get total atk & def for everyeteam
	int firstIndex = 0;
	int lastIndex = chromosome->teamSize[0];

	double fitness = 0;

	for (int i = 0; i < totalTeam; i++) {
		int totalAtkGroup = 0;
		int totalDefGroup = 0;

		for (int j = firstIndex; j < lastIndex; j++) {
			totalAtkGroup += chromosome->unitList[j].atk;
			totalDefGroup += chromosome->unitList[j].def;
		}
		
		// fitness = check difference between total atk/def with ideal atk/def -> the best fitness = 0 (balance team)
		fitness += abs(totalAtkGroup - idealAtk) + abs(totalDefGroup - idealDef);

		// increment index
		if (i < totalTeam - 1) {
			firstIndex += chromosome->teamSize[i];
			lastIndex += chromosome->teamSize[i + 1];
		}
	}
	
	chromosome->fitness = fitness;
}

void GeneticAlgorithm::mutation(Chromosome* chromosome) {
	// mutate teamSize

	// mutate units (switch 2 random unit in units array)
	int firstIndex = rand() % units.size();
	int secondIndex = rand() % units.size();
	// swap
	std::iter_swap(&chromosome->unitList[firstIndex], &chromosome->unitList[secondIndex]);
}

void GeneticAlgorithm::createFirstGeneration() {
	for (int i = 0; i < sizeof(population) / sizeof(*population); i++) {

	}
}