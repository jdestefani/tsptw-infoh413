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
      File:    TSPTW-II.cpp
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


#define DEFAULT_NEIGHBORHOOD_CHAIN TRANSPOSE_EXCHANGE_INSERT;
#define DEFAULT_VND_TYPE STANDARD_VND;
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
	ENeighborhoodChain neighborhoodChain = DEFAULT_NEIGHBORHOOD_CHAIN;
	EVNDType vndType=DEFAULT_VND_TYPE;
	unsigned int runs=DEFAULT_RUNS;
	unsigned int bestKnownSolution = DEFAULT_BEST_KNOWN_SOLUTION;
	bool setVNDType=false;
	bool setPivotingRule=false;
	bool setNeighborhoodChain=false;



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
					{"standard",     no_argument,       0, 't'},
					{"piped",  no_argument,       0, 'p'},
					{"TEI",     no_argument,       0, 'a'},
					{"TIE",  no_argument,       0, 'b'},
					{"input",    required_argument, 0, 'i'},
					{"runs",  optional_argument,       0, 'r'},
					{"seed",  optional_argument,       0, 's'},
					{"known-best",  optional_argument,       0, 'k'},
					{0, 0, 0, 0}
			};
			/* getopt_long stores the option index here. */
			int option_index = 0;

			c = getopt_long (argc, argv, "tpabi:r:s:k:",
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

			case 't':
				if(!setVNDType){
					vndType = STANDARD_VND;
					setVNDType=true;
				}
				else{
					std::cerr << "[Error] - Multiple VND types chosen." << std::endl << std::endl;
					usage();
					exit(-4);
				}
				break;

			case 'p':
				if(!setVNDType){
					vndType = PIPED_VND;
					setVNDType = true;
				}
				else{
					std::cerr << "[Error] - Multiple VND types chosen." << std::endl << std::endl;
					usage();
					exit(-4);
				}
				break;


			case 'a':
				if(!setNeighborhoodChain){
					neighborhoodChain = TRANSPOSE_EXCHANGE_INSERT;
					setNeighborhoodChain = true;
				}
				else{
					std::cerr << "[Error] - Multiple neighborhood chains chosen." << std::endl << std::endl;
					usage();
					exit(-3);
				}
				break;

			case 'b':
				if(!setNeighborhoodChain){
					neighborhoodChain = TRANSPOSE_INSERT_EXCHANGE;
					setNeighborhoodChain = true;
				}
				else{
					std::cerr << "[Error] - Multiple neighborhood chains chosen." << std::endl << std::endl;
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
	std::cout << "\tInitialization function: Random" << std::endl;
	std::cout << "\tNeighborhood: Transpose" << std::endl;
	std::cout << "\tPivoting rule: First improvement" << std::endl;
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
			}
			HeuristicCore solverCore(instanceReader.GetDistanceMatrix(),
										 instanceReader.GetTimeWindows(),
										 instanceReader.GetCities(),
										 vndType,
										 neighborhoodChain,
										 instanceReader.GetSeeds(),
										 runs,
										 inputFileName,
										 bestKnownSolution);
			solverCore.RunVND();
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
  std::cout << "Usage: TSPTW-II [OPTIONS]..." << std::endl << std::endl;
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
  std::cout << "-s,--seed\t [Req,Seed] \t Path to the file containing the list of seeds." << std::endl;
  std::cout << "-k,--known-best\t [Opt,Best] \t Best known solution for the analyzed instance. INT_MAX if omitted." << std::endl;
  version();
}


