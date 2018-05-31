cons: main.o
	gcc -o cons main.o -fsanitize=address 
cppcheck:
	cppcheck -j2 main.c main.h
checkpatch:
	checkpatch.pl --no-tree -f main.c main.h

main.o: main.c main.h
