CC=gcc
CFLAGS=-I -Wall -O2

all:  tree extras functions child main

main: tree.o extras.o functions.o
	$(CC)  $(CFLAGS) -o diseaseAggregator main.c tree.o extras.o functions.o child.o

tree: tree.c tree.h
	$(CC) -c tree.c $(CFLAGS) 

extras: extras.c extras.h
	$(CC) -c extras.c $(CFLAGS) 

functions: functions.c  functions.h
	$(CC) -c functions.c $(CFLAGS) 

child: child.c  child.h
	$(CC) -c child.c $(CFLAGS) 


clean: 
	$(RM) myexe *.o *~
