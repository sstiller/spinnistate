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
	List<T> tail() const
	{
    List<T>retList;
    retList.data = data;
    if(! retList.data.empty())
    {
      retList.data.pop_front();
    }
		return(retList);
	}

	///< Returns the list appended with l
	List<T> append(const T& l)
	{
    List<T> retList;
    retList.data = data;
    retList.data.push_back(l);
    return(retList);
	};

	///< Returns the list appended with l
  List<T> append(const List<T>& l)
	{
    List<T> retList;
    retList.data = data;
    for(auto currentElement : l.data)
    {
      retList.data.push_back(currentElement);
    }
    return(retList);
	};

	/// Returns the list of elements that satisfy the predicate f
	List<T> filter(std::function<bool (const T&)> f) const
	{
		List<T> retList;
		for(auto currentEntry : data)
		{
			if(f(currentEntry))
			{
				retList.addEntry(currentEntry);
			}
		}
		return(retList);
	};

  void addEntry(const T& l)
  {
    data.push_back(l);
  }
  
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

  bool empty() const
  {
    return(data.empty());
  }

  typename std::list<T>::const_iterator begin() const
  {
    return(data.begin());
  }
  typename std::list<T>::const_iterator end() const
  {
    return(data.end());
  }

private:
	std::list<T> data;
};

} /* namespace ssm */

#endif /* _LIST_H_ */
