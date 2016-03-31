#include "catch.hpp"
#include <thread>
#include <queue>
#include "NGE/Core/ConcurrentQueue.hpp"
using namespace NGE::Core;

SCENARIO("Push and pop elements from ConcurrentQueue", "[concurrent-queue]") {

	GIVEN("Empty queue") {
		ConcurrentQueue<int> queue;
		std::srand(std::time(0));

		WHEN("Some random number of elements are pushed") {
			size_t numOfElements = std::rand() % 50 + 1;

			for (size_t i = 0; i < numOfElements; ++i) {
				queue.Push(i);
			}

			THEN("All elements are in the queue") {
				REQUIRE_FALSE(queue.Empty());
				REQUIRE(queue.Size() == numOfElements);
			}
		}

		WHEN("Element is pushed and popped") {
			int pushValue = 128;
			queue.Push(pushValue);

			THEN("Size of the queue is 1") {
				REQUIRE(queue.Size() == 1);
			}

			THEN("Popped value is the same as pushed") {
				int poppedValue = -1;
				REQUIRE(queue.TryPop(poppedValue));
				REQUIRE(poppedValue == pushValue);

				AND_THEN("Size of the queue is 0") {
					REQUIRE(queue.Size() == 0);
					REQUIRE(queue.Empty());
				}
			}
		}

		WHEN("Multiple threads push values") {
			int numThreads = rand() % 15 + 5;
			int numOfElements = rand() % 2000 + 1;
			std::vector<std::thread> threads(numThreads);
			for (std::thread& t : threads) {
				t = std::thread([&queue, &numOfElements]() {
					for (int i = 0; i < numOfElements; ++i) {
						queue.Push(i);
					}
				});
			}

			for (std::thread& t : threads) {
				t.join();
			}

			THEN("Queue contains all elements") {
				REQUIRE(queue.Size() == (numOfElements * numThreads));
			}
		}
	}
}

SCENARIO("Clear queue", "[concurrent-queue]") {

	GIVEN("Queue with random number of elements") {
		ConcurrentQueue<int> queue;
		std::srand(std::time(0));
		size_t numOfElements = rand() % 1000 + 1;
		for (size_t i = 0; i < numOfElements; ++i) {
			queue.Push(i);
		}

		WHEN("Queue is cleared") {
			queue.Clear();

			THEN("Queue is empty") {
				REQUIRE(queue.Empty());
				REQUIRE(queue.Size() == 0);
			}
		}
	}
}

SCENARIO("Drain elements from one queue to another", "[concurrent-queue]") {

	GIVEN("ConcurrentQueue with some elements") {
		ConcurrentQueue<int> sourceQueue;
		std::srand(std::time(0));
		size_t numOfElements = rand() % 1000 + 1;
		for (size_t i = 0; i < numOfElements; ++i) {
			sourceQueue.Push(i);
		}

		WHEN("Elements are draind to another ConcurrentQueue") {
			ConcurrentQueue<int> destQueue;
			REQUIRE(sourceQueue.DrainTo(destQueue) == numOfElements);

			THEN("All elements from source queue are in dest queue") {
				REQUIRE(sourceQueue.Empty());
				REQUIRE(destQueue.Size() == numOfElements);
			}
		}

		WHEN("Elements are drained to standard queue") {
			std::queue<int> destQueue;
			REQUIRE(sourceQueue.DrainTo(destQueue) == numOfElements);

			THEN("All elements from source queue are in dest queue") {
				REQUIRE(sourceQueue.Empty());
				REQUIRE(destQueue.size() == numOfElements);
			}
		}
	}
}
