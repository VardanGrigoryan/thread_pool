/*
 * Author: Vardan Grigoryan
 * Date: 02/01/2023
 */

#ifndef __BALANCER_HPP__
#define __BALANCER_HPP__

#include "job.hpp"
#include "thread_pool.hpp"
#include "typedef.hpp"

template <typename R, typename...T>
class balancer {
	public:
		balancer(const size_t size, rcb&& cb): 
			m_pool(std::make_unique<thread_pool<R, T...>>(size, std::move(cb))) {
				m_pool->create_pool();
			}
                
		// Round-Robin circular balancer
		void push_job(job<R, T...>&& j) {
			m_pool->push_job(std::move(j), m_index % m_pool->size());
			++m_index;
		}
		
                // Round-Robin circular balancer
		void push_job(const job<R, T...>& j) {
			m_pool->push_job(j, m_index % m_pool->size());
			++m_index;
		}

		void shut_down() {
			m_pool->shut_down();
		}

	private:
		std::unique_ptr<thread_pool<R, T...>> m_pool{};
		size_t m_index{0};
};

#endif // __BALANCER_HPP__
