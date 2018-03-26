demo : main.o MetroImplementation.o
	gcc -o demo main.o MetroImplementation.o

main.o : main.c MetroInterface.h DataTypes.h
	gcc -c main.c

MetroImplemetation.o : MetroImplementation.c DataTypes.h
	gcc -c MetroImplementation.c

clean :
	rm demo main.o MetroImplementation.o
