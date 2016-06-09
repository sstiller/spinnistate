/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */
#include "libspinnistate.h"
#include "state.h"

namespace ssm // "Spinni state machine"
{

State::State(const std::string& name, StateMachine* stateMachine, State* parent, StateType stateType)
: StateMachineElement(stateMachine, name),
  StateContainer(stateMachine),
  ActionContainer(stateMachine),
  parentState(parent),
  stateType(stateType)
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

State* State::getState(const std::string name, StateType stateType, bool create)
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
  return(addState(name, this, stateType));
}

Transition* State::addTransition(const std::string& name, const std::string& guard)
{
  transitions.push_back(std::unique_ptr<Transition>(new Transition(name, this, guard)));
  return(transitions.back().get());
}

Transition* State::findExecutibleTransition(const Event& event)
{
  std::cout << __func__ << "(" << event.getDescriptor() << ") in state " << getName() << " called." << std::endl; 
  for(auto& currentTransition : transitions)
  {
    if(currentTransition->conditionsSatisfied(event))
    {
      std::cout << __func__ << "(): " << getName() << " found transition." << std::endl;
      return(currentTransition.get());
    }
  }
  std::cout << __func__ << "(): " << getName() << " no transition." << std::endl;
  return(nullptr);
}

State* State::getParent() const
{
  return(parentState);
}

bool State::isAncestorOf(const State* other) const
{
  if(!other)
  {
    return(false);
  }
  for(State* currentState = other->getParent(); currentState != nullptr; currentState = currentState->getParent())
  {
    if(currentState == this)
    {
      return(true);
    }
  }
  return(false);
}

bool State::isDescendantOf(const State* other) const
{
  if(!other)
  {
    std::cout << getName() << "." << __func__ << "(nullptr) called." << std::endl;
    return(false);
  }
  std::cout << getName() << "." << __func__ << "(" << other->getName() << ") called." << std::endl;
  return(other->isAncestorOf(this));
}

StateType State::getStateType() const
{
  return(stateType);
}

bool State::isParallel() const
{
  return(stateType == StateType::Parallel);
}

bool State::isInitial() const
{
  return(stateType == StateType::Initial);
}

bool State::isHistoryState() const
{
  return((stateType == StateType::HistoryDeep) ||
         (stateType == StateType::HistoryShallow));
}

bool State::isCompoundState() const
{
  return(StateContainer::containsStates());
}

bool State::isFinal() const
{
  return(stateType == StateType::Final);
}

bool State::isAtomicState() const
{
  return((! isCompoundState()) || isFinal());
}

List<State*> State::getProperAncestors(const State* state2) const
{
  List<State*> retList;
  if(state2 &&
     ((state2 == this) ||
      (state2->isDescendantOf(this))
     )
    )
  {
    // empty set
    return(retList);
  }
   
  for(auto currentAncestor = getParent(); currentAncestor && (currentAncestor != state2); currentAncestor = currentAncestor->getParent())
  {
    retList.addEntry(currentAncestor);
  } // for getParents

  return(retList);
}


void State::enter()
{
  executeOnEntry();
  getStateMachine()->setStateActive(this);
  if(isParallel())
  {
    std::cout << "State " << getName() << " TODO: Enter substates (because parallel)." << std::endl;
    //TODO: enter all the states
  }else if(isInitial())
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

#ifndef NOIMPLEMENT_INVOKE
#error implement invoke stuff here
    for(inv in s.invoke)
    {
      cancelInvoke(inv);
    }
#endif
  getStateMachine()->resetStateActive(this);
}

StateContainer* State::findLCCA(List<State*> stateList)
{
std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << std::endl;
  for(auto anc :
      stateList.head()->
        getProperAncestors(nullptr).
          filter([](State* state){return(! state || state->isCompoundState());})) //  isCompoundStateOrScxmlElement))
  {
    if(stateList.tail().every(std::bind(&State::isDescendantOf, std::placeholders::_1, anc)))
    {
std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << std::endl;
      return(anc);
    }
  }
std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << std::endl;
  return(getStateMachine());
}



} // namespace ssm

