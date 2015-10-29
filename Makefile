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

test: all
	./trochoidal_slot -w .625 -t .25 -b 1,1 -e 0,0 -f 15 -r 60 -s .020 -z -.75 -x .25


