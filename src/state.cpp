/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */
#include "state.h"

namespace ssm // "Spinni state machine"
{

State::State(const std::string& name, StateMachine& stateMachine, State* parent)
: StateContainer(stateMachine),
  ActionContainer(stateMachine),
  name(name),
  stateMachine(stateMachine),
  parentState(parent)
{
  if(name.size() == 0)
  {
    throw(std::invalid_argument("No valid name given!"));
  }
  if(stateMachine.stateExists(name))
  {
    throw(std::invalid_argument("State with given name already exists!"));
  }
}

State::~State()
{
}

State* State::getState(const std::string name, bool create)
{
  try{
    return(findState(name));
  }catch(std::out_of_range)
  {
    if(!create)
    {
      throw;
    }
  }
  return(addState(name, this));
}

void State::addTransition(const std::string& name, State* dstState, const std::string& triggerName, const std::string& guard)
{
  transitions.push_back(Transition(name, this, dstState, triggerName, guard));
}

const std::string& State::getName() const
{
  return(name);
}

State* State::getParent()
{
  return(parentState);
}

StateMachine& State::getStateMachine()
{
  return(stateMachine);
}

void State::enter()
{
  executeOnEntry();
  if(entryState)
  {
    std::cout << "State " << name << " is entering substate." << std::endl;
    entryState->enter();
  }else{
    std::cout << "State " << name << " is waiting for events." << std::endl;
  }  
}

void State::leave()
{
  executeOnExit();
}

} // namespace ssm

