#include <utility>
#include <functional>
#include <type_traits>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <string>
#include <list>
#include <vector>
#include <set>

#include <order_vector.h>

using namespace std;

struct Test
{
	int m_n;
	int m_nCheck;
	bool m_bThrowInMove, m_bThrowLater;
	static bool ms_bThrowDeferred;
	std::list <double> m_List;
	
	Test(int n, bool bThrowInMove = false, bool bThrowLater = false)
		: m_n(n),
		m_nCheck(1),
		m_bThrowInMove(bThrowInMove),
		m_bThrowLater(bThrowLater),
		m_List(100, 1.)
	{
		cout << "Test(" << setw(3) << n << ")" << endl;
	}
	
	Test(const Test &rcTest)
		: m_n(rcTest.m_n),
		m_nCheck(rcTest.m_nCheck),
		m_bThrowInMove(rcTest.m_bThrowInMove),
		m_bThrowLater(rcTest.m_bThrowLater),
		m_List(rcTest.m_List)
	{
		cout << "Test(const Test &rcTest), rcTest == " << rcTest << endl;
		if (m_bThrowInMove)
		{
			cout << "Throwing..." << endl;
			throw std::logic_error(string("Test(") + to_string(m_n) + ")");
		}
		if (m_bThrowLater && ms_bThrowDeferred)
		{
			cout << "Throwing..." << endl;
			throw std::logic_error(string("Test(Test &&)"));
		}
	}
	
	friend std::ostream &operator << (std::ostream &rStream, const Test &rcTest)
	{
		if (rcTest.m_nCheck == 1)
			rStream << setw(3) << rcTest.m_n;
		else
			rStream << "invalid";
		//
		return rStream;
	}
	
	bool operator<(const Test& rTest) const
	{
		return m_n < rTest.m_n;
	}

	Test &operator = (const Test &) = delete;
	Test &operator = (Test &&rTest)
	{
		if (this != &rTest)
		{
			m_n = std::forward<int>(rTest.m_n);
			m_nCheck = std::forward<int>(rTest.m_nCheck);
			m_bThrowInMove = std::forward<bool>(rTest.m_bThrowInMove);
			m_bThrowLater = std::forward<bool>(rTest.m_bThrowLater);
			m_List = std::forward<std::list<double>>(rTest.m_List);
		}
		return *this;
	}
};
bool Test::ms_bThrowDeferred = false;

template < class T >
void show_order_vector(std::string info, const order_vector<T>& ord_vec);
void test();
void test2();


int main(int argc, const char* argv)
{
	test();
	test2();
	return 0;
}

template < class T >
void show_order_vector(std::string info, const order_vector<T>& ord_vec)
{
	std::cout << "\n"
		<< info
		<< "size: " << ord_vec.size()
		<< ", refs: " << ord_vec.size_refs()
		<< ", capacity: " << ord_vec.capacity()
		<< ", is unreshable: " << (ord_vec.is_unreshable() ? "Yes" : "No")
		<< ", elements: ";
	for (auto it = ord_vec.cbegin(); it != ord_vec.cend(); ++it)
		std::cout << " " << *it;
	std::cout << std::endl;
}


void output(
	order_vector <Test>::iterator begin,
	order_vector <Test>::iterator end)
{
	while (begin != end)
	{
		std::cout << *begin << std::endl;
		++begin;
	}
	cout << endl;
}

void output(
	order_vector <int>::iterator begin,
	order_vector <int>::iterator end)
{
	while (begin != end)
	{
		std::cout << *begin << std::endl;
		++begin;
	}
	cout << endl;
}
void output(order_vector <Test> &rStack)
{
	cout <<
		"Size: " << rStack.size() <<
		", capacity: " << rStack.capacity() <<
		", size_refs: " << rStack.size_refs() <<
		", contents:\n";
	output(rStack.begin(), rStack.end());
}
void output(order_vector <int> &rStack)
{
	cout <<
		"Size: " << rStack.size() <<
		", capacity: " << rStack.capacity() <<
		", size_refs: " << rStack.size_refs() <<
		", contents:\n";
	output(rStack.begin(), rStack.end());
}
void output(const exception &rcException)
{
	cout << "Exception: " << rcException.what() << endl;
}


void test()
{
	std::cout << "\n\n Test 1" << std::endl;
	cout << "\n 1. insert()" << endl;
	order_vector<Test> container;
	int i = 0;
	for (; i < 10; ++i)
		container.insert(Test(i));
	for (; container.size() < container.capacity(); ++i)
		container.insert(Test(i));
	auto begin = container.begin();
	auto end = container.end();
	output(container);
	
	try
	{
		container.insert(Test(999, true));
	}
	catch (const exception &rcException)
	{
		output(rcException);
	}
	
	output(container);
	
	container.insert(Test(i++));
	
	begin = container.begin();
	end = container.end();
	
	output(container);
	
	try
	{
		container.insert(Test(9999, true));
	}
	catch (const exception &rcException)
	{
		output(rcException);
	}
	
	output(container);
	output(begin, end);
	
	container.insert(Test(i++, false, true));
	container.insert(Test(i++));
	
	output(container);

	//
	// 2. Copy constructor
	//
	cout << "\n 2. Copy construction" << endl;
	Test::ms_bThrowDeferred = true;
	try
	{
		order_vector<Test> s2 = container;
	}
	catch (const exception &rcException)
	{
		output(rcException);
	}
	catch (...)
	{
		cout << "catch (...)" << endl;
	}

	//
	// 3. Copy assignment
    //
	cout << "\n 3. Copy assignment" << endl;
	order_vector<Test> s3;
	s3.insert(Test(111));
	begin = s3.begin();
	end = s3.end();
	try
	{
		//s3 = container;
	}
	catch (const exception &rcException)
	{
		output(rcException);
	}
	catch (...)
	{
		cout << "catch (...)" << endl;
	}
	output(s3);
	output(begin, end);
}
void test2()
{
	std::cout << "\n\n Test2" << std::endl;

	order_vector<int> s1{ 1,2,3,4,5 };
	order_vector<int> s2(s1);
	order_vector<int> s3(s2);
	std::cout << "1)" << std::endl;
	std::cout << "s1: "; output(s1);
	std::cout << "s2: "; output(s2);
	std::cout << "s3: "; output(s3);
	auto& rint = s1[0];
	order_vector<int> s4(s1);
	output(s4);
	rint = 2;
	std::cout << "2)" << std::endl;
	std::cout << "s1: "; output(s1);
	std::cout << "s2: "; output(s2);
	std::cout << "s3: "; output(s3);
	std::cout << "s4: "; output(s4);
	s1.insert(6);
	order_vector<int> s5(s1);
	std::cout << "3)" << std::endl;
	std::cout << "s1: "; output(s1);
	std::cout << "s2: "; output(s2);
	std::cout << "s3: "; output(s3);
	std::cout << "s4: "; output(s4);
	std::cout << "s5: "; output(s5);
	s5.insert(7);
	std::cout << "4)" << std::endl;
	std::cout << "s1: "; output(s1);
	std::cout << "s2: "; output(s2);
	std::cout << "s3: "; output(s3);
	std::cout << "s4: "; output(s4);
	std::cout << "s5: "; output(s5);
	s5.insert(8);
	std::cout << "5)" << std::endl;
	std::cout << "s1: "; output(s1);
	std::cout << "s2: "; output(s2);
	std::cout << "s3: "; output(s3);
	std::cout << "s4: "; output(s4);
	std::cout << "s5: "; output(s5);
}