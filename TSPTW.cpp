//============================================================================
// Name        : INFO-H-413.cpp
// Author      : Jacopo De Stefani
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include "InstanceReader.h"
#include "HeuristicCore.h"

#define FIRST_IMPROVEMENT_FLAG "first";
#define BEST_IMPROVEMENT_FLAG "best";
#define TRANSPOSE_FLAG "transpose";
#define EXCHANGE_FLAG "exchange";
#define INSERT_FLAG "insert";

#define DEFAULT_INIT_FUNCTION HeuristicCore::RANDOM;
#define DEFAULT_NEIGHBORHOOD_TYPE HeuristicCore::INSERT;
#define DEFAULT_SOLUTION_UPDATE HeuristicCore::BEST_IMPROVEMENT;
#define DEFAULT_RUNS 1;
#define DEFAULT_SEED 0.0f;
#define DEFAULT_BEST_KNOWN_SOLUTION INT_MAX;

#define ERROR_CODE_PIVOTING_RULE -2;
#define ERROR_CODE_NEIGHBORHOOD -3;

void version(void);
void usage(void);
/* Flag set by ‘--verbose’. */
static int verbose_flag;

/*Check flags & Make file*/
int main (int argc, char **argv)
{

	int c;
	char* inputFileName=NULL;
	HeuristicCore::ENeighborhoodType neighborhoodType=DEFAULT_NEIGHBORHOOD_TYPE;
	HeuristicCore::EInitFunction initFunction=DEFAULT_INIT_FUNCTION;
	HeuristicCore::ESolutionUpdate solutionUpdate=DEFAULT_SOLUTION_UPDATE;
	unsigned int runs=DEFAULT_RUNS;
	double seed=DEFAULT_SEED;
	struct timespec *currTime;
	unsigned int bestKnownSolution = DEFAULT_BEST_KNOWN_SOLUTION;
	bool setInitFunction=false;
	bool setPivotingRule=false;
	bool setNeighborhood=false;

	std::cout << std::endl;
	if(argc > 1){
		while (true)
		{
			static struct option long_options[] =
			{
					/* These options set a flag. */
					{"verbose", no_argument,       &verbose_flag, 1},
					{"brief",   no_argument,       &verbose_flag, 0},
					/* These options don't set a flag.
                  We distinguish them by their indices. */
					{"first-imp",     no_argument,       0, 'f'},
					{"best-imp",  no_argument,       0, 'b'},
					{"transpose",     no_argument,       0, 't'},
					{"exchange",  no_argument,       0, 'e'},
					{"insert",  no_argument,       0, 'n'},
					{"input",    required_argument, 0, 'i'},
					{"random",     no_argument,       0, 'd'},
					{"heuristic",  no_argument,       0, 'h'},
					{"runs",  optional_argument,       0, 'r'},
					{"seed",  optional_argument,       0, 's'},
					{"known-best",  optional_argument,       0, 'k'},
					{0, 0, 0, 0}
			};
			/* getopt_long stores the option index here. */
			int option_index = 0;

			c = getopt_long (argc, argv, "fbteni:dhrsk",
					long_options, &option_index);

			/* Detect the end of the options. */
			if (c == -1)
				break;

			switch (c)
			{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;

			case 'd':
				if(!setInitFunction){
					initFunction = HeuristicCore::RANDOM;
					setInitFunction=true;
				}
				else{
					std::cerr << "[Error] - Multiple initialization functions chosen." << std::endl << std::endl;
					usage();
					exit(-4);
				}
				break;

			case 'h':
				if(!setInitFunction){
					initFunction = HeuristicCore::HEURISTIC;
					setInitFunction = true;
				}
				else{
					std::cerr << "[Error] - Multiple initialization functions chosen." << std::endl << std::endl;
					usage();
					exit(-4);
				}
				break;


			case 'f':
				if(!setPivotingRule){
					solutionUpdate = HeuristicCore::FIRST_IMPROVEMENT;
					setPivotingRule=true;
				}
				else{
					std::cerr << "[Error] - Multiple pivoting rules chosen." << std::endl << std::endl;
					usage();
					exit(-2);
				}
				break;

			case 'b':
				if(!setPivotingRule){
					solutionUpdate = HeuristicCore::BEST_IMPROVEMENT;
					setPivotingRule=true;
				}
				else{
					std::cerr << "[Error] - Multiple pivoting rules chosen." << std::endl << std::endl;
					usage();
					exit(-2);
				}
				break;

			case 't':
				if(!setNeighborhood){
					neighborhoodType = HeuristicCore::TRANSPOSE;
					setNeighborhood = true;
				}
				else{
					std::cerr << "[Error] - Multiple neighborhood types chosen." << std::endl << std::endl;
					usage();
					exit(-3);
				}
				break;

			case 'e':
				if(!setNeighborhood){
					std::cout << "\tNeighborhood: Exchange" << std::endl;
					neighborhoodType = HeuristicCore::EXCHANGE;
					setNeighborhood = true;
				}
				else{
					std::cerr << "[Error] - Multiple neighborhood types chosen." << std::endl << std::endl;
					usage();
					exit(-3);
				}
				break;

			case 'n':
				if(!setNeighborhood){
					neighborhoodType = HeuristicCore::INSERT;
					setNeighborhood = true;
				}
				else{
					std::cerr << "[Error] - Multiple neighborhood types chosen." << std::endl << std::endl;
					usage();
					exit(-3);
				}
				break;

			case 'i':
				//if(optarg != NULL){
					inputFileName = optarg;
				//}
				//else{

					//std::cerr << "[Error] - Missing input file." << std::endl << std::endl;
					//usage();
					//exit(0);
				//}
				break;

			case 'r':
				if(optarg != NULL){
					runs = atoi(optarg);
				}
				break;

			case 's':
				if(optarg == NULL){
					clock_gettime(CLOCK_MONOTONIC,currTime);
					seed = currTime->tv_sec;
				}
				else{
					seed = atoi(optarg);
				}

				break;

			case 'k':
				if(optarg == NULL){
					//std::cout << "No best solution submitted" << std::endl;
					//std::cout << "Omitting penalized relative percentage deviation computation" << std::endl;
					bestKnownSolution = INT_MAX;
				}
				else{
					bestKnownSolution = atoi(optarg);
				}
				break;

			case '?':
				/* getopt_long already printed an [Error] -  message. */
				break;

			default:
				abort ();
			}
		}
	}
	else{
		std::cerr << "[Error] - No arguments to the program." << std::endl << std::endl;
		usage();
		exit(0);

	}
	/* Instead of reporting ‘--verbose’
          and ‘--brief’ as they are encountered,
          we report the final status resulting from them. */
	if (verbose_flag)
		puts ("verbose flag is set");

	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		std::cout << "[Error] - Unknown elements: ";
		while (optind < argc){
			std::cout << argv[optind++] << " ";
		}
		std::cout << std::endl;
	}


	if(inputFileName == NULL){
		std::cerr << "[Error] - Missing input file." << std::endl << std::endl;
		usage();
		exit(0);
	}

	std::cout << "Solver parameters:" << std::endl;

	std::cout << "\tInstance: " << inputFileName << std::endl;

	std::cout << "\tInitialization function: ";
	switch (initFunction) {
	case HeuristicCore::RANDOM:
		std::cout << "Random" << std::endl;
		break;
	case HeuristicCore::HEURISTIC:
		std::cout << "Heuristic" << std::endl;
		break;
	default:
		std::cout << " " << std::endl;
		break;
	}

	std::cout << "\tNeighborhood: ";
	switch (neighborhoodType) {
	case HeuristicCore::EXCHANGE:
		std::cout << "Exchange" << std::endl;
		break;
	case HeuristicCore::TRANSPOSE:
		std::cout << "Transpose" << std::endl;
		break;
	case HeuristicCore::INSERT:
		std::cout << "Insert" << std::endl;
		break;
	default:
		std::cout << "" << std::endl;
		break;
	}

	std::cout << "\tPivoting rule: ";
	switch (solutionUpdate) {
	case HeuristicCore::BEST_IMPROVEMENT:
		std::cout << "Best improvement" << std::endl;
		break;
	case HeuristicCore::FIRST_IMPROVEMENT:
		std::cout << "First improvement" << std::endl;
		break;
	default:
		std::cout << "" << std::endl;
		break;
	}

	std::cout << "\tSeed: " << seed << std::endl;
	std::cout << "\tRuns: " << runs << std::endl;
	std::cout << "\tBest known solution: " << bestKnownSolution << std::endl << std::endl;


	InstanceReader instanceReader(inputFileName);
	if(instanceReader.OpenFile()){
		if(instanceReader.ReadInformations()){
			instanceReader.PrintDistanceMatrix();
			instanceReader.PrintTimeWindows();
			/*HeuristicCore simulationCore(instanceReader.GetVecDistanceMatrix(),
										 instanceReader.GetVecTimeWindows(),
										 instanceReader.GetCities(),
										 initFunction,
										 neighborhoodType,
										 solutionUpdate,
										 seed,
										 runs,
										 inputFileString,
										 bestKnownSolution);*/
			return EXIT_SUCCESS;
			exit(0);
		}
		else{
			std::cerr << "[Error] - Read error on " << inputFileName << std::endl << std::endl;
			exit(-1);
		}
	}
	else{
		std::cerr << "[Error] - Could not open " << inputFileName << std::endl << std::endl;
		exit(-1);
	}





	return EXIT_SUCCESS;
	exit (0);

}

void version(void)
{
	std::cout << std::endl << std::endl;
	std::cout << "Jacopo De Stefani (jacopo.de.stefani@ulb.ac.be)" << std::endl;
	std::cout << "This is free software, and you are welcome to redistribute it under certain" << std::endl;
	std::cout << "conditions.  See the GNU General Public License for details. There is NO   " << std::endl;
	std::cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
	std::cout << std::endl;
}

void usage(void)
{
  std::cout << "Usage: TSPTW [OPTIONS]..." << std::endl << std::endl;
  std::cout << "Flag \t\t Argument \t Description" << std::endl;
  std::cout << "-d,--random \t [No] \t\t Generate random initial solution." << std::endl;
  std::cout << "-h,--heuristic\t [No] \t\t Generate initial solution using heuristic." << std::endl;
  std::cout << "-f,--first-imp \t [No] \t\t Use first improvement pivoting rule." << std::endl;
  std::cout << "-b,--best-imp \t [No] \t\t Use best improvement pivoting rule." << std::endl;
  std::cout << "-t,--transpose \t [No] \t\t Use transpose neighborhood." << std::endl;
  std::cout << "-e,--exchange \t [No] \t\t Use exchange neighborhood." << std::endl;
  std::cout << "-n,--insert \t [No] \t\t Use insert neighborhood." << std::endl;
  std::cout << "-i,--input\t [Req,Path] \t Path to instance to be given as input to the program" << std::endl;
  std::cout << "-r,--runs\t [Opt,Runs] \t Number of runs of the algorithm. 1 if omitted." << std::endl;
  std::cout << "-s,--seed\t [Opt,Seed] \t Seed for the random number generator of the algorithm. System time if omitted." << std::endl;
  std::cout << "-k,--known-best\t [Opt,Best] \t Best known solution for the analyzed instance. INT_MAX if omitted." << std::endl;
  version();
}


