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


createReportFile "standard.tei"
createReportFile "standard.tie"
createReportFile "piped.tei"
createReportFile "piped.tie"

generateRandomSeeds $2 $3

instanceDir="./instances/"
instanceName=$1


./TSPTW-VND --standard --TEI --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
./TSPTW-VND --standard --TIE --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
./TSPTW-VND --piped --TEI --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}
./TSPTW-VND --piped --TIE --input ${instanceDir}$1 -r $2 -s $3 -k ${bestSolutions[$1]}



Rscript processDataVND.R standard.tei.$instanceName standard.tie.$instanceName piped.tei.$instanceName piped.tie.$instanceName

