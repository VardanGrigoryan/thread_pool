/*
 * Author: Vardan Grigoryan
 * Date: 02/01/2023
 */

#ifndef __JOB_HPP__
#define __JOB_HPP__

#include <functional>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

template <typename R, typename...T>
class job {
	public:
		using job_callback = std::function<R(T...)>;
	
	private:
		job_callback m_job{};
		std::tuple<T...> m_args;
		boost::uuids::uuid m_uuid{};

        private:
		template<std::size_t... I>
		R call_with_tuple(const std::tuple<T...>& tuple, std::index_sequence<I...>) {
			return m_job(std::get<I>(m_args)...);
		}
	
	public:
		job(job_callback&& func, T&&...args): m_job(std::move(func)),
		                                      m_args(std::forward<T>(args)...),
                                                      m_uuid(boost::uuids::random_generator()()) {
		}
		
		job(const job_callback& func, const T&...args): m_job(func),
		                                                m_args(std::forward_as_tuple<const T&>(args)...),
                                                                m_uuid(boost::uuids::random_generator()()) {
	        }


		R callback() {
			return call_with_tuple(m_args, std::index_sequence_for<T...>());
		}

		boost::uuids::uuid get_uuid() const {
			return m_uuid;
		}

		job(job<R, T...>&& other): m_job(std::move(other.m_job)),
                                           m_args(std::forward<std::tuple<T...>>(other.m_args)),
		                           m_uuid(std::move(other.m_uuid)) { 
		}
		
		job& operator=(job<R, T...>&&) = default;
};

#endif // __JOB_HPP__
