/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-container.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro@Wuerfelkiste>
 *
 */

#include "state.h"
#include "state-container.h"

namespace ssm // "Spinni state machine"
{


StateContainer::StateContainer()
{
  // TODO: Add implementation here
}

StateContainer::~StateContainer()
{
  // TODO: Add implementation here
}

State& StateContainer::getState(const std::string name, bool create)
{
  if(create && (containedStates.find(name) == containedStates.end()))
  {
    State newState(name);
    containedStates.insert(std::pair<std::string, State>(name, newState));
  }
  return(containedStates.at(name));
}


} // namespace ssm


