boxplotToPdf <- function(file,data,colNames,stringTitle,stringXLabel,pdfHeight,pdfWidth){
  pdf(file)
  boxplot(as.data.frame(data),names=colNames,horizontal=TRUE,main=stringTitle,col=terrain.colors(length(colNames)),las=1)
  title(xlab=stringXLabel)
  dev.off()
}

computeStatistics <- function(inputFile){
  #Read data and compute errors for each cluster
  inputData <- read.table(inputFile, header = TRUE)
  
  #Detect algorithm type
  tokens <- unlist(strsplit(inputFile,"\\."))
  outputFile <- paste(tokens[1],tokens[2],sep=".")
  instanceName <- paste(tokens[3],tokens[4],sep=".")
  
  #Compute number of infeasible runs
  infeasibleRunPercentage <- sum(inputData[,"CV"]*1 > 0)/length(inputData[,"CV"])
  
  #Mean penalised relative percentage deviation and cpu time
  meanPRDP <- mean(inputData[,"PRPD"])
  
  statistics <- paste(instanceName,infeasibleRunPercentage,meanPRDP,sep="\t")
  boxplots <- list(inputData[,"PRPD"])
  returnValues <- list(instanceName,outputFile,statistics,boxplots)
  return(returnValues)
}

library(MASS)

args <- commandArgs(trailingOnly = TRUE)
print(args)
# trailingOnly=TRUE means that only arguments after --args are returned
# if trailingOnly=FALSE then you got:
# [1] "--no-restore" "--no-save" "--args" "2010-01-28" "example" "100"

ACOFile <- args[1]
SAFile <- args[2]
rm(args)

#Compute statistics for all the files
ACOResults <- computeStatistics(ACOFile)
SAResults <- computeStatistics(SAFile)

#Detect instance names
instanceName <- ACOResults[[1]]

#Extract algorithm types to label columns in data frame
columnNames <- cbind("ACO",
                     "SA")

#Extract data for PRDP plots
PRDPBoxPlotData <- list(ACOResults[[4]][1],
                        SAResults[[4]][1])

# Statistical tests
#Compare ACO vs. SA for each neighborhood
testWilcoxon <- wilcox.test(ACOResults[[4]][[2]],SAResults[[4]][[2]],paired=TRUE)

#Box plots
boxplotToPdf(paste(instanceName,"PRPD",sep="-"),PRDPBoxPlotData,columnNames,paste(instanceName,"PRPD",sep="-"),"RPD")

#Write statistics in separate files for each algorithm
write(ACOResults[[3]],ACOResults[[2]],append=TRUE)
write(SAResults[[3]],SAResults[[2]],append=TRUE)

write(paste("ACO vs SA",testWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)

rm(ACOFile)
rm(SAFile)
rm(ACOResults)
rm(SAResults)
rm(PRDPBoxPlotData)
rm(testWilcoxon)