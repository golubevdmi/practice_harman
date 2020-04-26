#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <cassert>


#define print_array 0

const unsigned int nThread = std::thread::hardware_concurrency();
size_t szArray = 1;


template < class fwdIter >
struct ArrayReference
{
	fwdIter first;
	fwdIter last;

	ArrayReference(fwdIter first, fwdIter last) :
		first(first), last(last) {}
};

using Type = int;
using ReferenceType = std::vector<Type>::iterator;
std::queue<ArrayReference<ReferenceType>> g_queue;
std::mutex g_mutex;
std::atomic<bool> g_bRun(false);
std::atomic<int> nWorkThread = 0;
std::condition_variable g_condNotEmpty;


template < class T >
void output(const std::vector<T>& arr)
{
	for (const auto& val : arr)
		std::cout << " " << val;
}
// From cppreference example
template < class fwdIter >
void quick_sort(fwdIter first, fwdIter last);
void processing(size_t threadNumber);


int main(int argc, const char* argv[])
{
	srand(time(NULL));

	for (size_t i = 0; i < 9; i++, szArray *= 10)
	{
		std::cout << "Size: " << szArray << std::endl;

		clock_t begin = 0, end = 0;
		std::vector<Type> arr1, arr2, arr3;
		for (size_t i = 0; i < szArray; i++)
			arr1.push_back(rand() % 100);
		arr3 = arr2 = arr1;

		// Show source arrays
#if print_array
		std::cout << "Source array:\n"; output(arr1); std::cout << std::endl;
#endif //  print_array

		// Parallel quick sort
		begin = clock();
		g_bRun = true;
		while (!g_queue.empty())
			g_queue.pop();
		g_queue.push(ArrayReference<ReferenceType>(arr1.begin(), arr1.end()));
		std::vector<std::thread> threads;
		for (size_t i = 0; i < nThread - 2; ++i)
			threads.push_back(std::thread(processing, i + 1));
		std::for_each(threads.begin(), threads.end(), std::mem_fun_ref(&std::thread::join));
		end = clock();
		std::cout 
			<< "Parallel Time: " << (double)(end - begin) / (double)CLOCKS_PER_SEC
			<< ", is sorted: " << (std::is_sorted(arr1.begin(), arr1.end()) ? "Yes" : "No") << "\n";
#if print_array
		std::cout << "Array 1:\n"; output(arr1); std::cout << std::endl;
#endif //  print_array
		arr1.clear();

		// STL quick sort
		begin = clock();
		std::sort(arr2.begin(), arr2.end());
		end = clock();
		std::cout 
			<< "STL Time: " << (double)(end - begin) / (double)CLOCKS_PER_SEC
			<< ", is sorted: " << (std::is_sorted(arr2.begin(), arr2.end()) ? "Yes" : "No") << "\n";
#if print_array
		std::cout << "Array 2:\n"; output(arr2); std::cout << std::endl;
#endif //  print_array
		arr2.clear();

		// Source sort
		begin = clock();
		quick_sort(arr3.begin(), arr3.end());
		end = clock();
		std::cout
			<< "Source Time: " << (double)(end - begin) / (double)CLOCKS_PER_SEC
			<< ", is sorted: " << (std::is_sorted(arr3.begin(), arr3.end()) ? "Yes" : "No") << "\n";
#if print_array
		std::cout << "Array 3:\n"; output(arr3); std::cout << std::endl;
#endif //  print_array
		arr3.clear();

		std::cout << std::endl;
	}
	return 0;
}

void processing(size_t threadNumber)
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(g_mutex);
		g_condNotEmpty.wait(lock, [] { return !g_queue.empty() || !g_bRun; });
		if (g_queue.empty() && !g_bRun)
			break;

		++nWorkThread;
		ArrayReference<ReferenceType> elem = g_queue.front();
		g_queue.pop();
		//std::cout << threadNumber << " - low: " << *elem.first << ", high: " << *elem.last << std::endl;
		lock.unlock();

		if (elem.first != elem.last)
		{
			auto pivot = *std::next(elem.first, std::distance(elem.first, elem.last) / 2);
			auto middle1 = std::partition(elem.first, elem.last,
				[pivot](const auto& em) { return em < pivot; });
			auto middle2 = std::partition(middle1, elem.last,
				[pivot](const auto& em) { return !(pivot < em); });
			g_queue.push(ArrayReference<ReferenceType>(elem.first, middle1));
			g_condNotEmpty.notify_one();
			g_queue.push(ArrayReference<ReferenceType>(middle2, elem.last));
			g_condNotEmpty.notify_one();
		}
		--nWorkThread;
		if (g_queue.empty())
		//if (!nWorkThread)
			g_bRun = false;
	}
}

template < class fwdIter >
void quick_sort(fwdIter first, fwdIter last)
{
	if (first != last)
	{
		auto pivot = *std::next(first, std::distance(first, last) / 2);
		auto middle1 = std::partition(first, last,
			[pivot](const auto& em) { return em < pivot; });
		auto middle2 = std::partition(middle1, last,
			[pivot](const auto& em) { return !(pivot < em); });
		quick_sort(first, middle1);
		quick_sort(middle2, last);
	}
}