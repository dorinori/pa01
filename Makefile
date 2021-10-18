.PHONY:main
all:main

main:
	g++ -std=c++11 -g *.cpp -o prog1

clean:
	/bin/rm -rf *.o prog1