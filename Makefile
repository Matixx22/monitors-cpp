all:
	g++ -o program main.cpp buffer.cpp monitor.h -lrt -pthread
