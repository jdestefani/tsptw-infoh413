boxplotToPdf <- function(file,data,colNames,pdfHeight,pdfWidth){
  pdf(file,height=pdfHeight,width=pdfWidth)
  boxplot(as.data.frame(data),names=colNames)
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
  meanCpuTime <- mean(inputData[,"CpuTime"])
  
  statistics <- paste(instanceName,infeasibleRunPercentage,meanPRDP,meanCpuTime,sep="\t")
  boxplots <- list(inputData[,"PRPD"],inputData[,"CpuTime"])
  returnValues <- list(instanceName,outputFile,statistics,boxplots)
  return(returnValues)
}

library(MASS)
args <- commandArgs(trailingOnly = TRUE)
print(args)
# trailingOnly=TRUE means that only arguments after --args are returned
# if trailingOnly=FALSE then you got:
# [1] "--no-restore" "--no-save" "--args" "2010-01-28" "example" "100"

transposeFirstFile <- args[1]
exchangeFirstFile <- args[2]
insertFirstFile <- args[3]
transposeBestFile <- args[4]
exchangeBestFile <- args[5]
insertBestFile <- args[6]
rm(args)

#Compute statistics for all the files
transposeFirstResults <- computeStatistics(transposeFirstFile)
exchangeFirstResults <- computeStatistics(exchangeFirstFile)
insertFirstResults <- computeStatistics(insertFirstFile)
transposeBestResults <- computeStatistics(transposeBestFile)
exchangeBestResults <- computeStatistics(exchangeBestFile)
insertBestResults <- computeStatistics(insertBestFile)

#Detect instance names
instanceName <- transposeFirstResults[[1]]

#Extract algorithm types to label columns in data frame
columnNames <- cbind(transposeFirstResults[[2]],
                     exchangeFirstResults[[2]],
                     insertFirstResults[[2]],
                     transposeBestResults[[2]],
                     exchangeBestResults[[2]],
                     insertBestResults[[2]])

#Extract data for PRDP plots
PRDPBoxPlotData <- list(transposeFirstResults[[4]][1],
                         exchangeFirstResults[[4]][1],
                         insertFirstResults[[4]][1],
                         transposeBestResults[[4]][1],
                         exchangeBestResults[[4]][1],
                         insertBestResults[[4]][1])
        

print(str(transposeFirstResults[[4]][2]))
# Statistical tests
#Compare best vs. ﬁrst-improvement for each neighborhood
transposeWilcoxon <- wilcox.test(as.numeric(transposeFirstResults[[4]][2]),as.numeric(transposeBestResults[[4]][2]),paired=TRUE)
print(transposeWilcoxon)
#exchangeWilcoxon <- wilcox.test(exchangeFirstResults[[4]][2],exchangeBestResults[[4]][2],paired=TRUE)
#insertWilcoxon <- wilcox.test(insertFirstResults[[4]][2],insertBestResults[[4]][2],paired=TRUE)
#Compare exchange vs. insertion neighborhood for each pivoting rule.
#firstWilcoxon <- wilcox.test(exchangeFirstResults[[4]][2],insertFirstResults[[4]][2],paired=TRUE)
#bestWilcoxon <- wilcox.test(exchangeBestResults[[4]][2],insertBestResults[[4]][2],paired=TRUE)

#Extract data for CpuRunTime
CpuTimeBoxPlotData <- list(transposeFirstResults[[4]][2],
                         exchangeFirstResults[[4]][2],
                         insertFirstResults[[4]][2],
                         transposeBestResults[[4]][2],
                         exchangeBestResults[[4]][2],
                         insertBestResults[[4]][2])

#colnames(PRDPBoxPlotData) <- columnNames
#colnames(CpuTimeBoxPlotData) <- columnNames

#Box plots
boxplotToPdf(paste(instanceName,"PRPD",sep="-"),PRDPBoxPlotData,columnNames)
boxplotToPdf(paste(instanceName,"CpuTime",sep="-"),CpuTimeBoxPlotData,columnNames)

#Write statistics in separate files for each algorithm
write(transposeFirstResults[[3]],transposeFirstResults[[2]],append=TRUE)
write(exchangeFirstResults[[3]],exchangeFirstResults[[2]],append=TRUE)
write(insertFirstResults[[3]],insertFirstResults[[2]],append=TRUE)
write(transposeBestResults[[3]],transposeBestResults[[2]],append=TRUE)
write(exchangeBestResults[[3]],exchangeBestResults[[2]],append=TRUE)
write(insertBestResults[[3]],insertBestResults[[2]],append=TRUE)
