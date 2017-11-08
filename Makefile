#-----------------------------------#
#            Demo Makefile            #
#-----------------------------------#

# Compiler
CXX = mpicxx

# Target
TARG0 = server
TARG1 = client

# General Compiler flags
OPT0 = -Wall -O2 -DSERVER=1 -DPUBLISH_NAME
OPT1 = -Wall -O2 -DCLIENT=1 -DPUBLISH_NAME

# Additional libraries
LIBS =

# Objects to be built
OBJS0 = client-server0.o
OBJS1 = client-server1.o
#$(patsubst %.cpp,%.o,$(wildcard *.cpp))

# Rules
all: $(TARG0) $(TARG1)

$(TARG0): $(OBJS0)
	$(CXX) $(OPT0) $(LIBS) $(OBJS0) -o $(TARG0)

$(TARG1): $(OBJS1)
	$(CXX) $(OPT1) $(LIBS) $(OBJS1) -o $(TARG1)

client-server0.o: client-server.cpp
	$(CXX) -c $(OPT0) -o "$@" "$<"

client-server1.o: client-server.cpp
	$(CXX) -c $(OPT1) -o "$@" "$<"

clean:
	rm -f *.o
	rm -f $(TARGET)%
