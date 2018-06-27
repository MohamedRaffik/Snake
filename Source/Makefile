CXX := /usr/bin/g++
CXXFLAGS = -Wall 
SDL = -lSDL2 -lSDL2_ttf -lSDL2_image -no-pie
SRCS = gameblocks.cpp init.cpp main.cpp snakegame.cpp textbox.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
PROG = Snake

all: $(PROG)

.PHONY: clean cleanall
clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $(PROG) $(OBJS) $(SDL) 

.cpp:
	$(CXX) -c $@.cpp $(CXXFLAGS)
