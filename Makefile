CXX = g++

COMPILEFLAGS = -c

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

ENSOURCES=InstanceReader.cpp TimeWindow.cpp
ENHEADERS=$(ENSOURCES:.cpp=.h)
ENOBJECTS=$(ENSOURCES:.cpp=.o)

OBJS =		TSPTW.o

LIBS =

TARGET =	TSPTW



%.o: %.cpp 
	$(CXX) $(CFLAGS) $< $(DIRFLAG)

tsptw-entities.a: $(ENSOURCES) $(ENHEADERS)
		$(CXX) $(CFLAGS) $(ENSOURCES) 
		ar -r tsptw-entities.a $(MOOBJECTS) 
		ranlib tsptw-entities.a

$(TARGET):	tsptw-entities.a $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) tsptw-entities.a

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(ENOBJECTS)

.PHONY: clean