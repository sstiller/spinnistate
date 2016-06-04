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
      TransitionSet enabledTransitions = selectTransitions();
      if(enabledTransitions.empty())
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

bool StateMachine::finalReached() const
{
  std::cout << __PRETTY_FUNCTION__ << " Implement me!" << std::endl;
  return(false);
}

const OrderedSet<State*>& StateMachine::getConfiguration() const
{
  return(configuration);
}

List<State*> StateMachine::getHistoryValue(const State* state) const
{
  try{
    return(historyValue.at(state));
  }catch(...)
  {
  }
  // return empty list
  return(List<State*>());
}

void StateMachine::setHistoryValue(const State* state, List<State*> history)
{
  historyValue[state] = history;
}

void StateMachine::setStateActive(State* state)
{
  std::cout << __func__ << "(" << state->getName() << ") called" << std::endl;
  configuration.addElement(state);
}

void StateMachine::resetStateActive(State* state)
{
  std::cout << __func__ << "(" << state->getName() << ") called" << std::endl;
  configuration.deleteElement(state);
}

// protected/private

TransitionSet StateMachine::selectTransitions(const std::string& event)
{
  TransitionSet retSet;
  if(configuration.empty())
  {
    throw(std::logic_error("Failed finding transitions, configuration is empty."));
  }
  for(State* currentState : configuration)
  {
    do
    {
      Transition* retTransition = currentState->findExecutibleTransition(event);
      if(retTransition)
      {
        retSet.insert(retTransition); // double entries will not happen because of std::set
        break; // continue for loop
      }
      currentState = currentState->getParent(); // if no parent state --> top level state
    }while(currentState);
  }
  if(retSet.size() > 1)
  {
    removeConflictingTransitions(retSet);
  }
  return(retSet);
}

TransitionSet StateMachine::removeConflictingTransitions(const TransitionSet& transitions)
{
  std::set<Transition*, StateMachineElement::PointerDocOrderCompare> filteredTransitions;
  std::cerr << __PRETTY_FUNCTION__ << " IMPLEMENT ME!" << std::endl;
  for(auto t1 : transitions)
  {
    bool t1Preemted(false);
    std::set<Transition*> transitionsToRemove;
    auto t1ExitSet = t1->computeExitSet();
    for(auto t2 : filteredTransitions)
    {
      auto t2ExitSet = t2->computeExitSet();
      if(t1ExitSet.hasIntersection(t2ExitSet))
      {
        if(t2->getSrcState()->isAncestorOf(t1->getSrcState()))
        {
          transitionsToRemove.insert(t2);
        }else
        {
          t1Preemted = true;
          break;
        }
      }
    }
    if(! t1Preemted)
    {
      for(auto t3 : transitionsToRemove)
      {
        filteredTransitions.erase(t3);
      }
      filteredTransitions.insert(t1);
    }
  }
  return(filteredTransitions);
}

} // namespace ssm 
