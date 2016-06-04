/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * OrderedSet.h
 *
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 */

#ifndef _ORDEREDSET_H_
#define _ORDEREDSET_H_

// stl
#include <list>
#include <functional>
#include <algorithm> // find

// local includes
#include "list.h"

namespace ssm {

template <typename T>
class OrderedSet: public std::list<T>
{
public:
  // Adds e to the set if it is not already a member
  void addElement(const T& e)
  {
    if(! isMember(e))
    {
      this->push_back(e);
    }
  }

  // Deletes e from the set
  void deleteElement(const T& e)
  {
    auto pos = std::find(std::list<T>::begin(), std::list<T>::end(), e);
    if(pos != std::list<T>::end())
    {
      this->erase(pos);
    }
  }

  /** Adds all members of s that are not already members of the set.
   * s must also be an OrderedSet,
   */
  void unite(const OrderedSet<T>& s)
  {
    for(auto currentElement : s)
    {
      addElement(currentElement);
    }
  };

  void unite(const List<T>& s)
  {
    for(auto& currentElement : s)
    {
      addElement(currentElement);
    }
  };

  /** Is e a member of set?
   */
  bool isMember(const T& e) const
  {
    return(std::find(std::list<T>::begin(), std::list<T>::end(), e) != std::list<T>::end());
  };

  /** Returns true if some element in the set satisfies the predicate f.
   *  @returns false for an empty set.
   */
  bool some(std::function<bool (const T&)> f)
  {
    for(auto currentElement : *this)
    {
	    if(f(currentElement))
	    {
		    return(true);
	    }
    }
    return(false);
  }

  /** Returns true if every element in the set satisfies the predicate f.
   * @eturns true for an empty set.
   */
  bool every(std::function<bool (const T&)> f)
  {
    for(auto currentEntry : *this)
    {
	    if(! f(currentEntry))
	    {
		    return(false);
	    }
    }
    return(true);
  }

	/** Returns true if this set and  set s have at least one member in common
	 */
	bool hasIntersection(const OrderedSet<T>& s) const
  {
    for(auto currentElement : *this)
    {
      if(s.isMember(currentElement))
      {
        return(true);
      }
    }
    return(false);
  }

	/** Converts the set to a list that reflects the order in which elements were originally added
	  * In the case of sets created by intersection, the order of the first set (the one on which the method was called) is used
	  * In the case of sets created by union, the members of the first set (the one on which union was called) retain their original ordering
	  * while any members belonging to the second set only are placed after, retaining their ordering in their original set.
	  */
	List<T> toList() const
  {
    List<T> retList;
    for(auto currentEntry : *this)
    {
      retList.addEntry(currentEntry);
    }
    return(retList);
  }
 
//private:
//	std::list<T> data;
};

} /* namespace ssm */

#endif /* _ORDEREDSET_H_ */
