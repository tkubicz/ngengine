/* 
 * File:   ConcurrentQueue.hpp
 * Author: tku
 *
 * Created on 7 września 2015, 13:23
 */

#ifndef CONCURRENTQUEUE_HPP
#define	CONCURRENTQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace NGE {
	namespace Core {

		template <typename T> class ConcurrentQueue {
		  private:
			std::queue<T> queue;
			std::condition_variable condition;
			std::mutex mutex;

		  public:

			ConcurrentQueue() { }

			void Push(T const& data) {
				std::unique_lock<std::mutex> lock(mutex);
				queue.push(data);
				lock.unlock();
				condition.notify_one();
			}

			bool Empty() {
				std::unique_lock<std::mutex> lock(mutex);
				return queue.empty();
			}

			long unsigned int Size() {
				std::unique_lock<std::mutex> lock(mutex);
				return queue.size();
			}

			bool TryPop(T& poppedValue) {
				std::unique_lock<std::mutex> lock(mutex);
				if (queue.empty()) {
					return false;
				}

				poppedValue = queue.front();
				queue.pop();
				return true;
			}

			void WaitAndPop(T& poppedValue) {
				std::unique_lock<std::mutex> lock(mutex);
				while (queue.empty()) {
					condition.wait(lock);
				}
				poppedValue = queue.front();
				queue.pop();
			}
		};
	}
}

#endif	/* CONCURRENTQUEUE_HPP */

