#include "ClinicSimulator.h"
#include <cstdlib>
#include <vector>

using std::strtod;

int main(int argc, char* argv[]) {
	std::vector<double> stay_probabilities;
	for (int i = 5; i < argc; i++) stay_probabilities.push_back(strtod(argv[i], nullptr));
	ClinicSimulator* sim = sim->getInstance(strtod(argv[1], nullptr), strtol(argv[2], nullptr, 10), 
		strtod(argv[3], nullptr), strtod(argv[4], nullptr), stay_probabilities);
	sim->run();
	sim->printResults();
}
