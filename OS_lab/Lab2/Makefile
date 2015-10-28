CC = gcc
CFLAGS = -lpthread
FILES = Lab2.c
OUT_EXE = main.o

all: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES) $(CFLAGS)

clean:
	rm -f *.o core

rebuild: clean all
