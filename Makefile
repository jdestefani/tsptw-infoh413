CXX = g++

COMPILEFLAGS = -c

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

ENSOURCES=InstanceReader.cpp TimeWindow.cpp CandidateSolution.cpp HeuristicCore.cpp
ENHEADERS=$(ENSOURCES:.cpp=.h)
ENOBJECTS=$(ENSOURCES:.cpp=.o)

MAINSOURCES=TSPTW.cpp

OBJS=TSPTW.o

LIBS=

TARGET=TSPTW



#%.o: %.cpp 
#	$(CXX) $(COMPILEFLAGS) $< $(DIRFLAG)

tsptw-entities.a: $(ENSOURCES) $(ENHEADERS)
		$(CXX) $(COMPILEFLAGS) $(ENSOURCES) 
		ar -r tsptw-entities.a $(ENOBJECTS) 
		ranlib tsptw-entities.a

$(OBJS): $(MAINSOURCES)
		 $(CXX) $(COMPILEFLAGS) $(MAINSOURCES)

$(TARGET): $(OBJS) tsptw-entities.a 
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) tsptw-entities.a -o $(TARGET) 

all: $(TARGET)

clean:
	rm -f $(OBJS) $(ENOBJECTS) tsptw-entities.a

.PHONY: clean