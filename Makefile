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

IISOURCES=TSPTW-II.cpp
IILOCATEDSOURCES = $(addprefix $(SRCDIR),$(IISOURCES))
IIOBJECTS=$(IISOURCES:.cpp=.o)

VNDSOURCES=TSPTW-VND.cpp
VNDLOCATEDSOURCES = $(addprefix $(SRCDIR),$(VNDSOURCES))
VNDOBJECTS=$(VNDSOURCES:.cpp=.o)

LIBS=

TSPTW-II=bin/TSPTW-II
TSPTW-VND=bin/TSPTW-VND



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
		

$(IIOBJECTS): $(IILOCATEDSOURCES)
		 $(CXX) $(COMPILEFLAGS) $(IILOCATEDSOURCES)

$(VNDOBJECTS): $(VNDLOCATEDSOURCES)
		 $(CXX) $(COMPILEFLAGS) $(VNDLOCATEDSOURCES)

$(TSPTW-II): tsptw-modules.a $(IIOBJECTS) 
	$(CXX) $(CXXFLAGS) $(IIOBJECTS) tsptw-modules.a -o $(TSPTW-II) 
	
$(TSPTW-VND): tsptw-modules.a $(VNDOBJECTS) 
	$(CXX) $(CXXFLAGS) $(VNDOBJECTS) tsptw-modules.a -o $(TSPTW-VND) 

all: $(TSPTW-II) $(TSPTW-VND)

clean:
	rm -f $(IIOBJECTS) $(VNDOBJECTS) tsptw-modules.a

.PHONY: clean
