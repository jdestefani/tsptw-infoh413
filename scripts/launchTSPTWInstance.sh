#!/bin/bash
# The only parameter required is the instance name

declare -A bestSolutions

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


instanceNameArray=$(echo $1| tr "." "\n")

instanceName=$instanceNameArray[0]
instanceName+="."
instanceName+=$instanceNameArray[1]

instanceDir="./Instances/"

./TSPTW --transpose --first-impr --input $instanceDir$1 -r 100 -k bestSolutions[$1]
./TSPTW --exchange --first-impr --input $instanceDir$1 -r 100 -k bestSolutions[$1]
./TSPTW --insert --first-impr --input $instanceDir$1 -r 100 -k bestSolutions[$1]
./TSPTW --transpose --best-impr --input $instanceDir$1 -r 100 -k bestSolutions[$1]
./TSPTW --exchange --best-impr --input $instanceDir$1 -r 100 -k bestSolutions[$1]
./TSPTW --insert --first-impr --input $instanceDir$1 -r 100 -k bestSolutions[$1]

cd script
Rscript processData.R transpose.first.$instanceName exchange.first.$instanceName insert.first.$instanceName transpose.best.$instanceName exchange.best.$instanceName insert.best.$instanceName

