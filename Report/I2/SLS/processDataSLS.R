boxplotToPdf <- function(file,data,colNames,stringTitle,stringXLabel,pdfHeight,pdfWidth){
  pdf(paste(file,"pdf",sep='.'))
  boxplot(as.data.frame(data),names=colNames,horizontal=TRUE,main=stringTitle,col=terrain.colors(length(colNames)),las=1)
  title(xlab=stringXLabel)
  dev.off()
}

RTDToPdf <- function(file,timeScale,data,stringTitle,legend,pdfHeight,pdfWidth){
  pdf(paste(file,"pdf",sep='.'))
  #plot.ts(data)
  #boxplot(as.data.frame(data),names=colNames,horizontal=TRUE,main=stringTitle,col=terrain.colors(length(colNames)),las=1)
  
  #Set up the plot - Line types and plot chars: http://www.cookbook-r.com/Graphs/Shapes_and_line_types/
  xRange <- range(timeScale)
  yRange <- range(0,1)
  nPlots <- dim(data)[2]
  plotColors <- rainbow(nPlots) 
  lineType <- c(1:nPlots) 
  plotChar <- seq(18,18+nPlots,1)
  
  plot(xRange,yRange, type="n", xlab="Time (s)", ylab=expression(p) , log = 'x' )   

  # add lines
  matlines(timeScale, data, type="l" , lwd=1.5, lty=lineType, col=plotColors, pch=plotChar)

  #Add a title
  title(stringTitle)
  
  # add a legend 
  legend(xRange[1], yRange[2], legend, cex=0.8, col=plotColors, pch=plotChar, lty=lineType, title="QRTD")
  dev.off()
}


computeStatistics <- function(inputFile,PRPD){
  #Read data and compute errors for each cluster
  inputData <- read.table(inputFile, header = TRUE)
  
  #Detect algorithm type
  tokens <- unlist(strsplit(inputFile,"\\."))
  outputFile <- paste(tokens[1],"stats",sep=".")
  instanceName <- paste(tokens[2],tokens[3],sep=".")
  
  #Compute number of infeasible runs
  infeasibleRunPercentage <- sum(inputData[,"CV"]*1 > 0)/length(inputData[,"CV"])
  
  #Mean penalised relative percentage deviation and cpu time
  meanPRDP <- mean(PRPD)
  
  statistics <- paste(instanceName,infeasibleRunPercentage,meanPRDP,sep="\t")
  boxplots <- PRPD
  returnValues <- list(instanceName,outputFile,statistics,boxplots)
  return(returnValues)
}

computeRTD <- function(inputFile,bestSolution){
  #Read data and compute errors for each cluster
  inputData <- read.table(inputFile, header = TRUE, check.name=FALSE)
  
  #Remove seeds column and determine time scale by the column names
  inputData['Seed'] <- NULL
  timeScale <- as.numeric(colnames(inputData))
  
  #Computed corrected PRPD
  print(inputData["100"])
  finalSolutionQuality <- inputData["100"]
  PRPD <- apply(finalSolutionQuality,1:2,function(x) (x*100))
  
  #Detect algorithm type
  tokens <- unlist(strsplit(inputFile,"\\."))
  algorithmName <- tokens[1]
  instanceName <- paste(tokens[2],tokens[3],sep=".")
  
  #Compute number of high quality runs
  RTDTwenty <- inputData < 0.2
  RTDTen <- inputData < 0.1
  RTDFive <- inputData < 0.05
  RTDTwo <- inputData < 0.02
  
  #Compute RTDs
  RTDTwenty <- apply(RTDTwenty,2,sum) / nrow(inputData)
  RTDTen <- apply(RTDTen,2,sum) / nrow(inputData)
  RTDFive <- apply(RTDFive,2,sum) / nrow(inputData)
  RTDTwo <- apply(RTDTwo,2,sum) / nrow(inputData)
  
  plots <- cbind(RTDTwenty,RTDTen,RTDFive,RTDTwo)
  returnValues <- list(instanceName,algorithmName,timeScale,plots,PRPD)
  return(returnValues)
}

library(MASS)

args <- commandArgs(trailingOnly = TRUE)
print(args)
# trailingOnly=TRUE means that only arguments after --args are returned
# if trailingOnly=FALSE then you got:
# [1] "--no-restore" "--no-save" "--args" "2010-01-28" "example" "100"

ACOFile <- args[1]
ACOFileRTD <- args[2]
SAFile <- args[3]
SAFileRTD <- args[4]
bestSolution <- as.numeric(args[5])
rm(args)

#Compute RTD for all the files
ACORTD <- computeRTD(ACOFileRTD,bestSolution)
SARTD <- computeRTD(SAFileRTD,bestSolution)
RTDVs <- cbind(ACORTD[[4]][,4],SARTD[[4]][,4])

#Compute statistics for all the files
ACOResults <- computeStatistics(ACOFile,ACORTD[[5]])
SAResults <- computeStatistics(SAFile,SARTD[[5]])

#Detect instance names
instanceName <- ACOResults[[1]]

#Extract algorithm types to label columns in data frame
columnNames <- cbind("ACO",
                     "SA")

#Extract data for PRDP plots
PRDPBoxPlotData <- list(ACOResults[[4]],
                        SAResults[[4]])

# Statistical tests
#Compare ACO vs. SA for each neighborhood
testWilcoxon <- wilcox.test(ACORTD[[5]],SARTD[[5]],paired=TRUE)

#Box plots
boxplotToPdf(paste(instanceName,"PRPD",sep="-"),PRDPBoxPlotData,columnNames,paste(instanceName,"PRPD",sep="-"),"RPD")

#Real time distributions plot
RTDToPdf(paste(instanceName,"RTDs",sep="-"),ACORTD[[3]],RTDVs,paste(instanceName,"RTD",sep="-"),c("ACO","SA"))
RTDToPdf(paste("ACO",instanceName,"RTDs",sep="-"),ACORTD[[3]],ACORTD[[4]],paste("ACO",instanceName,"RTD",sep="-"),c("20%","10%","5%","2%"))
RTDToPdf(paste("SA",instanceName,"RTDs",sep="-"),SARTD[[3]],SARTD[[4]],paste("SA",instanceName,"RTD",sep="-"),c("20%","10%","5%","2%"))

#Write statistics in separate files for each algorithm
write(ACOResults[[3]],ACOResults[[2]],append=TRUE)
write(SAResults[[3]],SAResults[[2]],append=TRUE)

#Write test results in 
write(paste(instanceName,testWilcoxon$p.value,sep="\t"),"SLSWilcoxonTests",append=TRUE)

#Clean up memory
rm(ACOFile)
rm(SAFile)
rm(ACOResults)
rm(SAResults)
rm(PRDPBoxPlotData)
rm(testWilcoxon)