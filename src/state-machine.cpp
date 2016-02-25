/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#include "state-machine.h"
#include "state.h"

namespace ssm // "Spinni state machine"
{
  
StateMachine::StateMachine(boost::asio::io_service& ioService, DataModel* dataModel)
: StateContainer(this),
  ioService(ioService),
  activeState(nullptr),
  dataModel(dataModel),
  stateMachineInitialized(false)
{
  if(!dataModel)
  {
    throw(std::invalid_argument("Data model nullptr."));
  }
}

StateMachine::~StateMachine()
{
}

State* StateMachine::getState(const std::string name, bool create)
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

void StateMachine::announceState(State* newState)
{
  // Check if name already exists and add to global state list  
  if(stateExists(newState->getName()))
  {
    throw(std::logic_error("State name already in references list!")); 
  }
  statePointers.insert(std::pair<std::string, State*>(newState->getName(), newState));  
}

bool StateMachine::stateExists(const std::string& name)
{
  auto stateIt = statePointers.find(name);
  return(stateIt != statePointers.end());
}

DataModel* StateMachine::getDataModel()
{
  return(dataModel);
}

void StateMachine::init(const std::string& dataModelInitScript)
{
  if(isInitialized())
  {
    throw(std::logic_error("StateMachine already initialized."));
  }
  this->dataModelInitScript = dataModelInitScript;
  stateMachineInitialized = true;
}

bool StateMachine::isInitialized()
{
  return(stateMachineInitialized);
}

void StateMachine::start()
{
  if(!isInitialized())
  {
    throw(std::logic_error("Can not start StateMachine (not initialized)."));
  }  
  if(dataModelInitScript.size())
  {
    dataModel->executeAction(dataModelInitScript);
  }
  if(entryState)
  {
    entryState->enter();
  }else{
    throw(std::logic_error("No state to enter!"));
  }
}

void StateMachine::processEvent(const std::string& eventName)
{
  std::cout << __func__ << "(" << eventName << ") called." << std::endl;
  bool done = false;
  do
  {
    bool macrostepDone = false;
    while(! macrostepDone)
    {
      // check for eventless transitions
      // TODO: later here we select more transitions (for parallel states)
      Transition* activeTransition = findExecutibleTransition();
      if(! activeTransition)
      {
        std::cout << __func__ << "() Would check internal events" << std::endl;
        macrostepDone = true; //TODO: Only if internal queue is empty!
      }else{
        std::cout << __func__ << "() Would execute microstep because eventless transition possible" << std::endl;
      }
    }

    if(finalReached())
    {
      break;
    }
break;
  }while(! done);
  std::cout << __func__ << "() finished." << std::endl;
}

bool StateMachine::finalReached()
{
  std::cout << __PRETTY_FUNCTION__ << " Implement me!" << std::endl;
  return(false);
}

State* StateMachine::getActiveState()
{
  return(activeState);
}

// protected/private

Transition* StateMachine::findExecutibleTransition(const std::string& event)
{
  State* currentState = getActiveState();
  if(! currentState)
  {
    throw(std::logic_error("Can not find transition, current state == nullptr."));
  }
  do
  {
    Transition* retTransition = currentState->findExecutibleTransition(event);
    if(retTransition)
    {
      return(retTransition);
    }
    currentState = currentState->getParent(); // if no parent state --> top level state
  }while(currentState);
  return(nullptr);
}

} // namespace ssm 
