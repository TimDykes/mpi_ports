#----------------------------------------------------------#
#                      Demo Makefile                       #
#----------------------------------------------------------#

# ----------------------- OPTIONS -------------------------#

# Environment (select CRAY or OMPI)
COMP_ENV = CRAY
#COMP_ENV = OMPI

# Uncomment this to use name server (e.g. opmi-server)
#NAMESERVER = -DPUBLISH_NAME

# ---------------------------------------------------------#

# Environment settings
ifeq (CRAY,$(findstring CRAY,$(COMP_ENV)))
  CXX       = CC
  CRAY_OPT  = -craympich-dpm
else
  CXX       = mpicxx
endif

# make DEBUG=1 for debug build
ifeq ($(DEBUG), 1)
  OPTIMIZE = -g -O0
else
  OPTIMIZE = -O2
endif

# Targets
TARG0 = server
TARG1 = client

# Flags 
OPT0 = -DSERVER=1 $(NAMESERVER) $(OPTIMIZE) $(CRAY_OPT)
OPT1 = -DCLIENT=1 $(NAMESERVER) $(OPTIMIZE) $(CRAY_OPT)

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
