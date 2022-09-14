CC = g++

CFLAGS = -g -Wall

TARGETS = RatingDistance MovieRate
 
all: RatingDistance MovieRate

RatingDistance: RatingDistance.cpp
	$(CC) $(CFLAGS) -o RatingDistance RatingDistance.cpp

MovieRate: MovieRate.cpp
	$(CC) $(CFLAGS) -o MovieRate MovieRate.cpp

clean:
	$(RM) $(TARGETS)