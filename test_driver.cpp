/*
 * Author: Vardan Grigoryan
 * Date: 02/01/2023
 */

#include <iostream>

#include "balancer.hpp"
#include "result.hpp"

using ret_type = result<int>;

ret_type func(int a, int b) {
	ret_type obj{};
	obj.value = a + b;
	return obj;
}

ret_type func2(int a, int b) {
	ret_type obj{};
	obj.value = a - b;
	return obj;
}

ret_type func3(int a, int b) {
	ret_type obj{};
	obj.value = a * b;
	return obj;
}

ret_type func4(int a, int b) {
	ret_type obj{};
	obj.value = a / b;
	return obj;
}

ret_type func5(int a, int b) {
	ret_type obj{};
	obj.value = a * b;
	return obj;
}

class class_obj {
	public:
		class_obj() {}
		
		ret_type operator()(int x, int y) {
			ret_type obj{};
			obj.value = x + y;
			return obj;
		}
};

/*
 * A callback for getting results
 * */
void print(ret_type x) {
	std::cout << x.value << "\n";
}

// Small test driver program
int main() {
	job<result<int>, int, int> j1(std::move(func), 2, 3);
	job<result<int>, int, int> j2(std::move(func2), 2, 3);
	job<result<int>, int, int> j3(std::move(func3), 2, 3);
	job<result<int>, int, int> j4(std::move(func4), 2, 3);
	job<result<int>, int, int> j5(std::move(func5), 2, 3);

	balancer<result<int>, int, int> pool(3, print);
	pool.push_job(std::move(j1));
	pool.push_job(std::move(j2));
	
	class_obj obj{};
	job<result<int>, int, int> j6(std::move(obj), 3, 5);
	
	pool.push_job(std::move(j3));
	pool.push_job(std::move(j4));
	
	pool.push_job(std::move(j5));
	pool.push_job(std::move(j6));

        /*
	 * The work has been done, the threads can be closed.
	 */	
	pool.shut_down();
	return 0;
}
