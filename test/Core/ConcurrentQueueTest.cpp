#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "NGE/Core/ConcurrentQueue.hpp"
using namespace NGE::Core;

BOOST_AUTO_TEST_CASE(CreateQueue) {
    ConcurrentQueue<int> queue;

    queue.Push(10);
    queue.Push(20);
    queue.Push(30);

    BOOST_CHECK_EQUAL(queue.Empty(), false);
    BOOST_CHECK_EQUAL(queue.Size(), 3);
}

BOOST_AUTO_TEST_CASE(PushAndPopElement) {
    ConcurrentQueue<int> queue;
    queue.Push(10);
    BOOST_CHECK_EQUAL(queue.Size(), 1);

    int poppedValue = -1;
    bool result = queue.TryPop(poppedValue);

    BOOST_CHECK(result == true && poppedValue == 10);
    BOOST_CHECK_EQUAL(queue.Empty(), true);
}

BOOST_AUTO_TEST_CASE(PushAndPopMultipleElements) {
    ConcurrentQueue<int> queue;
    for (int i = 0; i < 1000; ++i) {
        queue.Push(i);
    }

    BOOST_CHECK_EQUAL(queue.Size(), 1000);

    std::array<int, 1000> result;

    bool popResult = false;
    for (int i = 0; i < 1000; ++i) {
        popResult = queue.TryPop(result[i]);
        BOOST_CHECK_EQUAL(popResult, true);
    }

    for (int i = 0; i < 1000; ++i) {
        BOOST_CHECK_EQUAL(result[i], i);
    }
}

void pushData(ConcurrentQueue<int>& queue) {
    for (int i = 0; i < 1000; ++i) {
        queue.Push(i);
    }
}

BOOST_AUTO_TEST_CASE(PushFromMultipleThreads) {
    ConcurrentQueue<int> queue;

    const int numThreads = 10;
    std::array<std::thread, numThreads> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(pushData, std::ref(queue));
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    BOOST_CHECK_EQUAL(queue.Size(), 1000 * numThreads);
}

BOOST_AUTO_TEST_CASE(ClearQueue) {
    ConcurrentQueue<int> queue;

    for (int i = 0; i < 1000; ++i) {
        queue.Push(i);
    }

    BOOST_CHECK_EQUAL(1000, queue.Size());

    queue.Clear();

    BOOST_CHECK_EQUAL(0, queue.Size());
}

BOOST_AUTO_TEST_CASE(DrainToAnotherConcurrentQueue) {
    ConcurrentQueue<int> sourceQueue;
    ConcurrentQueue<int> destQueue;

    for (int i = 0; i < 1000; ++i) {
        sourceQueue.Push(i);
    }

    BOOST_CHECK_EQUAL(1000, sourceQueue.Size());
    BOOST_CHECK_EQUAL(0, destQueue.Size());

    BOOST_CHECK_EQUAL(1000, sourceQueue.DrainTo(destQueue));

    BOOST_CHECK_EQUAL(0, sourceQueue.Size());
    BOOST_CHECK_EQUAL(1000, destQueue.Size());
}

BOOST_AUTO_TEST_CASE(DrainToAnotherQueue) {
    ConcurrentQueue<int> sourceQueue;
    std::queue<int> destQueue;

    for (int i = 0; i < 1000; ++i) {
        sourceQueue.Push(i);
    }

    BOOST_CHECK_EQUAL(1000, sourceQueue.Size());
    BOOST_CHECK_EQUAL(0, destQueue.size());

    BOOST_CHECK_EQUAL(1000, sourceQueue.DrainTo(destQueue));

    BOOST_CHECK_EQUAL(0, sourceQueue.Size());
    BOOST_CHECK_EQUAL(1000, destQueue.size());
}