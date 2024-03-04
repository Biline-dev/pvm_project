CC = gcc
CFLAGS = -Wall -O2
BDIR = $(PWD)

# Source files
UPPER_SRC = master.c point.c
SLAVE_SRC = slave.c point.c

# Object files
UPPER_OBJ = $(UPPER_SRC:.c=.o)
SLAVE_OBJ = $(SLAVE_SRC:.c=.o)

EXEC = $(PROG:%=$(BDIR)/%)

PVM_LIB = -lpvm3
CFLAGS =  -I$(PVM_ROOT)/include -D_POSIX_C_SOURCE=2 -DEPATH=\"$(BDIR)\"

# Executables
UPPER_EXEC = $(BDIR)/master
SLAVE_EXEC = $(BDIR)/slave

# Compilation rules
all: $(UPPER_EXEC) $(SLAVE_EXEC)

$(UPPER_EXEC): $(UPPER_OBJ)
	$(CC) -o $@ $(UPPER_OBJ) -lpvm3

$(SLAVE_EXEC): $(SLAVE_OBJ)
	$(CC) -o $@ $(SLAVE_OBJ) -lpvm3

.c.o: point.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(UPPER_EXEC) $(SLAVE_EXEC)
