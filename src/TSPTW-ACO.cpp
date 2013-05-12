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
#include <limits.h>

#include "InstanceReader.h"
#include "ACOCore.h"
#include "CommonDefs.h"


/*Cfr. Stutzle, T., & Hoos, H. H. (2000). MAX-MIN ant system. Future generations computer systems, 16(8), 889-914.*/
#define DEFAULT_ALPHA 1.0f;
#define DEFAULT_BETA 2.0f;
#define DEFAULT_RHO 0.8f;
#define DEFAULT_TAU_ZERO 1.0f;
#define DEFAULT_EPSILON 0.5f;
#define DEFAULT_ANTS 25;
#define DEFAULT_RUNS 1;
#define DEFAULT_MAX_RUNTIME 10; //seconds
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
	double alpha=DEFAULT_ALPHA;
	double beta=DEFAULT_BETA;
	double rho=DEFAULT_RHO;
	double tau_zero=DEFAULT_TAU_ZERO;
	double epsilon=DEFAULT_EPSILON;
	unsigned int ants=DEFAULT_ANTS;
	unsigned int runs=DEFAULT_RUNS;
	unsigned int bestKnownSolution = DEFAULT_BEST_KNOWN_SOLUTION;
	double max_runtime=DEFAULT_MAX_RUNTIME;

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
					{"alpha",   required_argument,       0, 'a'},
					{"beta",  	required_argument,       0, 'b'},
					{"rho",     required_argument,       0, 'h'},
					{"tau-zero",required_argument,       0, 'z'},
					{"epsilon", required_argument,       0, 'e'},
					{"t-max",  	required_argument,       0, 't'},
					{"ants",  	required_argument,       0, 'n'},
					{"input",   required_argument, 		 0, 'i'},
					{"runs",  	required_argument,       0, 'r'},
					{"seed",  	required_argument,       0, 's'},
					{"known-best",  required_argument,   0, 'k'},
					{0, 0, 0, 0}
			};
			/* getopt_long stores the option index here. */
			int option_index = 0;

			c = getopt_long (argc, argv, "a:b:h:z:e:t:n:i:r:s:k:",
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

			case 'a':
				if(optarg != NULL){
					alpha = atof(optarg);
				}
				else{
					std::cerr << "[Error] - Missing alpha value." << std::endl << std::endl;
					usage();
					exit(0);
				}
				break;

			case 'b':
				if(optarg != NULL){
					beta = atof(optarg);
				}
				else{
					std::cerr << "[Error] - Missing beta value." << std::endl << std::endl;
					usage();
					exit(0);
				}
				break;

			case 'h':
				if(optarg != NULL){
					rho = atof(optarg);
				}
				else{
					std::cerr << "[Error] - Missing rho value." << std::endl << std::endl;
					usage();
					exit(0);
				}
				break;

			case 'z':
				if(optarg != NULL){
					tau_zero = atof(optarg);
				}
				else{
					std::cerr << "[Error] - Missing tau_zero value." << std::endl << std::endl;
					usage();
					exit(0);
				}
				break;

			case 'e':
				if(optarg != NULL){
					epsilon = atof(optarg);
				}
				else{
					std::cerr << "[Error] - Missing epsilon value." << std::endl << std::endl;
					usage();
					exit(0);
				}
				break;

			case 't':
				if(optarg != NULL){
					max_runtime = atof(optarg);
				}
				else{
					std::cerr << "[Error] - Missing maximum run-time value." << std::endl << std::endl;
					usage();
					exit(0);
				}
				break;

			case 'n':
				if(optarg != NULL){
					ants = atoi(optarg);
				}
				else{
					std::cerr << "[Error] - Missing number of ants." << std::endl << std::endl;
					usage();
					exit(0);
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
	std::cout << "\tAlpha: " << alpha << std::endl;
	std::cout << "\tBeta: " << beta << std::endl;
	std::cout << "\tRho: " << rho << std::endl;
	std::cout << "\tTau_zero: " << tau_zero << std::endl;
	std::cout << "\tEpsilon: " << epsilon << std::endl;
	std::cout << "\tAnts: " << ants << std::endl;
	std::cout << "\tSeeds file: " << seedsFileName << std::endl;
	std::cout << "\tRuns: " << runs << std::endl;
	std::cout << "\tMaximum runtime: " << max_runtime << std::endl;
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
			ACOCore solverCore(
					instanceReader.GetPDistanceMatrix(),
					instanceReader.GetTimeWindows(),
					instanceReader.GetCities(),
					ants,
					alpha,
					beta,
					rho,
					tau_zero,
					epsilon,
					max_runtime,
					instanceReader.GetSeeds(),
					runs,
					inputFileName,
					bestKnownSolution);
			solverCore.Run();
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
  std::cout << "-a,--alpha \t [Opt,Alpha] \t Influence of pheromone trails." << std::endl;
  std::cout << "-b,--beta\t [Opt,Beta] \t Influence of heuristic information." << std::endl;
  std::cout << "-h,--rho \t [Opt,Rho] \t Pheromone trail evaporation." << std::endl;
  std::cout << "-z,--tau-zero \t [Opt,Tau_zero]  Pheromone trail initial value." << std::endl;
  std::cout << "-e,--epsilon \t [Opt,Epsilon] \t Probability of choosing the best global solution instead of the best iteration one for pheromone update." << std::endl;
  std::cout << "-t,--t-max \t [Opt,T-Max] \t Maximum runtime for each run" << std::endl;
  std::cout << "-n,--ants \t [Opt,Ants] \t Number of ants." << std::endl;
  std::cout << "-i,--input\t [Req,Path] \t Path to instance to be given as input to the program" << std::endl;
  std::cout << "-r,--runs\t [Opt,Runs] \t Number of runs of the algorithm. 1 if omitted." << std::endl;
  std::cout << "-s,--seed\t [Req,Seed] \t Path to the file containing the list of seeds." << std::endl;
  std::cout << "-k,--known-best\t [Opt,Best] \t Best known solution for the analyzed instance. INT_MAX if omitted." << std::endl;
  version();
}


