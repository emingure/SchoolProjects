all: main

main: main.o
	@g++ -o main main.o

main.o: main.cpp
	@g++ -std=c++11 -c main.cpp -o main.o
	
run: main
	@./main ${ARGS}
	
clean:
	@rm main *.o
