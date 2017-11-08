#-----------------------------------#
#            Demo Makefile            #
#-----------------------------------#

# Compiler
CXX = CC

# Targets
TARG0 = server
TARG1 = client

# Use name server
# Comment this to manually pass port name from server to client
NAMESERVER = #-DPUBLISH_NAME

# General Compiler flags
OPT0 = -O0 -g -DSERVER=1 $(NAMESERVER)
OPT1 = -O0 -g -DCLIENT=1 $(NAMESERVER)

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
	rm -f $(TARG0) $(TARG1)
