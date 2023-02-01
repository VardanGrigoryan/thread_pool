/*
 * Author: Vardan Grigoryan
 * Date: 02/01/2023
 */

#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <memory>
#include <vector>

#include "thread.hpp"
#include "typedef.hpp"

template <typename R, typename...T>
class thread_pool {
	public:
		thread_pool() {}
		
		thread_pool(size_t size, rcb&& cb): m_size(size), 
		                                    m_cb(std::move(cb)) {
			m_threads.reserve(m_size);
		}

		void create_pool() {
			std::generate_n(std::back_inserter(m_threads), m_size, []{ return std::make_unique<thread<R, T...>>();});
			for(auto& thread: m_threads) {
				thread->start();
			}
		}

		void create_thread() {
			std::unique_ptr<thread<R, T...>> thread(std::make_unique<thread<R, T...>>());
			m_threads.push_back(std::move(thread));
		}

		void push_job(job<R, T...>&& j, size_t index) {
			m_threads[index]->push_job(std::move(j));
		}
		
		void push_job(const job<R, T...>& j, size_t index) {
			m_threads[index]->push_job(j);
		}

		void shut_down() {
			for(auto& t: m_threads) {
				t->stop(true);
			}
		}

		size_t size() const {
			return m_threads.size();
		}

		bool empty() const {
			return m_threads.empty();
		}

		~thread_pool() {
			for(auto& thread: m_threads) {
				thread->wait();
			}
			for(auto& t: m_threads) {
				auto res = t->result();
				for(auto& r: res) {
					m_cb(r);
				}
			}
			m_threads.erase(m_threads.begin(), m_threads.end());
		}

	private:
		size_t m_size{0};
		std::vector<std::unique_ptr<thread<R, T...>>> m_threads{};
		rcb m_cb;
};

#endif // __THREAD_POOL_HPP__
