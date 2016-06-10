/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

// stl
#include <functional>

// local includes
#include "state-machine.h"
#include "state.h"

namespace ssm // "Spinni state machine"
{
  
StateMachine::StateMachine(boost::asio::io_service& ioService, DataModel* dataModel, const std::string& machineName)
: StateContainer(this, machineName, this),
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

State* StateMachine::getState(const std::string name, StateType stateType, bool create)
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

  return(addState(name, stateType));
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

void StateMachine::processExternalEvent(const Event externalEvent)
{
  ioService.post(std::bind(&StateMachine::executeMacrosteps, this, externalEvent));
}

void StateMachine::executeMacrosteps(const Event& externalEvent)
{
  std::cout << __func__ << "(" << externalEvent << ") called." << std::endl;
  TransitionSet enabledTransitions;
  
  do
  {
    // handle internal events
    bool macrostepDone = false;
    while(! macrostepDone)
    {
      // check for eventless transitions
      enabledTransitions = selectTransitions();
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
        std::cout << __func__ << "() TODO: check internal events" << std::endl;
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
      return;
    }
    // or we've completed a macrostep, so we start a new macrostep by waiting for an external event
  #ifndef NOIMPLEMENT_INVOKE
  #error implement invoke stuff here
    /* TODO: implement the "invoke" stuff"
    // Here we invoke whatever needs to be invoked. The implementation of 'invoke' is platform-specific
    statesToInvoke.sort(StateMachineElement::entryOrder);
    for(auto state : statesToInvoke)
    {
      for(auto inv : state.invoke.sort(documentOrder))
      {
        invoke(inv);
      }
    }
    statesToInvoke.clear();
    */
  #endif
    // Invoking may have raised internal error events and we iterate to handle them        
  } while(! internalQueue.isEmpty());

  /* TODO: datamodel["_event"] = externalEvent; */
#ifndef NOIMPLEMENT_INVOKE
#error implement invoke stuff here
  for(auto  state : configuration)
  {
    for(auto inv : state->invoke)
    {
      if(inv.invokeid == externalEvent.invokeid)
      {
        applyFinalize(inv, externalEvent);
      }
      if(inv.autoforward)
      {
        send(inv.id, externalEvent);
      }
    }
  }
#endif
  enabledTransitions = selectTransitions(externalEvent);
  if(! enabledTransitions.isEmpty())
  {
    microstep(enabledTransitions.toList());
  }

  if(finalReached())
  {
    std::cout << __PRETTY_FUNCTION__ << " TODO: WHAT NOW?" << std::endl;
  }
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

TransitionSet StateMachine::selectTransitions(const Event& event)
{
  std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
  TransitionSet retSet;
  if(configuration.empty())
  {
    throw(std::logic_error("Failed finding transitions, configuration is empty."));
  }
  auto atomicStates = configuration.toList().filter([](State* state)
                                                    {
                                                      return(state->isAtomicState());
                                                    });
  for(State* currentState : atomicStates)
  {
    Transition* retTransition = currentState->findExecutibleTransition(event);
    if(retTransition)
    {
      retSet.addElement(retTransition); // double entries will not happen because of std::set
      break; // continue for loop
    }
  }
  if(retSet.size() > 1)
  {
    removeConflictingTransitions(retSet);
  }
  std::cout << __PRETTY_FUNCTION__ << " Returning " << retSet.size() << " transitions." << std::endl;
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

void StateMachine::microstep(List<Transition*> enabledTransitions)
{
  exitStates(enabledTransitions);
  executeTransitionContent(enabledTransitions);
  enterStates(enabledTransitions);
}

void StateMachine::exitStates(List<Transition*>& enabledTransitions)
{
  //state->leave()
  std::cout << __PRETTY_FUNCTION__ << " called (" << enabledTransitions.size() << " transitions)." << std::endl;
  OrderedSet<State*> statesToExit;
  for(auto currentTransition : enabledTransitions)
  {
    statesToExit.unite(currentTransition->computeExitSet());           
  }
  
#ifndef NOIMPLEMENT_INVOKE
#error implement invoke stuff here
  for(auto s : statesToExit)
  {
    statesToInvoke.delete(s)
  }
#endif
  statesToExit.sort(StateMachineElement::exitOrder);
  for(State* s : statesToExit)
  {
    for(auto h : s->getHistory())
    {
      std::function<bool(State*)> f;
      if(h->getStateType() == StateType::HistoryDeep)
      {
        f = [s](State* s0)
        {
          return(s0->isAtomicState() && s0->isDescendantOf(s));
        };
      }else
      {
        f = [s](State* s0)
        {
          return(s0->getParent() == s);
        };
      }
      historyValue[h] = configuration.toList().filter(f);
    }
  }
  for (auto s : statesToExit)
  {
    s->leave();
  }
}

void StateMachine::executeTransitionContent(List<Transition*>& enabledTransitions)
{
  std::cerr << __PRETTY_FUNCTION__ << " IMPLEMENT ME!" << std::endl;
}

void StateMachine::enterStates(List<Transition*>& enabledTransitions)
{
  std::cerr << __PRETTY_FUNCTION__ << " IMPLEMENT ME!" << std::endl;
  OrderedSet<State*>statesToEnter;
  OrderedSet<State*>statesForDefaultEntry;
#error implement me!
  // initialize the temporary table for default content in history states
  HashTable defaultHistoryContent; 
  computeEntrySet(enabledTransitions, statesToEnter, statesForDefaultEntry, defaultHistoryContent) 
  for(auto s : statesToEnter.toList().sort(entryOrder))
  {
    configuration.add(s)
    statesToInvoke.add(s)
    if(binding == "late" and s.isFirstEntry)
    {
      initializeDataModel(datamodel.s,doc.s)
      s.isFirstEntry = false
    }
    for(auto content : s.onentry.sort(documentOrder))
    {
        executeContent(content);
    }
    if(statesForDefaultEntry.isMember(s))
    {
      executeContent(s.initial.transition);
    }
    if(defaultHistoryContent[s.id])
    {
      executeContent(defaultHistoryContent[s.id]);
    }
    if(s->isFinalState())
    {
      if(s->getParent()->isSCXMLElement())
      {
        running = false;
      }
      else
      {
        auto parent = s->getParent();
        auto grandparent = parent->getParent();
        internalQueue.enqueue(new Event("done.state." + parent.id, s.donedata));
        if(isParallelState(grandparent))
        {
          if(getChildStates(grandparent).every(isInFinalState))
          {
            internalQueue.enqueue(new Event("done.state." + grandparent.id));
          }
        }
      }
    }
  }
}

} // namespace ssm 
