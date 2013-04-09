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
columnNames <- cbind("t.f",
                     "e.f",
                     "i.f",
                     "t.b",
                     "e.b",
                     "i.b")

#Extract data for PRDP plots
PRDPBoxPlotData <- list(transposeFirstResults[[4]][1],
                         exchangeFirstResults[[4]][1],
                         insertFirstResults[[4]][1],
                         transposeBestResults[[4]][1],
                         exchangeBestResults[[4]][1],
                         insertBestResults[[4]][1])
        

# Statistical tests
#Compare best vs. ﬁrst-improvement for each neighborhood
transposeWilcoxon <- wilcox.test(transposeFirstResults[[4]][[2]],transposeBestResults[[4]][[2]],paired=TRUE)
exchangeWilcoxon <- wilcox.test(exchangeFirstResults[[4]][[2]],exchangeBestResults[[4]][[2]],paired=TRUE)
insertWilcoxon <- wilcox.test(insertFirstResults[[4]][[2]],insertBestResults[[4]][[2]],paired=TRUE)
#Compare exchange vs. insertion neighborhood for each pivoting rule.
firstWilcoxon <- wilcox.test(exchangeFirstResults[[4]][[2]],insertFirstResults[[4]][[2]],paired=TRUE)
bestWilcoxon <- wilcox.test(exchangeBestResults[[4]][[2]],insertBestResults[[4]][[2]],paired=TRUE)


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
boxplotToPdf(paste(instanceName,"PRPD",sep="-"),PRDPBoxPlotData,columnNames,paste(instanceName,"PRPD",sep="-"),"RPD")
boxplotToPdf(paste(instanceName,"CpuTime",sep="-"),CpuTimeBoxPlotData,columnNames,paste(instanceName,"Runtime",sep="-"),"Time(s)")

#Write statistics in separate files for each algorithm
#write(transposeFirstResults[[3]],transposeFirstResults[[2]],append=TRUE)
#write(exchangeFirstResults[[3]],exchangeFirstResults[[2]],append=TRUE)
#write(insertFirstResults[[3]],insertFirstResults[[2]],append=TRUE)
#write(transposeBestResults[[3]],transposeBestResults[[2]],append=TRUE)
#write(exchangeBestResults[[3]],exchangeBestResults[[2]],append=TRUE)
#write(insertBestResults[[3]],insertBestResults[[2]],append=TRUE)


write(paste("First vs best - Transpose",transposeWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
write(paste("First vs best - Exchange",exchangeWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
write(paste("First vs best - Insert",insertWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
write(paste("Exchange vs Insert - First",firstWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
write(paste("Exchange vs Insert - Best",bestWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
