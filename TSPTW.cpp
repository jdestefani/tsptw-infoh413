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

#define DEFAULT_INIT_FUNCTION 0;
#define DEFAULT_NEIGHBORHOOD_TYPE 0;
#define DEFAULT_SOLUTION_UPDATE 0;
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
	char* inputFileName;
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

	std::cout << "Solver parameters:" << std::endl;
	while (1)
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
				{"runs",  required_argument,       0, 'r'},
				{"seed",  optional_argument,       0, 's'},
				{"known-best",  optional_argument,       0, 'k'},
				{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "ab:cde:f:gh:i",
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
				std::cout << "\tInitialization function: Random" << std::endl;
				initFunction = HeuristicCore::RANDOM;
				setInitFunction=true;
			}
			else{
				std::cerr << "Error: Multiple initialization function chosen." << std::endl;
				exit(0);
			}
			break;

		case 'h':
			if(!setInitFunction){
				std::cout << "The initialization function will be heuristic" << std::endl;
				initFunction = HeuristicCore::HEURISTIC;
				setInitFunction = true;
			}
			else{
				std::cerr << "Error: Multiple initialization function chosen." << std::endl;
				exit(0);
			}
			break;


		case 'f':
			if(!setPivotingRule){
				std::cout << "\tPivoting rule: First Improvement" << std::endl;
				solutionUpdate = HeuristicCore::FIRST_IMPROVEMENT;
				setPivotingRule=true;
			}
			else{
				std::cerr << "Error: Multiple pivoting rule chosen." << std::endl;
				exit(0);
			}
			break;

		case 'b':
			if(!setPivotingRule){
				std::cout << "\tPivoting rule: Best Improvement" << std::endl;
				solutionUpdate = HeuristicCore::BEST_IMPROVEMENT;
				setPivotingRule=true;
			}
			else{
				std::cerr << "Error: Multiple pivoting rule chosen." << std::endl;
				exit(0);
			}
			break;

		case 't':
			if(!setNeighborhood){
				std::cout << "\tNeighborhood: Transpose" << std::endl;
				neighborhoodType = HeuristicCore::TRANSPOSE;
				setNeighborhood = true;
			}
			else{
				std::cerr << "Error: Multiple neighborhood type chosen." << std::endl;
				exit(0);
			}
			break;

		case 'e':
			if(!setNeighborhood){
				std::cout << "\tNeighborhood: Exchange" << std::endl;
				neighborhoodType = HeuristicCore::EXCHANGE;
				setNeighborhood = true;
			}
			else{
				std::cerr << "Error: Multiple neighborhood type chosen." << std::endl;
				exit(0);
			}
			break;

		case 'n':
			if(!setNeighborhood){
				std::cout << "\tNeighborhood: Insert" << std::endl;
				neighborhoodType = HeuristicCore::INSERT;
				setNeighborhood = true;
			}
			else{
				std::cerr << "Error: Multiple neighborhood type chosen." << std::endl;
				exit(0);
			}
			break;

		case 'i':
			std::cout << "The instance will be read from " << optarg << std::endl;
			inputFileName = optarg;
			std::cout << "Instance: " << inputFileName << std::endl;
			break;

		case 'r':
			if(optarg != NULL){
				std::cout << "The experiment will be executed " << optarg << "times" << std::endl;
				runs = atoi(optarg);
			}
			std::cout << "Runs: " << runs << std::endl;
			break;

		case 's':
			if(optarg == NULL){
				clock_gettime(CLOCK_MONOTONIC,currTime);
				seed = currTime->tv_sec;
			}
			else{
				seed = atoi(optarg);
			}
			std::cout << "Seed: " << seed << std::endl;
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
			std::cout << "Best known solution: " << bestKnownSolution << std::endl;
			break;

		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			abort ();
		}
	}

	/* Instead of reporting ‘--verbose’
          and ‘--brief’ as they are encountered,
          we report the final status resulting from them. */
	if (verbose_flag)
		puts ("verbose flag is set");

	/* Print any remaining command line arguments (not options). */
	/*if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
	}*/

	if (optind < argc)
	{
		usage();
		exit(0);
	}

	std::string inputFileString(inputFileName);
	InstanceReader instanceReader(inputFileString);
	if(instanceReader.OpenFile()){
		instanceReader.ReadInformations();
	}
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
								 bestKnownSolution);
*/

	return EXIT_SUCCESS;
	exit (0);

}

void version(void)
{
	std::cout << "\n\n";
	std::cout << "Jacopo De Stefani (jacopo.de.stefani@ulb.ac.be)\n";
	std::cout << "This is free software, and you are welcome to redistribute it under certain\n";
	std::cout << "conditions.  See the GNU General Public License for details. There is NO   \n";
	std::cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n";
	std::cout << "\n";
}

void usage(void)
{
  std::cout << "\nUsage: TSPTW [OPTIONS]...\n\n";
  std::cout << "Flag \t Argument \t Description";
  std::cout << "-d,--random \t [No] Generate random initial solution.\n";
  std::cout << "-h,--heuristic\t [No] Generate initial solution using heuristic.\n";
  std::cout << "-f,--first-imp \t [No] Use first improvement pivoting rule.\n";
  std::cout << "-b,--best-imp \t [No] Use best improvement pivoting rule.\n";
  std::cout << "-t,--transpose \t [No] Use transpose neighborhood.\n";
  std::cout << "-e,--exchange \t [No] Use exchange neighborhood.\n";
  std::cout << "-n,--insert \t [No] Use insert neighborhood.\n";
  std::cout << "-i,--input\t [Req,Path] Path to instance to be given as input to the program\n";
  std::cout << "-r,--runs\t [Opt,Runs] Number of runs of the algorithm. 1 if omitted.\n";
  std::cout << "-s,--seed\t [Opt,Seed] Seed for the random number generator of the algorithm. System time if omitted.\n";
  std::cout << "-k,--known-best\t [Opt,Best] Best known solution for the analyzed instance. INT_MAX if omitted.\n";
  version();
}


