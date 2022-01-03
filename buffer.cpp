#include <stdio.h>
#include <queue>
#include <pthread.h>
#include <iostream>

#include "monitor.h"

class Buffer: public Monitor {
	private:
		Condition notEmpty;
		Condition notFull;
		std::queue<int> queue;
	
	public:
		int capacity = 10;

		Buffer() {}

		void put(int item) {
			enter();

			if (queue.size() == capacity)
				wait(notFull);

			queue.push(item);

			if (queue.size() == 1)
				signal(notEmpty);

			leave();
		}

		int get() {
			enter();

			if (queue.size() == 0)
				wait(notEmpty);

			int item = queue.front();
			queue.pop();

			if (queue.size() == capacity-1)
				signal(notFull);

			leave();
			return item;
		}

		bool isFull() {
			return (queue.size() == capacity);
		}

		bool isEmpty() {
			return (queue.empty());
		}
};
