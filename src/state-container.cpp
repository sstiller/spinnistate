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

void StateContainer::addState(const State& newState)
{
  auto it = containedStates.find(newState.getName());
  if (it != containedStates.end())
  {
    throw(std::logic_error("State with given name already exists!"));
  }
  containedStates[newState.getName()] = std::make_shared<State>(newState);
}

void StateContainer::addState(State* newState)
{
  if (containedStates.find(newState->getName()) != containedStates.end())
  {
    throw(std::logic_error("State with given name already exists!"));
  }
  containedStates[newState->getName()] = std::shared_ptr<State>(newState);  
}

void StateContainer::addState(const std::string& name) noexcept
{
  if (containedStates.find(name) == containedStates.end())
  {
    addState(new State(name));
  }
}  

std::shared_ptr<State> StateContainer::getState(const std::string name, bool create)
{
  if(create)
  {
    addState(name);
  }
  return(containedStates.at(name));
}


} // namespace ssm


