CXX = g++

COMPILEFLAGS = -c

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

ENSOURCES=InstanceReader.cpp TimeWindow.cpp
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
		ar -r tsptw-entities.a $(MOOBJECTS) 
		ranlib tsptw-entities.a

$(OBJS): $(MAINSOURCES)
		 $(CXX) $(COMPILEFLAGS) $(MAINSOURCES)

$(TARGET): $(OBJS) tsptw-entities.a 
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) tsptw-entities.a

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(ENOBJECTS) tsptw-entities.a

.PHONY: clean