CXX = g++

COMPILEFLAGS = -c

CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -lrt
INCLUDEFLAGS = -I 

SRCDIR=src/
BINDIR=bin/

MOSOURCES= CandidateSolution.cpp InstanceReader.cpp Writer.cpp HeuristicCore.cpp
MOLOCATEDSOURCES = $(addprefix $(SRCDIR),$(MOSOURCES))
MOHEADERS=$(MOLOCATEDSOURCES:.cpp=.h)
MOOBJECTS=$(MOSOURCES:.cpp=.o)

#DASOURCES= TimeWindow.cpp CandidateSolution.cpp 
#DAHEADERS=$(DASOURCES:.cpp=.h)
#DAOBJECTS=$(DASOURCES:.cpp=.o)

MAINSOURCES=TSPTW.cpp
MAINLOCATEDSOURCES = $(addprefix $(SRCDIR),$(MAINSOURCES))
MAINOBJECTS=$(MAINSOURCES:.cpp=.o)

LIBS=

TARGET=bin/TSPTW



#%.o: %.cpp 
#	$(CXX) $(COMPILEFLAGS) $< $(DIRFLAG)

#tsptw-datamodels.a: $(DASOURCES) $(DAHEADERS)
#		$(CXX) $(COMPILEFLAGS) $(DASOURCES) 
#		ar -r tsptw-datamodels.a $(DAOBJECTS) 
#		ranlib tsptw-datamodels.a

tsptw-modules.a: $(MOLOCATEDSOURCES) $(MOHEADERS)
		$(CXX) $(COMPILEFLAGS) $(MOLOCATEDSOURCES)  
		ar -r tsptw-modules.a $(MOOBJECTS) 
		ranlib tsptw-modules.a
		rm -f $(MOOBJECTS)
		

$(MAINOBJECTS): $(MAINLOCATEDSOURCES)
		 $(CXX) $(COMPILEFLAGS) $(MAINLOCATEDSOURCES)

$(TARGET): tsptw-modules.a $(MAINOBJECTS) 
	$(CXX) $(CXXFLAGS) $(MAINOBJECTS) tsptw-modules.a -o $(TARGET) 

all: $(TARGET)

clean:
	rm -f $(MAINOBJECTS) tsptw-modules.a

.PHONY: clean