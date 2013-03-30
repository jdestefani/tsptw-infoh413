#!/bin/bash
# The only parameter required is the instance name

instanceNameArray=$(echo $1| tr "." "\n")

instanceName=$instanceNameArray[0]
instanceName+="."
instanceName+=$instanceNameArray[1]

./TSPTW --transpose --first-impr --input ./Instances/$1
./TSPTW --exchange --first-impr --input ./Instances/$1
./TSPTW --insert --first-impr --input ./Instances/$1
./TSPTW --transpose --best-impr --input ./Instances/$1
./TSPTW --exchange --best-impr --input ./Instances/$1
./TSPTW --insert --first-impr --input ./Instances/$1

cd script
Rscript processData.R transpose.first.$instanceName exchange.first.$instanceName insert.first.$instanceName transpose.best.$instanceName exchange.best.$instanceName insert.best.$instanceName

