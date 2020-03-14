build:
	gcc main.c -o fsminishell -Wall -lm
run:
	valgrind ./fsminishell