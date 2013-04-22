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

standardTeiFile <- args[1]
standardTieFile <- args[2]
pipedTeiFile <- args[3]
pipedTieFile <- args[4]

rm(args)

#Compute statistics for all the files
standardTeiResults <- computeStatistics(standardTeiFile)
standardTieResults <- computeStatistics(standardTieFile)
pipedTeiResults <- computeStatistics(pipedTeiFile)
pipedTieResults <- computeStatistics(pipedTieFile)


#Detect instance names
instanceName <- standardTeiResults[[1]]

#Extract algorithm types to label columns in data frame
columnNames <- cbind( "s.tei",
					  "s.tie",
					  "p.tei",
					  "p.tie")

#Extract data for PRDP plots
PRDPBoxPlotData <- list(standardTeiResults[[4]][1],
						standardTieResults[[4]][1],
                         pipedTeiResults[[4]][1],
                         pipedTieResults[[4]][1])
        

# Statistical tests
#Compare Tie vs. Tei chain for each neighborhood
standardWilcoxon <- wilcox.test(standardTeiResults[[4]][[2]],standardTieResults[[4]][[2]],paired=TRUE)
pipedWilcoxon <- wilcox.test(pipedTeiResults[[4]][[2]],pipedTieResults[[4]][[2]],paired=TRUE)
#Compare piped vs. standard neighborhood for each pivoting rule.
TeiWilcoxon <- wilcox.test(standardTeiResults[[4]][[2]],pipedTeiResults[[4]][[2]],paired=TRUE)
TieWilcoxon <- wilcox.test(standardTieResults[[4]][[2]],pipedTieResults[[4]][[2]],paired=TRUE)


#Extract data for CpuRunTime
CpuTimeBoxPlotData <- list(standardTeiResults[[4]][2],
						   standardTieResults[[4]][2],
                           pipedTeiResults[[4]][2],
                           pipedTieResults[[4]][2])

#colnames(PRDPBoxPlotData) <- columnNames
#colnames(CpuTimeBoxPlotData) <- columnNames

#Box plots
#boxplotToPdf(paste(instanceName,"PRPD",sep="-"),PRDPBoxPlotData,columnNames,paste(instanceName,"PRPD",sep="-"),"RPD")
#boxplotToPdf(paste(instanceName,"CpuTime",sep="-"),CpuTimeBoxPlotData,columnNames,paste(instanceName,"Runtime",sep="-"),"Time(s)")

#Write statistics in separate files for each algorithm
write(standardTeiResults[[3]],standardTeiResults[[2]],append=TRUE)
write(standardTieResults[[3]],standardTieResults[[2]],append=TRUE)
write(pipedTeiResults[[3]],pipedTeiResults[[2]],append=TRUE)
write(pipedTieResults[[3]],pipedTieResults[[2]],append=TRUE)

write(paste("Tei vs Tie - Standard",standardWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
write(paste("Tei vs Tie - Piped",pipedWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
write(paste("Standard vs Piped - Tei",TeiWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
write(paste("Standard vs Piped - Tie",TieWilcoxon$p.value,sep="\t"),paste(instanceName,"Tests",sep=""),append=TRUE)
