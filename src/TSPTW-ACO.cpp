/*

      TTTTTT   SSSSS  PPPPP	  TTTTTT  W         W
        TT    SS      PP  PP	TT	   W       W
        TT     SSSS   PPPPP		TT      W W W W
        TT        SS  PP		TT		 W W W
        TT    SSSSS   PP		TT		  W W

######################################################
########## Iterative improvement algorithms for ######
########## the TSP problem with Time Windows #########
######################################################

      Version: 1.0
      File:    TSPTW-ACO.cpp
      Author:  Jacopo De Stefani
      Purpose: Command line parameter parser and algorithm launcher
      Check:   README and gpl.txt
*/

/***************************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    email: jacopo.de.stefani@ulb.ac.be

***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include "InstanceReader.h"
#include "HeuristicCore.h"
#include "CommonDefs.h"



#define DEFAULT_INIT_FUNCTION RANDOM;
#define DEFAULT_NEIGHBORHOOD_TYPE INSERT;
#define DEFAULT_SOLUTION_UPDATE FIRST_IMPROVEMENT;
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
	char* seedsFileName=NULL;
	ENeighborhoodType neighborhoodType=DEFAULT_NEIGHBORHOOD_TYPE;
	EInitFunction initFunction=DEFAULT_INIT_FUNCTION;
	ESolutionUpdate solutionUpdate=DEFAULT_SOLUTION_UPDATE;
	unsigned int runs=DEFAULT_RUNS;
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

			c = getopt_long (argc, argv, "fbteni:dhr:s:k:",
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
					initFunction = RANDOM;
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
					initFunction = HEURISTIC;
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
					solutionUpdate = FIRST_IMPROVEMENT;
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
					solutionUpdate = BEST_IMPROVEMENT;
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
					neighborhoodType = TRANSPOSE;
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
					neighborhoodType = EXCHANGE;
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
					neighborhoodType = INSERT;
					setNeighborhood = true;
				}
				else{
					std::cerr << "[Error] - Multiple neighborhood types chosen." << std::endl << std::endl;
					usage();
					exit(-3);
				}
				break;

			case 'i':
				if(optarg != NULL){
					inputFileName = optarg;
				}
				else{
					std::cerr << "[Error] - Missing input file." << std::endl << std::endl;
					usage();
					exit(0);
				}
				break;

			case 'r':
				if(optarg != NULL){
					runs = atoi(optarg);
				}
				break;

			case 's':
				if(optarg != NULL){
					seedsFileName = optarg;
				}
				else{
					std::cerr << "[Error] - Missing seeds file." << std::endl << std::endl;
					usage();
					exit(0);
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

	if(seedsFileName == NULL){
			std::cerr << "[Error] - Missing seeds file." << std::endl << std::endl;
			usage();
			exit(0);
		}

	std::cout << "Solver parameters:" << std::endl;

	std::cout << "\tInstance: " << inputFileName << std::endl;

	std::cout << "\tInitialization function: ";
	switch (initFunction) {
	case RANDOM:
		std::cout << "Random" << std::endl;
		break;
	case HEURISTIC:
		std::cout << "Heuristic" << std::endl;
		break;
	default:
		std::cout << " " << std::endl;
		break;
	}

	std::cout << "\tNeighborhood: ";
	switch (neighborhoodType) {
	case EXCHANGE:
		std::cout << "Exchange" << std::endl;
		break;
	case TRANSPOSE:
		std::cout << "Transpose" << std::endl;
		break;
	case INSERT:
		std::cout << "Insert" << std::endl;
		break;
	default:
		std::cout << "" << std::endl;
		break;
	}

	std::cout << "\tPivoting rule: ";
	switch (solutionUpdate) {
	case BEST_IMPROVEMENT:
		std::cout << "Best improvement" << std::endl;
		break;
	case FIRST_IMPROVEMENT:
		std::cout << "First improvement" << std::endl;
		break;
	default:
		std::cout << "" << std::endl;
		break;
	}

	std::cout << "\tSeeds file: " << seedsFileName << std::endl;
	std::cout << "\tRuns: " << runs << std::endl;
	std::cout << "\tBest known solution: " << bestKnownSolution << std::endl;


	InstanceReader instanceReader(inputFileName,seedsFileName);
	if(instanceReader.OpenFiles()){
		if(instanceReader.ReadInformations()){
			//instanceReader.PrintDistanceMatrix();
			//instanceReader.PrintTimeWindows();
			if(instanceReader.ReadSeeds()){
				//instanceReader.PrintSeeds();
				if(instanceReader.GetSeeds().size() < runs){
					std::cerr << "[Error] - The seeds file contains lees seeds than the required number of runs " << std::endl << std::endl;
					exit(-1);
				}
			}
			else{
				std::cerr << "[Error] - Read error on " << seedsFileName << std::endl << std::endl;
				exit(-1);
			}
			HeuristicCore solverCore(instanceReader.GetPDistanceMatrix(),
										 instanceReader.GetTimeWindows(),
										 instanceReader.GetCities(),
										 initFunction,
										 neighborhoodType,
										 solutionUpdate,
										 instanceReader.GetSeeds(),
										 runs,
										 inputFileName,
										 bestKnownSolution);
			solverCore.RunII();
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
  std::cout << "Usage: TSPTW-ACO [PARAMETERS] -i [INPUTFILE] -s [SEEDSFILE]" << std::endl << std::endl;
  std::cout << "Flag \t\t Argument \t Description" << std::endl;
  std::cout << "-a,--alpha \t [Opt,Alpha] \t\t Influence of pheromone trails." << std::endl;
  std::cout << "-b,--beta\t [Opt,Beta] \t\t Influence of heuristic information." << std::endl;
  std::cout << "-h,--rho \t [Opt,Alpha] \t\t Pheromone trail evaporation." << std::endl;
  std::cout << "-n,--ants \t [Opt,Alpha] \t\t Number of ants." << std::endl;
  std::cout << "-i,--input\t [Req,Path] \t Path to instance to be given as input to the program" << std::endl;
  std::cout << "-r,--runs\t [Opt,Runs] \t Number of runs of the algorithm. 1 if omitted." << std::endl;
  std::cout << "-s,--seed\t [Req,Seed] \t Path to the file containing the list of seeds." << std::endl;
  std::cout << "-k,--known-best\t [Opt,Best] \t Best known solution for the analyzed instance. INT_MAX if omitted." << std::endl;
  version();
}

-r, --tries          # number of independent trials
-s, --tours          # number of steps in each trial
-t, --time           # maximum time for each trial
    --seed           # seed for the random number generator
-i, --tsplibfile     f inputfile (TSPLIB format necessary)
-o, --optimum        # stop if tour better or equal optimum is found
-m, --ants           # number of ants
-g, --nnants         # nearest neighbours in tour construction
-a, --alpha          # alpha (influence of pheromone trails)
-b, --beta           # beta (influence of heuristic information)
-e, --rho            # rho: pheromone trail evaporation
-q, --q0             # q_0: prob. of best choice in tour construction
-c, --elitistants    # number of elitist ants
-f, --rasranks       # number of ranks in rank-based Ant System
-k, --nnls           # No. of nearest neighbors for local search
-l, --localsearch    0: no local search   1: 2-opt   2: 2.5-opt   3: 3-opt
-d, --dlb            1 use don't look bits in local search
-u, --as               apply basic Ant System
-v, --eas              apply elitist Ant System
-w, --ras              apply rank-based version of Ant System
-x, --mmas             apply MAX-MIN ant system
-y, --bwas             apply best-worst ant system
-z, --acs              apply ant colony system
-h, --help             display the help text and exit


