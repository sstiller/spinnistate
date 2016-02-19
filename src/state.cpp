/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */
#include "state.h"

namespace ssm // "Spinni state machine"
{

State::State(const std::string& name, StateMachine* stateMachine, State* parent)
: StateMachineElement(stateMachine, name),
  StateContainer(stateMachine),
  ActionContainer(stateMachine),
  parentState(parent)
{
  if(getName().size() == 0)
  {
    throw(std::invalid_argument("No valid name given."));
  }
  if(getStateMachine()->stateExists(name))
  {
    throw(std::invalid_argument("State with given name already exists."));
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

Transition* State::addTransition(const std::string& name, State* dstState, const std::string& triggerName, const std::string& guard)
{
  transitions.push_back(std::unique_ptr<Transition>(new Transition(name, this, dstState, triggerName, guard)));
  return(transitions.back().get());
}

State* State::getParent()
{
  return(parentState);
}

void State::enter()
{
  executeOnEntry();
  if(entryState)
  {
    std::cout << "State " << getName() << " is entering substate." << std::endl;
    entryState->enter();
  }else{
    std::cout << "State " << getName() << " is waiting for events." << std::endl;
  }  
}

void State::leave()
{
  executeOnExit();
}

} // namespace ssm

