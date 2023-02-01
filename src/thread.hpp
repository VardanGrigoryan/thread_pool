/*
 * Author: Vardan Grigoryan
 * Date: 02/01/2023
 */

#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <memory>
#include <thread>

#include "job.hpp"
#include "queue.hpp"

template <typename R, typename...T>
class thread {
	public:
		thread(): m_queue(std::make_unique<queue<R, T...>>()) {
			m_uuid = boost::uuids::random_generator()();
		}
		
		void push_job(const job<R, T...>& j) {
			m_queue->push(j);
		}
		
		void push_job(job<R, T...>&& j) {
			m_queue->push(std::move(j));
		}

		void stop(bool status) {
			has_to_be_stoped = status;
		}

		std::thread& start() {
			m_thread = std::thread([this]() {
					while(!has_to_be_stoped || !m_queue->empty()) {
					     if(!m_queue->empty()) {
						     auto job = m_queue->pop();
						     if(job) {
							 m_result.push_back(job->callback());
					     } else if(m_queue->empty() && has_to_be_stoped) {
						     break;
					     }
					     }
					}
					});
			return m_thread;
		}

		void wait() {
			if(m_thread.joinable()) {
				m_thread.join();
			}
		}

		 const std::vector<R>& result() const {
			return m_result;
		}
		
		thread(const thread<R, T...>&) = delete;
		
		thread& operator=(const thread<R, T...>&) = delete;
		
		~thread() {
			m_queue.reset();
		}
	private:
		std::unique_ptr<queue<R, T...>> m_queue{};
		std::thread m_thread{};
		bool has_to_be_stoped{false};
		boost::uuids::uuid m_uuid{};
		std::vector<R> m_result{};
};

#endif // __THREAD_HPP__

