CXX = g++

COMPILEFLAGS = -c -g

CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -lrt
INCLUDEFLAGS = -I 

SRCDIR=src/
BINDIR=bin/

MOSOURCES= CandidateSolution.cpp InstanceReader.cpp Writer.cpp HeuristicCore.cpp ACOCore.cpp SACore.cpp
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

ACOSOURCES=TSPTW-ACO.cpp
ACOLOCATEDSOURCES = $(addprefix $(SRCDIR),$(ACOSOURCES))
ACOOBJECTS=$(ACOSOURCES:.cpp=.o)

SASOURCES=TSPTW-SA.cpp
SALOCATEDSOURCES = $(addprefix $(SRCDIR),$(SASOURCES))
SAOBJECTS=$(SASOURCES:.cpp=.o)


LIBS=

TSPTW-II=bin/TSPTW-II
TSPTW-VND=bin/TSPTW-VND
TSPTW-ACO=bin/TSPTW-ACO
TSPTW-SA=bin/TSPTW-SA


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

$(TSPTW-ACO): tsptw-modules.a $(ACOOBJECTS) 
	$(CXX) $(CXXFLAGS) $(ACOOBJECTS) tsptw-modules.a -o $(TSPTW-ACO) 
	
$(TSPTW-SA): tsptw-modules.a $(SAOBJECTS) 
	$(CXX) $(CXXFLAGS) $(SAOBJECTS) tsptw-modules.a -o $(TSPTW-SA) 


all: $(TSPTW-II) $(TSPTW-VND) $(TSPTW-ACO) $(TSPTW-SA)

clean:
	rm -f $(IIOBJECTS) $(VNDOBJECTS) $(ACOOBJECTS) $(SAOBJECTS) tsptw-modules.a

.PHONY: clean
