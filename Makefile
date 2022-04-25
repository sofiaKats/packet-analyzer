# Makefile for sniffer
SRC = ./src

# .o files will be stored here
BLD = ./obj

# Compiler options
CC = gcc
CFLAGS = -Wno-unused-parameter -g

# Executable file names
EXE_SNIFFER = ./sniffer

# .o files needed
OBJS =  $(SRC)/main.o $(SRC)/queue.o $(SRC)/communication.o

# Build executables
all: $(EXE_SNIFFER)
	mkdir -p $(BLD)
	mv -f $(OBJS) $(BLD)

$(EXE_SNIFFER): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE_SNIFFER)

# Delete executable & object files
clean:
	rm -f $(EXE_SNIFFER)
	rm -rf $(BLD)

# Clean and compile
comp: clean all