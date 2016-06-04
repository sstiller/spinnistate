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
  stateMachineInitialized(false),
  running(false)
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

  running = true;
  //TODO: fix this:
  if(entryState)
  {
    entryState->enter();
  }else{
    throw(std::logic_error("No state to enter!"));
  }
}

void StateMachine::processEvent(const std::string& eventName)
{
  //TODO: set somewhere else and remove eventName param
  externalQueue.enqueue(eventName);
  
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
        if(internalQueue.isEmpty())
        {
          macrostepDone = true;
        }else
        {
          auto internalEvent = internalQueue.dequeue();
          //TODO: datamodel["_event"] = internalEvent
          enabledTransitions = selectTransitions(internalEvent);
        }
        std::cout << __func__ << "() Would check internal events" << std::endl;
        macrostepDone = true; //TODO: Only if internal queue is empty!
      }

      if(! enabledTransitions.empty())
      {
        microstep(enabledTransitions.toList());
      }
    }

    //  either we're in a final state, and we break out of the loop 
    if(! running)
    {
      break;
    }
    // or we've completed a macrostep, so we start a new macrostep by waiting for an external event
    // Here we invoke whatever needs to be invoked. The implementation of 'invoke' is platform-specific
    for(auto state : statesToInvoke.sort(entryOrder))
    {
      for(auto inv : state.invoke.sort(documentOrder))
      {
        invoke(inv);
      }
    }
    statesToInvoke.clear()

    //TODO:
    /*
    # Invoking may have raised internal error events and we iterate to handle them        
    if not internalQueue.isEmpty():
        continue
    # A blocking wait for an external event.  Alternatively, if we have been invoked
    # our parent session also might cancel us.  The mechanism for this is platform specific,
    # but here we assume it’s a special event we receive
    externalEvent = externalQueue.dequeue()
    if isCancelEvent(externalEvent):
        running = false
        continue
    datamodel["_event"] = externalEvent
    for state in configuration:
        for inv in state.invoke:
            if inv.invokeid == externalEvent.invokeid:
                applyFinalize(inv, externalEvent)
            if inv.autoforward:
                send(inv.id, externalEvent) 
    enabledTransitions = selectTransitions(externalEvent)
    if not enabledTransitions.isEmpty():
        microstep(enabledTransitions.toList()) 
     */

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

void StateMachine::printConfiguration() const
{
  std::cout << "Current config SM:\n";
  for(auto state : configuration)
  {
    std::cout << "\t" << state->getName() << "\n";
  }
  std::cout << std::flush;
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
        retSet.addElement(retTransition); // double entries will not happen because of std::set
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
  TransitionSet filteredTransitions;
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
        filteredTransitions.deleteElement(t3);
      }
      filteredTransitions.addElement(t1);
    }
  }
  return(filteredTransitions);
}

void StateMachine::microstep(TransitionSet enabledTransitions)
{
  exitStates(enabledTransitions);
  executeTransitionContent(enabledTransitions);
  enterStates(enabledTransitions);
}

void StateMachine::exitStates(List<Transition*>& enabledTransitions)
{
  std::cerr << __PRETTY_FUNCTION__ << " IMPLEMENT ME!" << std::endl;
}

void StateMachine::executeTransitionContent(List<Transition*>& enabledTransitions)
{
  std::cerr << __PRETTY_FUNCTION__ << " IMPLEMENT ME!" << std::endl;
}

void StateMachine::enterStates(List<Transition*>& enabledTransitions)
{
  std::cerr << __PRETTY_FUNCTION__ << " IMPLEMENT ME!" << std::endl;
}

} // namespace ssm 
