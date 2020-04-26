#ifndef _order_vector_H__
#define _order_vector_H__

#include <iostream>
#include <string>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <iterator>
#include <algorithm>


#if defined(_DEBUG)
	#define debug_message(str) //printf("--> %s\n", str) 
#else
	#define debug_message(str)
#endif

template <class T1, class T2>
void construct(T1 *pT1, T2 &&rrT2)
{
	new (pT1) T1(std::forward <T2>(rrT2));
}

template <class T>
void destroy(T *pT)
{
	pT->~T();
}

template <class FwdIter>
void destroy(
	FwdIter first, FwdIter last)
{
	while (first != last)
	{
		destroy(&*first);
		++first;
	}
}

template < class T >
class order_vector;

/*!
Implementation template class
*/
template < class T >
class OrdVectorImpl
{
public:
	OrdVectorImpl(size_t capacity = 0);
	OrdVectorImpl(std::initializer_list<T>& lst);
	OrdVectorImpl(OrdVectorImpl<T>&& rImpl);
	OrdVectorImpl(const OrdVectorImpl<T>& rImpl, size_t capacity = 0);
	~OrdVectorImpl();

	void swap(OrdVectorImpl<T>& impl) noexcept;
	OrdVectorImpl<T>* clone();

	size_t size() const { return m_nUsed; }
	size_t capacity() const { return m_capacity; }
	bool is_unreshable() const { return m_bUnreshable; }
	void sort();

	OrdVectorImpl<T>& operator=(const OrdVectorImpl<T>& rImpl);

private:
	friend class order_vector<T>;

	T* m_pBuffer;
	size_t m_capacity;
	size_t m_nUsed;
	bool m_bUnreshable;

};

template < class T >
OrdVectorImpl<T>::OrdVectorImpl(size_t capacity) :
	m_pBuffer(capacity ? static_cast<T*>(operator new(sizeof(T) * capacity)) : nullptr),
	m_capacity(capacity),
	m_nUsed(0),
	m_bUnreshable(false)
{
	debug_message("Impl: constructor default");
}
template < class T >
OrdVectorImpl<T>::OrdVectorImpl(std::initializer_list<T>& lst) :
	m_pBuffer(lst.size() ? static_cast<T*>(operator new(sizeof(T) * lst.size())) : nullptr),
	m_capacity(lst.size()),
	m_nUsed(0),
	m_bUnreshable(false)
{
	debug_message("Impl: constructor std::initializer_list");
	for (auto& node : lst)
	{
		construct(m_pBuffer + m_nUsed, std::move(node));
		++m_nUsed;
	}
}
template < class T >
OrdVectorImpl<T>::OrdVectorImpl(OrdVectorImpl<T>&& rImpl) :
	m_pBuffer(std::move(rImpl.m_pBuffer)),
	m_capacity(std::move(rImpl.m_capacity)),
	m_nUsed(std::move(rImpl.m_nUsed)),
	m_bUnreshable(std::move(rImpl.m_bUnreshable))
{
	debug_message("Impl: move constructor");
}
template < class T >
OrdVectorImpl<T>::OrdVectorImpl(const OrdVectorImpl<T>& rImpl, size_t capacity) :
	m_pBuffer(static_cast<T*>(std::max(capacity, rImpl.capacity()) ? operator new(sizeof(T) * std::max(capacity, rImpl.capacity())) : nullptr)),
	m_capacity(std::max(capacity, rImpl.capacity())),
	m_nUsed(0),
	m_bUnreshable(false)
{
	debug_message("Impl: copy constructor");
	while (m_nUsed < rImpl.m_nUsed)
	{
		construct(m_pBuffer + m_nUsed, *(rImpl.m_pBuffer + m_nUsed));
		++m_nUsed;
	}
}
template < class T >
OrdVectorImpl<T>::~OrdVectorImpl()
{
	debug_message("Impl: ~D");
	::destroy(m_pBuffer, m_pBuffer + m_nUsed);
	operator delete(m_pBuffer);
}

template < class T >
void OrdVectorImpl<T>::swap(OrdVectorImpl<T>& impl) noexcept
{
	if (this != &impl)
	{
		std::swap(this->m_pBuffer, impl.m_pBuffer);
		std::swap(this->m_capacity, impl.m_capacity);
		std::swap(this->m_nUsed, impl.m_nUsed);
		std::swap(this->m_bUnreshable, impl.m_bUnreshable);
	}
}
template < class T >
OrdVectorImpl<T>* OrdVectorImpl<T>::clone()
{
	debug_message("impl: clone");
	return new OrdVectorImpl<T>(*this);
}

template < class T >
OrdVectorImpl<T>& OrdVectorImpl<T>::operator=(const OrdVectorImpl<T>& rImpl)
{
	debug_message("Impl: operator =");
	if (this != &rImpl)
	{
		this->m_pBuffer = rImpl.m_pBuffer;
		this->m_capacity = rImpl.m_capacity;
		this->m_nUsed = rImpl.m_nUsed;
		this->m_bUnreshable = rImpl.m_bUnreshable;
	}
	return *this;
}

template < class T >
void OrdVectorImpl<T>::sort()
{
	std::sort(m_pBuffer, m_pBuffer + m_nUsed);
}


/*!
Order Container template class
*/
template < class T >
class order_vector
{
public:
	class iterator_base;
	using iterator = typename order_vector<T>::iterator_base;
	using const_iterator = typename order_vector<const T>::iterator_base;

	order_vector(size_t capacity = 0);
	order_vector(std::initializer_list<T> lst);
	order_vector(order_vector<T>&& rOVec);
	order_vector(const order_vector<T>& rOVec);
	~order_vector();

	iterator find(const T& other);
	const_iterator find(const T& other) const;
	void insert(T&& element);
	void insert(const T& element);
	void erase(iterator& iter);
	void clear() noexcept;

	bool is_unreshable() const;
	size_t size() const;
	size_t size_refs() const;
	size_t capacity() const;

	order_vector<T>& operator=(order_vector<T>&& rOVec) noexcept;
	order_vector<T>& operator=(const order_vector<T>& rOVec) noexcept;
	T& operator[](size_t i);

private:
	std::shared_ptr<OrdVectorImpl<T>> m_impl;

	void copyOrMove(T&& element);

public:

	iterator begin();
	const_iterator cbegin() const;
	iterator end();
	const_iterator cend() const;
	
	class iterator_base
	{
	public:
		explicit iterator_base(T* pT);
		iterator_base(const iterator_base& rIt);

		bool operator==(const iterator_base& other) const;
		bool operator!=(const iterator_base& other) const;

		// Get value
		T &operator*();
		const T &operator*() const;
		T* operator->();

		iterator_base operator+(int n) const;
		iterator_base operator-(int n) const;
		iterator_base& operator++();
		iterator_base operator++(int);
		iterator_base& operator--();
		iterator_base operator--(int);

	private:
		T* m_pT;

	};
	
};

template < class T >
order_vector<T>::order_vector(size_t capacity = 0) :
	m_impl(std::make_shared<OrdVectorImpl<T>>(capacity))
{
	debug_message("order_vector: constructor default");
}
template < class T >
order_vector<T>::order_vector(std::initializer_list<T> lst) :
	m_impl(std::make_shared<OrdVectorImpl<T>>(lst))
{
	debug_message("order_vector: constructor std::initializer_list");
	m_impl->sort();
}
template < class T >
order_vector<T>::order_vector(const order_vector<T>& rOVec)
{
	debug_message("order_vector: copy constructor");
	if (rOVec.is_unreshable())
		m_impl = std::make_shared<OrdVectorImpl<T>>(*rOVec.m_impl);
	else
		m_impl = rOVec.m_impl;
	debug_message(("m_impl refs: " + std::to_string(m_impl.use_count())).c_str());
}
template < class T >
order_vector<T>::order_vector(order_vector<T>&& rOVec)
{
	debug_message("order_vector: constructor rval");
	m_impl->swap(*rOVec.m_impl);
}
template < class T >
order_vector<T>::~order_vector()
{
	debug_message("order_vector: ~D");
}

template < class T >
typename order_vector<T>::iterator order_vector<T>::find(const T& other)
{
	for (auto it = begin(); it != end(); ++it)
		if (*it == other)
		{
			if (m_impl.use_count() > 1)
			{
				std::ptrdiff_t diff = &*it - m_impl->m_pBuffer;
				OrdVectorImpl<T>* temp = new OrdVectorImpl<T>(*m_impl);
				m_impl.reset(temp);
				it = typename order_vector<T>::iterator(&m_impl->m_pBuffer[diff]);
			}
			m_impl->m_bUnreshable = true;
			return it;
		}
	return end();
}
template < class T >
typename order_vector<T>::const_iterator order_vector<T>::find(const T& other) const
{
	for (auto it = cbegin(); it != cend(); ++it)
		if (*it == other)
			return it;
	return cend();
}

template < class T >
void order_vector<T>::insert(T&& element)
{
	if (m_impl->capacity() < m_impl->m_nUsed + 1 || m_impl.use_count() > 1)
	{
		size_t capacity_new = m_impl->size() + 1 > m_impl->capacity() ? m_impl->size() + 1 : m_impl->capacity();
		OrdVectorImpl<T>* temp = new OrdVectorImpl<T>(*m_impl, capacity_new);
		construct(temp->m_pBuffer + temp->size(), std::forward<T>(element));
		++temp->m_nUsed;
		m_impl.reset(temp);
	}
	else
	{
		construct(m_impl->m_pBuffer + m_impl->size(), std::forward<T>(element));
		++m_impl->m_nUsed;
	}
	m_impl->sort();
}
template < class T >
void order_vector<T>::insert(const T& element)
{
	if (m_impl->capacity() < m_impl->m_nUsed + 1 || m_impl.use_count() > 1)
	{
		size_t capacity_new = m_impl->size() + 1 > m_impl->capacity() ? m_impl->size() + 1 : m_impl->capacity();
		OrdVectorImpl<T>* temp = new OrdVectorImpl<T>(*m_impl, capacity_new);
		construct(temp->m_pBuffer + temp->size(), std::forward<T>(element));
		++temp->m_nUsed;
		m_impl.reset(temp);
	}
	else
	{
		construct(m_impl->m_pBuffer + m_impl->size(), element);
		++m_impl->m_nUsed;
	}
	m_impl->sort();
}
template < class T >
void order_vector<T>::erase(iterator& iter)
{
	std::ptrdiff_t shift = &*iter - m_impl->m_pBuffer;
	if (m_impl.use_count() > 1)
	{
		OrdVectorImpl<T>* temp = new OrdVectorImpl<T>(*m_impl);
		m_impl.reset(temp);
	}

	std::swap(m_impl->m_pBuffer[shift], m_impl->m_pBuffer[m_impl->m_nUsed - 1]);
	--m_impl->m_nUsed;
	m_impl->sort();
}
template < class T >
void order_vector<T>::clear() noexcept
{
	m_impl.reset();
}

template < class T >
bool order_vector<T>::is_unreshable() const { return m_impl->m_bUnreshable; }
template < class T >
size_t order_vector<T>::size() const { return m_impl->m_nUsed; }
template < class T >
size_t order_vector<T>::size_refs() const { return m_impl.use_count(); }
template < class T >
size_t order_vector<T>::capacity() const { return m_impl->capacity(); }

template < class T >
order_vector<T>& order_vector<T>::operator=(order_vector<T>&& rOVec) noexcept
{
	debug_message("order_vector: operator= (move)");
	if (this != &rOVec)
	{
		if (m_impl.use_count() > 1)
		{
			OrdVectorImpl<T>* temp = new OrdVectorImpl<T>();
			m_impl.reset(temp);
		}
		m_impl->swap(*rOVec.m_impl);
	}
	return *this;
}
template < class T >
order_vector<T>& order_vector<T>::operator=(const order_vector<T>& rOVec) noexcept
{
	debug_message("order_vector: operator =");
	if (this != &rOVec)
	{
		if (rOVec.m_impl->is_unreshable())
			m_impl = std::make_shared<OrdVectorImpl<T>>(*rOVec.m_impl);
		else
			m_impl = rOVec.m_impl;
	}
	return *this;
}
template < class T >
T& order_vector<T>::operator[](size_t i)
{
	if (m_impl.use_count() > 1)
	{
		OrdVectorImpl<T>* temp = new OrdVectorImpl<T>(*m_impl);
		m_impl.reset(temp);
	}
	m_impl->m_bUnreshable = true;
	return m_impl->m_pBuffer[i];
}


template < class T >
typename order_vector<T>::iterator order_vector<T>::begin() 
{
	if (m_impl.use_count() > 1)
	{
		OrdVectorImpl<T>* temp = new OrdVectorImpl<T>(*m_impl);
		m_impl.reset(temp);
	}
	m_impl->m_bUnreshable = true;
	return order_vector<T>::iterator(&m_impl->m_pBuffer[0]);
}

template < class T >
typename order_vector<T>::const_iterator order_vector<T>::cbegin() const
{
	return order_vector<T>::const_iterator(&m_impl->m_pBuffer[0]);
}

template < class T >
typename order_vector<T>::iterator order_vector<T>::end()
{
	if (m_impl.use_count() > 1)
	{
		OrdVectorImpl<T>* temp = new OrdVectorImpl<T>(*m_impl);
		m_impl.reset(temp);
	}
	m_impl->m_bUnreshable = true;
	return order_vector<T>::iterator(&m_impl->m_pBuffer[0] + m_impl->m_nUsed);
}

template < class T >
typename order_vector<T>::const_iterator order_vector<T>::cend() const
{
	return order_vector<T>::const_iterator(&m_impl->m_pBuffer[0] + m_impl->m_nUsed);
}


/*
iterator_base
*/
template < class T >
order_vector<T>::iterator_base::iterator_base(T* pT) :
	m_pT(pT)
{}
template < class T >
order_vector<T>::iterator_base::iterator_base(const iterator_base& rIt) :
	m_pT(rIt.m_pT)
{}

template < class T >
bool order_vector<T>::iterator::operator==(const iterator_base& other) const
{
	return m_pT == other.m_pT;
}
template < class T >
bool order_vector<T>::iterator::operator!=(const iterator_base& other) const
{
	return m_pT != other.m_pT;
}
template < class T >
T& order_vector<T>::iterator::operator*()
{
	return *m_pT;
}
template < class T >
const T& order_vector<T>::iterator::operator*() const
{
	return *m_pT;
}
template < class T >
T* order_vector<T>::iterator::operator->()
{
	return m_pT;
}

template < class T >
typename order_vector<T>::iterator_base order_vector<T>::iterator::operator+(int n) const
{
	return iterator_base(m_pT + n);
}
template < class T >
typename order_vector<T>::iterator_base order_vector<T>::iterator::operator-(int n) const
{
	return iterator_base(m_pT - n);
}
template < class T >
typename order_vector<T>::iterator_base& order_vector<T>::iterator::operator++()
{
	++this->m_pT;
	return *this;
}
template < class T >
typename order_vector<T>::iterator_base order_vector<T>::iterator::operator++(int)
{
	iterator_base tmp(m_pT++);
	return tmp;
}
template < class T >
typename order_vector<T>::iterator_base& order_vector<T>::iterator::operator--()
{
	this->m_pT--;
	return *this;
}
template < class T >
typename order_vector<T>::iterator_base order_vector<T>::iterator::operator--(int)
{
	iterator_base tmp(m_pT--);
	return tmp;
}

#endif // !_order_vector_H__