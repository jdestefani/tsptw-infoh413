
      TTTTTT   SSSSS  PPPPP	  TTTTTT  W         W
        TT    SS      PP  PP	TT	   W       W
        TT     SSSS   PPPPP		TT      W W W W
        TT        SS  PP		TT		 W W W
        TT    SSSSS   PP		TT		  W W

######################################################
########## 		Algorithms framework for 	 #########
########## the TSP problem with Time Windows #########
######################################################

	  Version: 1.0
      File:    CandidateSolution.h
      Author:  Jacopo De Stefani


This is the README file to the software package TSP-TW.

The software package is freely available subject to the 
GNU General Public Licence, which is included in file gpl.txt.

If you use TSPTW in your research, I would appreciate a citation in
your publication(s). Please cite it as

Jacopo De Stefani. TSPTW, Version 1.0. Available from
https://code.google.com/p/tsptw-infoh413/, 2013.

This software package provides an implementation of various metaheuristics
to obtain a solution for for the symmetric Traveling Salesman Problem with Time Windows (TSPTW). 
The metaheuristic implemented are Iterative Improvement (-II) and Variable Neighborhood Descent (-VND).

AIMS OF THE SOFTWARE: This software is developed as an implementation 
exercise for the course INFO-H-413 - Heuristic Optimization.
The software tries to provide a reasonably efficient object-oriented 
implementation of these metaheuristics while at the same time aiming for 
extensibility, readability and understandability of the code.



=========
CONTENTS
=========

The source code is located in the src folder, whereas the binary
files, the instances and the launcher and postprocessing scripts
are located in the bin folder.

The GNU General Public Licence:
gpl.txt

The command line parameter parser and core launcher,
respectively for Iterative Improvement and Variable Neighborhood descent:
TSPTW-II.cpp
TSPTW-VND.cpp

User-defined data type to model the time window:
TimeWindow.h

User-defined data type to model the candidate solution:
CandidateSolution.h
CandidateSolution.cpp

Seed and Instance files reader:
InstanceReader.h
InstanceReader.cpp

Program core containing all the metaheuristics implementation:
HeuristicCore.h
HeuristicCore.cpp

Writer of the run results to the output file
Writer.h
Writer.cpp

Global data types:
CommonDefs.h

Makefile

R scripts to process data:
processDataII.r
processDataVND.r

Bash script to launch all the algorithm variants on a single instance
and process data:
launchTSPTW-II.sh
launchTSPTW-VND.sh


Instances: 
  n80w20.001.txt
  n80w20.002.txt
  n80w20.003.txt
  n80w20.004.txt
  n80w20.005.txt
  n80w200.001.txt
  n80w200.002.txt
  n80w200.003.txt
  n80w200.004.txt
  n80w200.005.txt


=====
Code
=====


The software was developed in C++98 under Linux (Debian Wheezy), 
using the GNU g++ (Debian 4.7.2-5) 4.7.2 compiler and tested in this environment. 
The software is distributed as a compressed tar file, containing both the source code (in the src folder) and the scripts and instances (in the bin and bin\instances respectively).

To install the program, first obtain the file TSPTW.V1.0.tar.gz. Unzip
the file by typing

gunzip TSPTW.V1.0.tar.gz

and then unpack it by typing 

tar -xvf TSPTW.V1.0.tar

The software will be extracted in a new folder TSPTW.V1.0

Finally by launching:

make all

the Makefile will trigger the compilation of the files, producing the executables 'TSPTW-II' and 'TSPTW-VND" in the bin folder.

Note: The code is written in C++98. Hence, the code should be
reasonable portable to other Operating Systems than Linux or Unix.


======
USAGE
======

The design choice was to separate the two different kind of metaheuristic
using two different executable files, in order to limit the number of 
command line parameters to be given as input to the program.

Usage: TSPTW-II [PARAMETERS] -i [INPUT_FILE] -s [SEEDS_FILE]

Flag 		 	Argument 	 Description

-d,--random 	[No] 		 Generate random initial solution.
-h,--heuristic	[No] 		 Generate initial solution using heuristic.
-f,--first-imp 	[No] 		 Use first improvement pivoting rule.
-b,--best-imp 	[No] 		 Use best improvement pivoting rule.
-t,--transpose 	[No] 		 Use transpose neighborhood.
-e,--exchange 	[No] 		 Use exchange neighborhood.
-n,--insert 	[No] 		 Use insert neighborhood.
-i,--input	 	[Req,Path] 	 Path to instance to be given as input to the program
-r,--runs	 	[Opt,Runs] 	 Number of runs of the algorithm. 1 if omitted.
-s,--seed	 	[Req,Seed] 	 Path to the file containing the list of seeds.
-k,--known-best	[Opt,Best] 	 Best known solution for the analyzed instance. INT_MAX if omitted.

Usage: TSPTW-VND [PARAMETERS] -i [INPUT_FILE] -s [SEEDS_FILE]

Flag 		 	Argument 	 Description

-t,--standard 	[No] 		 Use standard VND algorithm.
-p,--piped	 	[No] 		 Use piped VND algorithm.
-a,--TEI 	 	[No] 		 Use Transpose-Exchange-Insert neighborhood chain.
-b,--TIE 	 	[No] 		 Use Transpose-Insert-Exchange neighborhood chain.
-i,--input	 	[Req,Path] 	 Path to instance to be given as input to the program
-r,--runs	 	[Opt,Runs] 	 Number of runs of the algorithm. 1 if omitted.
-s,--seed	 	[Req,Seed] 	 Path to the file containing the list of seeds.
-k,--known-best	[Opt,Best] 	 Best known solution for the analyzed instance. INT_MAX if omitted.


For each of the parameters, in the column argument it is indicated whether
an argument to the option is not required, optional or mandatory.

The argument to the input option must contain the full path to the instance file.

The only mandatory options are "-i, --input" and "-s,-seeds", since
without these components will not be possible to execute the simulation.

The seeds file must contain a number of seeds at least equal to the number of
runs required, one seed per line without any additional information.

The options controlling the same parameters are mutually exclusive, with that
meaning that only one option must be selected in order to run the program.
If multiple options for the same parameter are chosen, the program will not
run.

[TSPTW-II]

Parameter			Mutually exclusive options

Initial solution  	-d,--random , -h,--heuristic
Neighborhood type 	-t,--transpose , -e,--exchange , -n,--insert 
Pivoting rule 		-f,--first-imp , -b,--best-imp 
 	

[TSPTW-VND]

Parameter			Mutually exclusive options

VND algorithm  		-t,--standard , -p,--piped
Neighborhood chain 	-a,--TEI  , -b,--TIE 


==========================
Default parameter settings
==========================

The default parameter settings are:

[TSPTW-II]

Initial solution: Random
Neighborhood type: Transpose
Pivoting rule: First improvement
Runs: 1
Known best: INT_MAX


[TSPTW-VND]

Neighborhood chain: Transpose-Exchange-Insert
VND Type: Standard VND
Runs: 1
Known best: INT_MAX


==========================
Examples
==========================

Examples for running an experiments are:

./TSPTW-II -h -f -t -i ./instances/n80w20.001.txt -s testseed -r 2 -k 616
./TSPTW-VND -t -a -i ./instances/n80w20.001.txt -s testseed -r 2 -k 616

or

./TSPTW-II --heuristic --first-imp --transpose --input ./instances/n80w20.001.txt --seeds testseed -runs 2 --known-best 616
./TSPTW-VND --standard --TEI --input ./instances/n80w20.001.txt --seed testseed --runs 2 --known-best 616


=======
OUTPUT
=======

The program outputs on the standard error all the relevant errors occurred
in the execution of the program.

At the beginning of its execution the program outputs on the standard output
the parameter setting for the current experiment.

Then, for each run the program outputs the following informations:

Run <run_number> - Seed <run_seed>
Solution found in <runtime> s.
Tour: <best_solution_tour>
Constraint Violations: <best_solution_constraint_violation>
Total travel time: <best_solution_total_travel_time>

Every experiment produces a single file:

[TSPTW-II]

<pivoting_rule>.<neighborhood_type>.<instance_name>
	{first,			{transpose,
	 best}			 exchange,
					 insert}

[TSPTW-VND]

<vnd_type>.[neighborhood_chain].<instance_name>
{standard,		{tei,
 piped}			 tie}
 
Ex: exchange.best.n80w200.004.txt
	standard.tei.n80w20.003.txt

The internal structure of the file is the following:

Seed	CV	CpuTime	PRPD

For each run of the algorithm, the program writes in the file, using a tabulation as separator,
the used seed, the number of constraints violations, the cpu runtime
and the penalised relative percentage deviation, computed as:

100*(((bestFoundSolution+constraintViolations*CONSTRAINT_VIOLATION_PENALTY)-known_best_solution)/known_best_solution)

with CONSTRAINT_VIOLATION_PENALTY being 10^4.

The CSV structure of the file allows for an easy manipulation and post-processing using R.

=======
SCRIPTS
=======

The software comes bundled with two scripts, each of them in the Iterative Improvement and Variable Neighborhood Descent form.
The scripts are used respectively, to launch all the tested algorithm on a single instance and to process the results to draw the box plots
and to compute the required statistics.

=============
launchTSPTW-X (X being either II or VND)
=============

Usage : launchTSPTW-X [INSTANCE_NAME] [RUNS] [SEEDS_FILE]

The script takes as input exactly three parameters:
[INSTANCE_NAME]: String indicating the instance name (all the instances are assumed to be in the instances folder)
[RUNS] : Integer number indicating the number of runs of the algorithm
[SEEDS_FILE] : String indicating the name of the file where the generated seeds will be stored.

The script will then proceed to launch 6 times (-II - transpose.first, exchange.first, insert.first, transpose.best, exchange.best, insert.best)
or 4 times (-VND - standard.tei, standard.tie, piped.tei, piped.tie) and after completion of all the experiments,
launch the corresponding processing script (processDataX.R) to process the results.

The output of the script will be:

<instanceName>-CpuTime.pdf
<instanceName>-PRPD.pdf
Update of transpose.first, exchange.first, insert.first, transpose.best, exchange.best, insert.best (II) or standard.tei, standard.tie, piped.tei, piped.tie (VND) files.

The .pdf files will containing respectively, the box plot comparing the runtime of the different algorithms and the solution quality, measured in terms of PRPD.
The latter files are CSV files having the same name as the algorithm to evaluate, containing one line for each instance.
Each line is composed by:

Instance	Infeasible	mean(PRDP)	mean(CpuTime)

which represents: the instance name, the percentage of infeasible runs, the mean PRDP and the mean runtime across 100 runs.

N.B. : In order to process the data, a working R distribution (including the package Rscript) must be installed.

If you have any comments or questions please write to: jacopo.de.stefani@ulb.ac.be
