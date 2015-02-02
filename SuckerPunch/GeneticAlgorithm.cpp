#include "GeneticAlgorithm.hpp"

Chromosome::Chromosome() {}

Chromosome::~Chromosome() {}

GeneticAlgorithm::GeneticAlgorithm(int totalTeam, std::vector<Unit> units, int sizePopulation, int sizeGeneration) {
	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	this->totalTeam = totalTeam;
	this->units = units;

	this->sizePopulation = sizePopulation;
	this->sizeGeneration = sizeGeneration;

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

void GeneticAlgorithm::createFirstGeneration() {
	for (int i = 0; i < sizePopulation; i++) {
		Chromosome* chromosome = createOneChromosome();
		population.push_back(chromosome);
	}
	sortPopulation();
}

void GeneticAlgorithm::sortPopulation() {
	std::sort(population.begin(), population.end(), [](const Chromosome* lhs, const Chromosome* rhs) {
		return lhs->fitness < rhs->fitness;
	});
}

void GeneticAlgorithm::setTeamSize(Chromosome* chromosome) {
	// clear the vector
	chromosome->teamSize.clear();

	int totalUnits = units.size();
	for (int i = 0; i < totalTeam; i++) {
		// last index, assign the rest of the team
		if (i == totalTeam - 1) {
			chromosome->teamSize.push_back(totalUnits);
		}
		else {
			int unitsInOneTeam = (rand() % (totalUnits - (totalTeam - (i + 1)))) + 1;
			totalUnits -= unitsInOneTeam;

			chromosome->teamSize.push_back(unitsInOneTeam);
		}
	}
}

Chromosome* GeneticAlgorithm::copyChromosome(Chromosome* chromosome) {
	Chromosome* result = new Chromosome();

	// copy teamSize
	for (int i = 0; i < chromosome->teamSize.size(); i++)
		result->teamSize.push_back(chromosome->teamSize[i]);
	// copy unitList
	for (int i = 0; i < chromosome->unitList.size(); i++) {
		Unit unit;
		unit.atk = chromosome->unitList[i].atk;
		unit.def = chromosome->unitList[i].def;
		unit.index = chromosome->unitList[i].index;
		result->unitList.push_back(unit);
	}
	// copy fitness
	result->fitness = chromosome->fitness;
	
	return result;
}

Chromosome* GeneticAlgorithm::mutation(Chromosome* chromosome) {
	// copy chromosome
	Chromosome* result = copyChromosome(chromosome);

	// mutate teamSize (change team size or not, chance 50%)
	if (rand() % 2 == 0) setTeamSize(result);
	// mutate units (switch 2 random unit in units array)
	int firstIndex = rand() % result->unitList.size();
	int secondIndex = rand() % result->unitList.size();
	// swap
	std::iter_swap(&result->unitList[firstIndex], &result->unitList[secondIndex]);
	// update fitness
	calculateFitness(result);

	return result;
}

Chromosome* GeneticAlgorithm::createOneChromosome() {
	Chromosome* result = new Chromosome();
	
	// set total team (one team contains minimum 1 unit)
	setTeamSize(result);
	// set unitList
	std::vector<Unit> unitsCopy;
	for (int i = 0; i < units.size(); i++) {
		Unit unit;
		unit.atk = units[i].atk; 
		unit.def = units[i].def;
		unit.index = units[i].index;
		unitsCopy.push_back(unit);
	}
	for (int i = 0; i < units.size(); i++) {
		// choose random index
		int indexChosed = (rand() % unitsCopy.size());
		// create unit
		Unit unit;
		unit.atk = unitsCopy[indexChosed].atk;
		unit.def = unitsCopy[indexChosed].def;
		unit.index = unitsCopy[indexChosed].index;
		// assign to result
		result->unitList.push_back(unit);
		// delete from unitsCopy
		unitsCopy.erase(unitsCopy.begin() + indexChosed);
	}
	// calculate fitness point
	calculateFitness(result);

	return result;
}

std::vector<std::vector<int>> GeneticAlgorithm::findOptimumSolution() {
	// iterate generation
	for (int i = 0; i < sizeGeneration; i++) {
		// mutation and add the populationSize = populationSize * (populationSize / 2);
		for (int j = 0; j < floor(sizePopulation / 2); j++) {
			population.push_back(mutation(population[j]));
		}
		// sort the population
		sortPopulation();
		// delete bad chromosome
		while (population.size() > sizePopulation)
			population.pop_back();
	}

	// return the best chromosome
	return convertToOutputList(population[0]);
}

std::vector<std::vector<int>> GeneticAlgorithm::convertToOutputList(Chromosome* chromosome) {
	std::vector<std::vector<int>> result;
	
	int firstIndex = 0;
	int lastIndex = chromosome->teamSize[0];

	for (int i = 0; i < totalTeam; i++) {
		std::vector<int> listTeam;
		for (int j = firstIndex; j < lastIndex; j++) {
			listTeam.push_back(chromosome->unitList[j].index);
		}
		// sort 
		std::sort(listTeam.begin(), listTeam.end());
		// add to result
		result.push_back(listTeam);

		if (i < totalTeam - 1) {
			firstIndex = lastIndex;
			lastIndex += chromosome->teamSize[i + 1];
		}
	}

	return result;
}