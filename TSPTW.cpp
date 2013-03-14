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

#include "InstanceReader.h"

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

	while (1)
	{
		static struct option long_options[] =
		{
				/* These options set a flag. */
				{"verbose", no_argument,       &verbose_flag, 1},
				{"brief",   no_argument,       &verbose_flag, 0},
				/* These options don't set a flag.
                  We distinguish them by their indices. */
				{"first",     no_argument,       0, 'a'},
				{"best",  no_argument,       0, 'b'},
				{"transpose",     no_argument,       0, 'c'},
				{"exchange",  no_argument,       0, 'd'},
				{"insert",  no_argument,       0, 'e'},
				{"file",    required_argument, 0, 'f'},
				{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "ab:cde:f:",
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
			std::cout << "The solver will run first-improvement heuristic" << std::endl;
			break;

		case 'b':
			std::cout << "The solver will run best-improvement heuristic" << std::endl;
			break;

		case 'c':
			std::cout << "The exploring strategy will be transpose" << std::endl;
			break;

		case 'd':
			std::cout << "The exploring strategy will be exchange" << std::endl;
			break;

		case 'e':
			std::cout << "The exploring strategy will be insert" << std::endl;
			break;

		case 'f':
			std::cout << "The instance will be read from " << optarg << std::endl;
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

       puts("Hello World!!!");
       return EXIT_SUCCESS;
       exit (0);

}


