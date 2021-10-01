CC=gcc
CFLAGS=-Wall -ansi -lm -lMLV
SRC := $(shell find src -name '*.c')
SRC := $(filter-out src/main.c, $(SRC))
HEADER :=  $(shell find include -name '*.h')
_OBJ := $(notdir $(patsubst %.c, %.o, $(SRC)))
ODIR := bin

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

main: $(shell find src) $(HEADER)
	$(CC) -c $(SRC) $(CFLAGS)
	mv *.o bin
	$(CC) src/main.c $(OBJ) -o yaic $(CFLAGS)

clean:
	rm -f yaic
	rm -f bin/*.o
	rm -d -f -r doc/html
	rm -d -f -r doc/latex
	