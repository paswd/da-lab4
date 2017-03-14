FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -std=c++11 -O2
COMPILLER=g++

#all: lib start
all: start

#start: main.o
#	$(COMPILLER) $(FLAGS) -o da-lab4 main.o -L. lib/lib-z-search.a

start: converter.o z-search.o main.o
	$(COMPILLER) $(FLAGS) -o da-lab4 converter.o z-search.o main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

converter.o: converter.cpp
	$(COMPILLER) -c $(FLAGS) converter.cpp

z-search.o: z-search.cpp
	$(COMPILLER) -c $(FLAGS) z-search.cpp

clear:
	-rm -f *.o *.gch *.dat da-lab4
	#rm lib/lib-z-search.a

lib: z-search.o
	ar rc lib/lib-z-search.a z-search.o
	ranlib lib/lib-z-search.a
