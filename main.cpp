#include <iostream>

#include "buffer.cpp"

using namespace std;

int main(int argc, char** argv) {
	Buffer buffer;

	cout << buffer.isEmpty() << endl;

	buffer.put(1);
	
	cout << buffer.isEmpty() << endl;
	
	return 0;
}
