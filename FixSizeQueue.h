#ifndef _FIXSIZEQUEUE_H_
#define _FIXSIZEQUEUE_H_

#include <algorithm>
#include <iterator>
#include <boost/scoped_array.hpp>

template<class T> class FixSizeQueue {
	T *first, *second; //arrays contain data
	T *beginP, *endP; //pointers indicate where actual data starts and ends
					  //beginP always points in first array
					  //endP never points outside of first or second array
	size_t maxSize, currentSize;
	boost::scoped_array<T> firstPtr, secondPtr;

	class BaseIterator : public std::iterator<std::forward_iterator_tag, T> {
	protected:
		mutable T * current;
		FixSizeQueue<T> const * queue;
	public:
		BaseIterator(T * pointer, FixSizeQueue<T> const * queue) 
			: current(pointer), queue(queue) {}
		BaseIterator const & operator++() const;
		bool operator==(BaseIterator const & it) const { return current == it.current; }
		bool operator!=(BaseIterator const & it) const { return !(*this == it); }
		
	};
	struct Iterator : public BaseIterator {
		Iterator(T * pointer, FixSizeQueue<T> const * queue) 
			: BaseIterator(pointer, queue) {}
		T & operator*() { return *current; }
	};
	struct ConstIterator : public BaseIterator {
		ConstIterator(T * pointer, FixSizeQueue<T> const * queue) 
			: BaseIterator(pointer, queue) {}
		T const & operator*() const { return *current; }
	};

	FixSizeQueue(const FixSizeQueue &);
public:
	FixSizeQueue() : maxSize(0) {}
	FixSizeQueue(const size_t maxSize);
	FixSizeQueue & operator=(const FixSizeQueue &);

	void put(const T & e);

	typedef Iterator iterator;
	typedef ConstIterator const_iterator;
	iterator begin() { return Iterator(beginP, this); }
	iterator end() { return Iterator(endP, this); }
	const_iterator cbegin() const { return ConstIterator(beginP, this); }
	const_iterator cend() const { return ConstIterator(endP, this); }
};

/* Iterator's prefix ++ operator */
template<class T> typename FixSizeQueue<T>::BaseIterator const & 
						FixSizeQueue<T>::BaseIterator::operator++() const 
{
	if (current == &queue->first[queue->maxSize - 1])
		current = queue->second;
	else
		++current;
	return *this;
} /* End of 'FixSizeQueue<T>::Iterator::operator++' function */

/* Constructor */
template<class T> FixSizeQueue<T>::FixSizeQueue(const size_t maxSize) 
			: first(0), second(0), maxSize(0), currentSize(0)
{
	firstPtr.reset(new T[maxSize]);
	secondPtr.reset(new T[maxSize]);
	/* only when everything is ok with memory allocation, 
		initialize size */
	this->maxSize = maxSize; 
	first = firstPtr.get();
	second = secondPtr.get();
	beginP = endP = first;
} /* End of 'FixSizeQueue<T>::FixSizeQueue' function */

/* Assign operator */
template<class T> FixSizeQueue<T> & FixSizeQueue<T>::operator=(const FixSizeQueue & q) {
	if (this != &q) {
		maxSize = q.maxSize;
		if (maxSize == 0)
			return *this;
		currentSize = q.currentSize;

		first = new T[maxSize];
		second = new T[maxSize];
		firstPtr.reset(first);
		secondPtr.reset(second);

		size_t beginOffset = q.beginP - q.first;
		beginP = first + beginOffset; //biginP always points to the first
		if (beginOffset + currentSize < maxSize) //endP points to the first
			endP = first + (q.endP - q.first);
		else //endP points to the second
			endP = second + (q.endP - q.second);

		std::copy(q.cbegin(), q.cend(), begin());
	}
	return *this;
} /* End of 'FixSizeQueue<T>::operator=' function */

/* Puts new element in queue */
template<class T> void FixSizeQueue<T>::put(const T & e) {
	if (maxSize == 0)
		return;

	*endP = e;
	if (endP == &second[maxSize - 1]) { //points to the last element of second array
			/* in this case currentSize already equals maxSize */
			std::swap(first, second);
			endP = second;
			beginP = first;	
	}
	else if (endP == &first[maxSize - 1])
		endP = second, currentSize++;
	else {
		++endP;
		if (currentSize == maxSize)
			++beginP; //beginP never goes from first array to second
		else
			currentSize++;
	}
} /* End of 'FixSizeQueue<T>::put' function */

#endif _FIXSIZEQUEUE_H_
