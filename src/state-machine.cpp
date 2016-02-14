/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#include "state-machine.h"

namespace ssm // "Spinni state machine"
{
  
StateMachine::StateMachine(boost::asio::io_service& ioService, DataModel& dataModel)
: StateContainer(*this),
  ioService(ioService),
  currentState(nullptr),
  dataModel(dataModel)
{
}

StateMachine::~StateMachine()
{
}

State& StateMachine::getState(const std::string name, bool create)
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

  return(addState(name, nullptr));
}

void StateMachine::announceState(State& newState)
{
  // Check if name already exists and add to global state list  
  if(stateExists(newState.getName()))
  {
    throw(std::logic_error("State name already in references list!")); 
  }
  stateReferences.insert(std::pair<std::string, State&>(newState.getName(), newState));  
}

bool StateMachine::stateExists(const std::string& name)
{
  return(stateReferences.find(name) != stateReferences.end());
}

void StateMachine::executeAction(const std::string& actionString)
{
  if(actionString.empty())
  {
    return;
  }
  //TODO: forward to data model
  std::cout << __func__ << "() Implement me: execute \"" << actionString << "\"." << std::endl;
}

void StateMachine::start()
{
  if(entryState)
  {
    entryState->enter();
  }else{
    throw(std::logic_error("No state to enter!"));
  }
}

} // namespace ssm 
