#include "folly/RWSpinLock.h"
#include "thread_pool.h"

#include <shared_mutex>
#include <chrono>
#include <iostream>
#include <future>
#include <array>


unsigned long long g_data;
std::shared_timed_mutex g_std_rw_mtx;
folly::RWSpinLock g_folly_rw_mtx;
const int g_number_of_runs = 10;

void read_std()
{
	std::shared_lock<std::shared_timed_mutex> read_lock(g_std_rw_mtx);
	auto b = g_data;
	++b;
}

void write_std()
{
	std::unique_lock<std::shared_timed_mutex> write_lock(g_std_rw_mtx);
	++g_data;
}

void read_folly()
{
	g_folly_rw_mtx.lock_shared();
	auto b = g_data;
	++b;
	g_folly_rw_mtx.unlock_shared();
}

void write_folly()
{
	g_folly_rw_mtx.lock();
	++g_data;
	g_folly_rw_mtx.unlock();
}

template <typename T, typename F>
long long unsigned int calls_per_second(const T &thereadcall, const F &thewritecall, int interval, int ratio)
{
	auto now = []() -> auto {
		return ::std::chrono::high_resolution_clock::now();
	};
	typedef ::std::chrono::duration<long double> timediff_t;
	unsigned long long count = 0;
	const auto start = now();

	long double curtime_as_ldbl = (timediff_t(now() - start)).count();
	while (curtime_as_ldbl < interval) {
		if (count % ratio == 0)
		{
			thewritecall();
		}
		else
		{
			thewritecall();
		}
		++count;
		curtime_as_ldbl = (timediff_t(now() - start)).count();
	}
	return count / interval;
}

long long unsigned int get_avg(const std::array<long long unsigned int,10> &array_)
{
	long long unsigned int sum = 0;
	for (int i = 0; i <10; ++i)
	{
		sum += array_[i];
	}
	return sum / 10;
}


int main()
{

	std::array<long long unsigned int, 10> array_;

	for (int i = 0; i < 10; ++i)
	{
		g_data = 0;
		ThreadPool thread_pool(4);
		int ratio = 20;
		array_[i] = calls_per_second([&thread_pool]() { auto result = thread_pool.enqueue(&read_std); result.wait(); },
			[&thread_pool]() { auto result = thread_pool.enqueue(&write_std); result.wait(); },
			g_number_of_runs, ratio);
	}
	std::cout << "calls per second: " << get_avg(array_) << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		g_data = 0;
		ThreadPool thread_pool(4);
		int ratio = 10;
		array_[i] = calls_per_second([&thread_pool]() { auto result = thread_pool.enqueue(&read_std); result.wait(); },
			[&thread_pool]() { auto result = thread_pool.enqueue(&write_std); result.wait(); },
			g_number_of_runs, ratio);
	}
	std::cout << "calls per second: " << get_avg(array_) << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		g_data = 0;
		ThreadPool thread_pool(4);
		int ratio = 5;
		array_[i] = calls_per_second([&thread_pool]() { auto result = thread_pool.enqueue(&read_std); result.wait(); },
			[&thread_pool]() { auto result = thread_pool.enqueue(&write_std); result.wait(); },
			g_number_of_runs, ratio);
	}
	std::cout << "calls per second: " << get_avg(array_) << std::endl;
		///////////////////////////
	for (int i = 0; i < 10; ++i)
	{
		g_data = 0;
		ThreadPool thread_pool(4);
		int ratio = 20;
		array_[i] = calls_per_second([&thread_pool]() { auto result = thread_pool.enqueue(&read_folly); result.wait(); },
			[&thread_pool]() { auto result = thread_pool.enqueue(&write_folly); result.wait(); },
			g_number_of_runs, ratio);
	}
	std::cout << "calls per second: " << get_avg(array_) << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		g_data = 0;
		ThreadPool thread_pool(4);
		int ratio = 10;
		array_[i] = calls_per_second([&thread_pool]() { auto result = thread_pool.enqueue(&read_folly); result.wait(); },
			[&thread_pool]() { auto result = thread_pool.enqueue(&write_folly); result.wait(); },
			g_number_of_runs, ratio);
	}
	std::cout << "calls per second: " << get_avg(array_) << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		g_data = 0;
		ThreadPool thread_pool(4);
		int ratio = 5;
		array_[i] = calls_per_second([&thread_pool]() { auto result = thread_pool.enqueue(&read_folly); result.wait(); },
			[&thread_pool]() { auto result = thread_pool.enqueue(&write_folly); result.wait(); },
			g_number_of_runs, ratio);
	}
	std::cout << "calls per second: " << get_avg(array_) << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}