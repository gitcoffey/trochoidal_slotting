OBJS = trochoidal_slot.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)


all: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o trochoidal_slot

trochoidal_slot.o : trochoidal_slot.cpp
		$(CC) $(CFLAGS) trochoidal_slot.cpp -lm

clean:
	rm *.o *~ trochoidal_slot


