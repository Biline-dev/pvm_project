CC = gcc
CFLAGS = -Wall -O2
BDIR = $(PWD)

# Source files
MASTER_SRC = master.c point.c
SLAVE_SRC = slave.c point.c
UPPER_SRC = upperParaSimple.c point.c

# Object files
MASTER_OBJ = $(MASTER_SRC:.c=.o)
SLAVE_OBJ = $(SLAVE_SRC:.c=.o)
UPPER_OBJ = $(UPPER_SRC:.c=.o)

EXEC = $(PROG:%=$(BDIR)/%)

PVM_LIB = -lpvm3
CFLAGS =  -I$(PVM_ROOT)/include -D_POSIX_C_SOURCE=2 -DEPATH=\"$(BDIR)\"

# Executables
MASTER_EXEC = $(BDIR)/master
SLAVE_EXEC = $(BDIR)/slave
UPPER_EXEC = $(BDIR)/upperParaSimple

# Compilation rules
all: $(MASTER_EXEC) $(SLAVE_EXEC) $(UPPER_EXEC)

$(UPPER_EXEC): $(UPPER_OBJ)
	$(CC) -o $@ $(UPPER_OBJ) -lpvm3

$(MASTER_EXEC): $(MASTER_OBJ)
	$(CC) -o $@ $(MASTER_OBJ) -lpvm3

$(SLAVE_EXEC): $(SLAVE_OBJ)
	$(CC) -o $@ $(SLAVE_OBJ) -lpvm3

.c.o: point.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(MASTER_EXEC) $(SLAVE_EXEC) upper_hull.pdf
