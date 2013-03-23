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
			break;

		case 'd':
			std::cout << "The initialization function will be random" << std::endl;
			initFunction = HeuristicCore::RANDOM;
			break;

		case 'h':
			std::cout << "The initialization function will be heuristic" << std::endl;
			initFunction = HeuristicCore::HEURISTIC;
			break;

		case 'r':
			std::cout << "The experiment will be executed " << optarg << "times" << std::endl;
			runs = atoi(optarg);
			break;

		case 's':
			if(optarg == NULL){
				clock_gettime(CLOCK_MONOTONIC,currTime);
				seed = currTime->tv_sec;
			}
			else{
				seed = atoi(optarg);
			}
			std::cout << "The experiment seed will be " << seed << std::endl;
			break;

		case 'k':
			if(optarg == NULL){
				std::cout << "No best solution submitted" << std::endl;
				std::cout << "Omitting penalized relative percentage deviation computation" << std::endl;
				bestKnownSolution = INT_MAX;
			}
			else{
				bestKnownSolution = atoi(optarg);
			}
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
	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
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

static void version(void)
{
  printf ("%s", program_invocation_short_name);
  printf("\n\n"
"Copyright (C) 2009\n"
"Manuel Lopez-Ibanez (manuel.lopez-ibanez@ulb.ac.be) and\n"
"Christian Blum (cblum@lsi.upc.edu)\n"
"\n"
"This is free software, and you are welcome to redistribute it under certain\n"
"conditions.  See the GNU General Public License for details. There is NO   \n"
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
"\n"        );
}

static void usage(void)
{
  printf("\n"
         "Usage: %s INSTANCE_FILE SOLUTION_FILE\n\n", program_invocation_short_name);

    printf(
"Reads an instance file and a file with a permutation (from 1 to N, that is, not containing the depot) and evaluates the solution.\n"
"\n");
    version ();
}

