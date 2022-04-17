# Makefile for travelMonitor and Monitor
SRC = ./src

# .o files will be stored here
BLD = ./obj
BLD_LISTENER = $(BLD)/listener
BLD_MANAGER = $(BLD)/manager
BLD_WORKER = $(BLD)/workers

# Specify directories of .c files
MANAGER = $(SRC)/manager
WORKER = $(SRC)/worker
LISTENER = $(SRC)/listener

# Compiler options
CC = gcc
CFLAGS = -Wall -Wextra -I. -I$(SRC) -Wno-unused-parameter

# Executable file names
EXE_SNIFFER = ./sniffer

# manager's .o needed
OBJS_MANAGER =  $(MANAGER)/main.o

# Build executables
all: $(EXE_SNIFFER)
	mkdir -p $(BLD_LISTENER)
	mkdir -p $(BLD_MANAGER)
	mkdir -p $(BLD_WORKER)
	mv -f $(OBJS_MANAGER) $(BLD_MANAGER)

$(EXE_SNIFFER): $(OBJS_MANAGER)
	$(CC) $(CFLAGS) $(OBJS_MANAGER) -o $(EXE_SNIFFER)

# Delete executable & object files
clean:
	rm -f $(EXE_SNIFFER)
	rm -rf $(BLD)

# Clean and compile
comp: clean all