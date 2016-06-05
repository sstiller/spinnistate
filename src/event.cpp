/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * event.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro@Wuerfelkiste>
 *
 */

// stl
#include <sstream>
#include <iostream>

// local includes
#include "event.h"

namespace ssm
{

Event::Event(const std::string& descriptor)
: descriptor(descriptor)
{
  std::cout << __func__ << "(" << descriptor << ") called." << std::endl;
}

bool Event::matches(const Event& other) const
{
  auto localElements = split();
  auto otherElements = other.split();
  if(localElements.size() > otherElements.size())
  {
    return(false);
  }
  for(unsigned int i = 0; i < localElements.size(); i++)
  {
    if(localElements[i] != otherElements[i])
    {
      return(false);
    }
  }
  return(true);
}

// private
std::vector<std::string> Event::split() const
{
  std::vector<std::string> retVector;
  std::stringstream ss(descriptor);
  std::string currentElement;
  while (std::getline(ss, currentElement, '.'))
  {
    //std::cout << __PRETTY_FUNCTION__ << " adding \"" << currentElement << "\"." << std::endl;
    retVector.push_back(currentElement);
  }
  return retVector;
}  

} // namespace ssm

