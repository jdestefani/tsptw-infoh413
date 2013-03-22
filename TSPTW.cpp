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

/* Flag set by ‘--verbose’. */
static int verbose_flag;

/*Check flags & Make file*/
int main (int argc, char **argv)
{
	int c;
	char* inputFileName;
	HeuristicCore::ENeighborhoodType neighborhoodType;
	HeuristicCore::EInitFunction initFunction;
	HeuristicCore::ESolutionUpdate solutionUpdate;
	unsigned int runs=1;
	double seed=0.0f;
	struct timespec *currTime;
	unsigned int bestKnownSolution = INT_MAX;

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
			std::cout << "The solver will use first-improvement solution update" << std::endl;
			solutionUpdate = HeuristicCore::FIRST_IMPROVEMENT;
			break;

		case 'b':
			std::cout << "The solver will use best-improvement solution update" << std::endl;
			solutionUpdate = HeuristicCore::BEST_IMPROVEMENT;
			break;

		case 't':
			std::cout << "The exploring strategy will be transpose" << std::endl;
			neighborhoodType = HeuristicCore::TRANSPOSE;
			break;

		case 'e':
			std::cout << "The exploring strategy will be exchange" << std::endl;
			neighborhoodType = HeuristicCore::EXCHANGE;
			break;

		case 'n':
			std::cout << "The exploring strategy will be insert" << std::endl;
			neighborhoodType = HeuristicCore::INSERT;
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

		case 's':
			if(optarg == NULL){
				std::cout << "No best solution submitted" << std::endl;
				std::cout << "Omitting penalized relative percentage deviation computation" << std::endl;
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

	HeuristicCore simulationCore(instanceReader.GetVecDistanceMatrix(),
								 instanceReader.GetVecTimeWindows(),
								 initFunction,
								 neighborhoodType,
								 solutionUpdate,
								 seed);


	return EXIT_SUCCESS;
	exit (0);

}


