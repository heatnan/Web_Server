WebServer: main.o http.o
	cc -o WebServer main.o http.o -lpthread

main.o: main.c http.o
	cc -c main.c -lpthread

http.o: http.c http.h
	cc -c http.c

clean:
	rm WebServer main.o http.o
