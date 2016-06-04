/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * Queue.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

//stl
#include <queue>

namespace ssm
{

template <typename T>
class Queue
{
public:
  // Puts e last in the queue
  void enqueue(const T& e)
  {
    data.push(e);
  }

  // Removes and returns first element in queue
  T dequeue()
  {
    T retVal = data.front();
    data.pop();
    return(retVal);
  }

  // Is the queue empty?
  bool isEmpty()
  {
    return(data.empty());
  }

private:
	std::queue<T> data;
};
    

} /* namespace ssm */

#endif // _QUEUE_H_

