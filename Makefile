# Makefile

run_Assignment1 : Assignment1
	./Assignment1

Assignment1 : Assignment1.o
	gcc -o Assignment1 Assignment1.o

Assignment1.o : Assignment1.c
	gcc -c Assignment1.c

	gcc -Og -c while.c

clean :
	rm -rf *.o Assignment1 .*sw* a.out