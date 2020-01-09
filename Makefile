# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

CC = g++
LINKERFLAG = -std=c++14
BLDDIR = build
SRCDIR = src

SRCS = $(wildcard $(SRCDIR)/*.cpp)
# BINS = $(patsubst $(SRCDIR)/%.cpp,$(BLDDIR)/%,$(SRCS)) 
BINS = $(SRCS:$(SRCDIR)/%.cpp=%)

all: dir ${BINS}

dir: 
	@mkdir -p $(BLDDIR)

%: $(SRCDIR)/%.cpp
	@echo "Building.."
	${CC} ${LINKERFLAG} $< -o $(BLDDIR)/$@ -ljpeg


clean:
	@echo "Cleaning up..."
	rm -rvf $(BLDDIR)/${BINS}
