/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * event.h
 * Copyright (C) 2016 Sandro Stiller <sandro@Wuerfelkiste>
 *
 */

#ifndef _EVENT_H_
#define _EVENT_H_

// stl
#include <vector>
#include <string>

namespace ssm
{

class Event
{
public:
  Event() = default; // empty descriptor;
  Event(const std::string& descriptor);
  ~Event() = default;
  bool matches(const Event& other) const;
  std::string getDescriptor() const
  {
    return(descriptor);
  }
  operator bool() const
  {
    return(!descriptor.empty());
  }

private:
  std::vector<std::string> split() const;
  const std::string descriptor;

};

} // namespace ssm

#endif // _EVENT_H_

