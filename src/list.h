/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * List.h
 *
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 */

#ifndef _LIST_H_
#define _LIST_H_

// stl
#include <functional>
#include <list>

namespace ssm {

template <typename T>
class List {
public:
	/// Returns the head of the list.
	T head() const
	{
		return(data.front());
	};

	/** Returns the tail of the list.
	 * (i.e., the rest of the list once the head is removed)
	 */
	T& tail() const
	{
		return(data.back());
	}

	///< Returns the list appended with l
	void append(const T& l)
	{
		data.push_back(l);
	};

	/// Returns the list of elements that satisfy the predicate f
	List<T> filter(std::function<bool (const T&)> f) const
	{
		List<T> retList;
		for(auto currentEntry : data)
		{
			if(f(currentEntry))
			{
				retList.append(currentEntry);
			}
		}
		return(retList);
	};

	/** Returns true if some element in the list satisfies the predicate f.
	 * Returns false for an empty list.
	 */
	bool some(std::function<bool (const T&)> f) const
	{
		for(auto currentEntry : data)
		{
			if(f(currentEntry))
			{
				return(true);
			}
		}
		return(false);
	};

	/** Returns true if every element in the list satisfies the predicate f.
	 *  Returns true for an empty list.
	 */
	bool every(std::function<bool (const T&)> f) const
	{
		for(auto currentEntry : data)
		{
			if(! f(currentEntry))
			{
				return(false);
			}
		}
		return(true);
	}
private:
	std::list<T> data;
};

} /* namespace ssm */

#endif /* _LIST_H_ */
