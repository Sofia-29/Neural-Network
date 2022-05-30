all:
	g++ -c -g -I/usr/include/python3.8/ -no-pie -o main.o main.cpp
	g++ -g -no-pie -o main main.o -lpython3.8 -lm

clean:
	rm *.o 