# set up how compilation will happen
CC = clang -m64
CFLAGS = -g -O1
LDFLAGS = -lm

# define the set of files used
objects = tlb.o

tlb.a: mlpt.a config.h $(objects)
	ar rcs tlb.a $(objects)

# default to building everything, running nothing
all: tlb


# Create a runner target ...
tlb: main.o $(objects)
	$(CC) $(LDFLAGS) $^ -o $@

# ... and a target that runs it
run: tlb
	./tlb



# Create a tester target ...
tester: tester.o $(objects)
	$(CC) $(LDFLAGS) $^ -o $@

# ... and a target that runs it
test: tester
	./tester



# genetic rule to build a .o from any .c
# see https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@



# something to remove files the makefile created
clean:
	rm -f runner tester main.o tester.o tlb.a $(objects) 

# mark a few targets as not producing files
.PHONEY: all run test clean

