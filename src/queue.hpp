/*
 * Author: Vardan Grigoryan
 * Date: 02/01/2023
 */

#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "job.hpp"

template <typename R, typename...T>
class queue
{
public:
  queue() {}
  
  void push(job<R, T...>&& j)
  {
    std::unique_ptr<job<R, T...>> value(std::make_unique<job<R, T...>>(std::move(j)));
    std::lock_guard<std::mutex> lck(m_mtx);
    m_queue.push(std::move(value));
    m_cnd.notify_one();
  }
  
  void push(const job<R, T...>& j)
  {
    std::unique_ptr<job<R, T...>> value(std::make_unique<job<R, T...>>(j));
    std::lock_guard<std::mutex> lck(m_mtx);
    m_queue.push(std::move(value));
    m_cnd.notify_one();
  }
  
  std::unique_ptr<job<R, T...>> pop()
  {
    std::unique_lock<std::mutex> lck(m_mtx);
    if(m_queue.empty()) {
	    return nullptr;
    }
    m_cnd.wait(lck, [&]{ return !m_queue.empty();});
    std::unique_ptr<job<R, T...>> val(std::move(m_queue.front()));
    m_queue.pop();
    return val;
  }
  
  bool empty() const
  {
	  std::lock_guard<std::mutex> lck(m_mtx);
	  return m_queue.empty();
  }
  
  size_t size() const
  {
	  std::lock_guard<std::mutex> lck(m_mtx);
	  return m_queue.size();
  }
  
  queue(const queue<R, T...>&) = delete;

  queue& operator=(const queue<R, T...>&) = delete;

private:
  std::condition_variable m_cnd;
  mutable std::mutex m_mtx;
  std::queue<std::unique_ptr<job<R, T...>>> m_queue;
};

#endif // __QUEUE_HPP__
