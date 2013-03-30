CXX = g++

COMPILEFLAGS = -c

CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -lrt

MOSOURCES=CandidateSolution.cpp InstanceReader.cpp Writer.cpp HeuristicCore.cpp
MOHEADERS=$(MOSOURCES:.cpp=.h)
MOOBJECTS=$(MOSOURCES:.cpp=.o)

#DASOURCES= TimeWindow.cpp CandidateSolution.cpp 
#DAHEADERS=$(DASOURCES:.cpp=.h)
#DAOBJECTS=$(DASOURCES:.cpp=.o)

MAINSOURCES=TSPTW.cpp

OBJS=TSPTW.o

LIBS=

TARGET=TSPTW



#%.o: %.cpp 
#	$(CXX) $(COMPILEFLAGS) $< $(DIRFLAG)

#tsptw-datamodels.a: $(DASOURCES) $(DAHEADERS)
#		$(CXX) $(COMPILEFLAGS) $(DASOURCES) 
#		ar -r tsptw-datamodels.a $(DAOBJECTS) 
#		ranlib tsptw-datamodels.a

tsptw-modules.a: $(MOSOURCES) $(MOHEADERS)
		$(CXX) $(COMPILEFLAGS) $(MOSOURCES)  
		ar -r tsptw-modules.a $(MOOBJECTS) 
		ranlib tsptw-modules.a

$(OBJS): $(MAINSOURCES)
		 $(CXX) $(COMPILEFLAGS) $(MAINSOURCES)

$(TARGET): tsptw-modules.a $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) tsptw-modules.a -o $(TARGET) 

all: $(TARGET)

clean:
	rm -f $(OBJS) $(DAOBJECTS) $(MOOBJECTS) tsptw-datamodels.a tsptw-modules.a

.PHONY: clean