all: virusDetector

virusDetector: virusDetector.o 
	gcc -g -m32 -Wall -o virusDetector virusDetector.o

virusDetector.o : task1c.c
	gcc -g -Wall -m32  -c -o virusDetector.o  task1c.c
	
.PHONY: clean

clean: 
	rm -f *.o virusDetector

