/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * event.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro@Wuerfelkiste>
 *
 */

// stl
#include <sstream>

// local includes
#include "event.h"

namespace ssm
{

Event::Event(const std::string& descriptor)
: descriptor(descriptor)
{
}

bool Event::matches(const Event& other) const
{
  auto localElements = split();
  auto otherElements = other.split();
  if(localElements.size() > otherElements.size()
  {
    return(false);
  }
  for(int i = 0; i < localElements.size(); i++)
  {
    if(localElements[i] != otherElements[i])
    {
      return(false);
    }
  }
     returmn(true);
}

// private
std::vector<std::string> Event::split() const
{
  std::vector<std::string> retVector;
  std::stringstream ss(descriptor);
  std::string currentElement;
  while (std::getline(ss, currentElement, '.'))
  {
      retVector.push_back(item);
  }
  return retVector;
}  

} // namespace ssm

