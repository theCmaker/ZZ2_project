// Compute the best approximate convex hull of a set of 2D points
// An approximate convex hull covers a subset of points
// It is evaluated on the number of points it covers and its area
// Thus, we have to get a Pareto front of the trade-offs
// C++11, started by Christophe Duhamel (2016-02-14)


#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>


// BRKGA inclusion
// borrowed from https://github.com/rfrancotoso/brkgaAPI
#include "BRKGA.h"  // BRKGA core functions
#include "MTRand.h" // Mersenne Twister (C version) TODO: replace with C++11 Mersenne Twister
#include "decoder.h"



// solutions stored wrt. total number of points covered and hull area

// main function
int main (int, char * argv[])
{
  Data data(argv[1]);

  Decoder decoder(data);
  std::cout << data << std::endl;

  const long unsigned seed = 0;//time(0);       // set the seed of the RNG
  MTRand rng(seed);                         // set the RNG
  const unsigned n    = data.points.size(); // size of chromosomes
  const unsigned p    = 256;		        // size of population
  const double pe     = 0.1;		        // fraction of population to be the elite-set
  const double pm     = 0.1;		        // fraction of population to be replaced by mutants
  const double rhoe   = 0.7;	            // probability that offspring inherit an allele from elite parent
  const unsigned K    = 3;		            // number of independent populations
  const unsigned MAXT = 2;	                // number of threads for parallel decoding

  BRKGA<Decoder,MTRand> algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT); // set the BRKGA

  // BRKGA inner loop (evolution) configuration: exchange top individuals
  const unsigned X_INTVL  =  100; // exchange best individuals at every 100 generations
  const unsigned X_NUMBER =    2; // exchange top 2 best
  const unsigned MAX_GENS = 100; // run for 1000 gens

  // BRKGA evolution configuration: restart strategy
  unsigned relevantGeneration =   1; // last relevant generation: best updated or reset called
  const unsigned RESET_AFTER  = 200;

  // create the record
  std::vector<double> bestChromosome;
  double bestFitness = std::numeric_limits< double >::max();

  // display info about multi-threading:
#ifdef _OPENMP
  std::cout << "running for " << MAX_GENS << " generations using " << MAXT
            << " out of " << omp_get_max_threads()
			<< " available thread units..." << std::endl;
#else
  std::cout << "running for " << MAX_GENS
			<< " generations without multi-threading..." << std::endl;
#endif

	// run the evolution loop
	const clock_t begin = clock();
	for (size_t generation = 1; generation < MAX_GENS; ++generation)
	{
		algorithm.evolve();	// evolve the population for one generation

		// bookeeping: has the incumbent been improved?
		if (algorithm.getBestFitness() < bestFitness)
		{
			// save it
			relevantGeneration = generation;
			bestFitness = algorithm.getBestFitness();
			bestChromosome = algorithm.getBestChromosome();
			
			std::cout << "\t" << generation << ") improved incumbent: " << bestFitness << std::endl;
		}

		//  evolution strategy: restart if stalled
		if (generation - relevantGeneration > RESET_AFTER)
		{
			algorithm.reset();	// restart the algorithm with random keys
			relevantGeneration = generation;
			
			std::cout << "\t" << generation << ") reset at generation " << generation << std::endl;
		}

		// evolution strategy: exchange top individuals among the populations
		if (generation % X_INTVL == 0 && relevantGeneration != generation)
		{
			algorithm.exchangeElite(X_NUMBER);
			
			std::cout << "\t" << generation << ") exchanged top individuals." << std::endl;
		}
	}
	const clock_t end = clock();

	// display the best result
	std::cout << "got hypervolume " << bestFitness << " in " << ((end - begin) / double(CLOCKS_PER_SEC)) << "s" << std::endl;

	// print the fitness of the top 10 individuals of each population
	std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
	const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
	for (unsigned i = 0; i < K; ++i)
	{
		std::cout << "Population #" << i << ":" << std::endl;
		for (unsigned j = 0; j < bound; ++j)
		{
			std::cout << "\t" << j << ") " << algorithm.getPopulation(i).getFitness(j) << std::endl;
		}
	}

	// rebuild the best solution
	decoder.decode(bestChromosome);
	std::cout << decoder.getFront() << std::endl;
  
  return 0;
}