#!/bin/bash
# $1 = Instance name
# $2 = Runs
# $3 = Seeds output file

function createReportFile {
	touch $1
	echo "Instance	Infeasible	PRDP	CpuTime" > $1
}


function generateRandomSeeds {
	count=0

	while [ "$count" -lt $1 ]      
	do
		number=$RANDOM
		echo "$number\n" >> $2
	let "count += 1"  # Increment count.
	done
}

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

instanceDir="./Instances/"
scriptDir="./script/"
instanceName=$1


./TSPTW --transpose --first-impr --input ${instanceDir}$1 -r $2 -k bestSolutions[$1]
./TSPTW --exchange --first-impr --input ${instanceDir}$1 -r $2 -k bestSolutions[$1]
./TSPTW --insert --first-impr --input ${instanceDir}$1 -r $2 -k bestSolutions[$1]
./TSPTW --transpose --best-impr --input ${instanceDir}$1 -r $2 -k bestSolutions[$1]
./TSPTW --exchange --best-impr --input ${instanceDir}$1 -r $2 -k bestSolutions[$1]
./TSPTW --insert --first-impr --input ${instanceDir}$1 -r $2 -k bestSolutions[$1]


echo Rscript ${scriptDir}processData.R transpose.first.$instanceName exchange.first.$instanceName insert.first.$instanceName transpose.best.$instanceName exchange.best.$instanceName insert.best.$instanceName

