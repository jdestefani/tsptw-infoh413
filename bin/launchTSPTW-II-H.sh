#!/bin/bash
# $1 = Instance name
# $2 = Runs
# $3 = Seeds output file

createReportFile() {
	if [ ! -f $1 ]
	then
		touch $1
		echo "Instance	Infeasible	mean(PRDP)	mean(CpuTime)" > $1
	fi
	
}

generateRandomSeeds() {
	count=0
	
	if [ -f $2 ]
	then
		rm -f $2
	fi

	while [ "$count" -lt $1 ]      
	do
		number=$RANDOM
		echo "$number" >> $2
	let "count += 1"  # Increment count.
	done
}

usage() {
	echo 
	echo "usage $0 [INSTANCE_NAME] [RUNS] [SEEDS_FILE]"
	echo 
}

if [ $# -ne 3 ]
then
	usage
	exit
fi

declare -A bestSolutions

#Manual encoding of best solutions
bestSolutions[n80w20.001.txt]=616
bestSolutions[n80w20.002.txt]=737
bestSolutions[n80w20.003.txt]=667
bestSolutions[n80w20.004.txt]=615
bestSolutions[n80w20.005.txt]=748
bestSolutions[n80w200.001.txt]=491
bestSolutions[n80w200.002.txt]=488
bestSolutions[n80w200.003.txt]=466
bestSolutions[n80w200.004.txt]=526
bestSolutions[n80w200.005.txt]=440

createReportFile "transpose.first" 
createReportFile "exchange.first" 
createReportFile "insert.first" 
createReportFile "transpose.best" 
createReportFile "exchange.best" 
createReportFile "insert.best" 

generateRandomSeeds $2 $3

instanceDir="./instances/"
instanceName=$1

echo "TRANSPOSE - FIRST IMPROVEMENT"
./TSPTW-II --heuristic --transpose --first-imp --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
echo "EXCHANGE - FIRST IMPROVEMENT"
./TSPTW-II --heuristic --exchange --first-imp --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
echo "INSERT - FIRST IMPROVEMENT"
./TSPTW-II --heuristic --insert --first-imp --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
echo "TRANSPOSE - BEST IMPROVEMENT"
./TSPTW-II --heuristic --transpose --best-imp --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
echo "EXCHANGE - BEST IMPROVEMENT"
./TSPTW-II --heuristic --exchange --best-imp --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
echo "INSERT - BEST IMPROVEMENT"
./TSPTW-II --heuristic --insert --best-imp --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}


Rscript processDataII.R transpose.first.$instanceName exchange.first.$instanceName insert.first.$instanceName transpose.best.$instanceName exchange.best.$instanceName insert.best.$instanceName


