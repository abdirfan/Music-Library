build:
	g++ -g -std=c++11 -Wall musiclibrary.cpp -o program.exe

clean:
	rm -f program.exe

run:
	./program.exe

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./program.exe
