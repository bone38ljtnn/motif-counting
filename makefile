IPATH = ./MoTTo
SPATH = .

OBJS = main.o Constraint.o TemporalGraph.o CountAllMotifs.o
CC = g++
DEBUG = -g
CFLAGS = -std=c++17 -O3 -Wall

OMPFLAG = true

ifeq ($(OMPFLAG),true)
	CFLAGS += -fopenmp
endif

motto : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o motto

main.o : $(SPATH)/main.cpp $(IPATH)/TemporalGraph.h $(IPATH)/Constraint.h $(IPATH)/CountAllMotifs.h
	$(CC) $(CFLAGS) -c $(SPATH)/main.cpp -I $(IPATH)

TemporalGraph.o : $(SPATH)/TemporalGraph.cpp
	$(CC) $(CFLAGS) -c $(SPATH)/TemporalGraph.cpp -I $(IPATH)

Constraint.o : $(SPATH)/Constraint.cpp
	$(CC) $(CFLAGS) -c $(SPATH)/Constraint.cpp -I $(IPATH)

CountAllMotifs.o : $(SPATH)/CountAllMotifs.cpp
	$(CC) $(CFLAGS) -c $(SPATH)/CountAllMotifs.cpp -I $(IPATH)

clean :
	rm motto $(OBJS)
