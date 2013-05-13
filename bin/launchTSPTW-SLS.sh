#!/bin/bash
# $1 = Instance name
# $2 = Runs
# $3 = Seeds output file
# $4 = Max runtime

createReportFile() {
	if [ ! -f $1 ]
	then
		touch $1
		echo "Instance	Infeasible	mean(PRDP)" > $1
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
	echo "usage $0 [INSTANCE_NAME] [RUNS] [SEEDS_FILE] [MAX_RUNTIME]"
	echo 
}

if [ $# -ne 4 ]
then
	usage
	exit
fi

declare -A bestSolutions

# Manual encoding of best solutions
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


createReportFile "ACO.stat"
createReportFile "SA.stat"

generateRandomSeeds $2 $3

instanceDir="./instances/"
instanceName=$1

# Command line interface for the algorithms still to define
#./TSPTW-ACO --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
./TSPTW-SA  --input ${instanceDir}$1 -p 1000 -r $2 -s $3 -t $4 -k ${bestSolutions[$1]}



#Rscript processDataSLS.R ACO.$instanceName SA.$instanceName

