main:main.c interface.o book.o 
	gcc mian.c interface.o -o main
	
interface.o:interface.c
	gcc -c interface.c
	
book.o: book.c
	gcc -c book.c
	
clean:
	rm *.o main