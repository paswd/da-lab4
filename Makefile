FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -std=c++11
COMPILLER=g++

#all: lib start
all: start

#start: main.o
#	$(COMPILLER) $(FLAGS) -o da-lab2 main.o -L. lib/lib-z-search.a

start: main.o
	$(COMPILLER) $(FLAGS) -o da-lab4 main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp


z-search.o: z-search.cpp
	$(COMPILLER) -c $(FLAGS) z-search.cpp

clear:
	-rm -f *.o *.gch *.dat da-lab4
	#rm lib/lib-z-search.a

lib: z-search.o
	ar rc lib/lib-z-search.a z-search.o
	ranlib lib/lib-z-search.a
