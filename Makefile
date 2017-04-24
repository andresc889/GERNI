# GERNI4CPP Makefile
# Based on https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

# Paths to 3rd Party Libraries
BOOST = /s/chopin/l/grad/andrescj/3rd_party/boost_1_60_0
AGE = /s/chopin/l/grad/andrescj/3rd_party/AGE-1.1.1
EXPRTK = /s/chopin/l/grad/andrescj/3rd_party/exprtk
PLPLOT = /s/chopin/l/grad/andrescj/3rd_party/plplot-5.11.1

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -D AGE_VERSION=1.1.1 -D NDEBUG -O3

# Name of the executable file
MAIN = GERNI4CPP

# Include paths
INCLUDES = -I $(BOOST) \
	-I $(AGE)/include/AGE \
	-I $(EXPRTK) \
	-I $(PLPLOT)/include/plplot

# Library paths
LFLAGS = -L $(AGE)/lib \
	-L $(PLPLOT)/lib \
	-Wl,-rpath=$(PLPLOT)/lib

# Libraries to link
LIBS = -l AGE -l plplotcxx -l pthread

# Object files
OBJS = GERNI4CPP.o \
	Graph.o \
	ODESolver.o \
	ODESystem.o \
	ODESystemFitnessEvaluator.o \
	Plotter.o

# Headers
HEADERS = Graph.hpp \
	ODE.hpp \
	ODESolver.hpp \
	ODESystem.hpp \
	ODESystemFitnessEvaluator.hpp \
	Plotter.hpp

# Default target
all: $(MAIN)
	@echo "GERNI4CPP has been compiled"
	@echo ""

# Link target
$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	@echo ""

# Compile target
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo ""

# Clean target
clean:
	rm -rf *.o *~ $(MAIN)

# If header files change, recompile
$(OBJS): $(HEADERS)
