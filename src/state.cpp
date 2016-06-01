/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */
#include "state.h"

namespace ssm // "Spinni state machine"
{

State::State(const std::string& name, StateMachine* stateMachine, State* parent, bool isEntryState, bool parallel, bool history)
: StateMachineElement(stateMachine, name),
  StateContainer(stateMachine),
  ActionContainer(stateMachine),
  parentState(parent),
  isEntryState(isEntryState),
  parallel(parallel),
  isHistory(history)
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

Transition* State::addTransition(const std::string& name, const std::string& triggerName, const std::string& guard)
{
  transitions.push_back(std::unique_ptr<Transition>(new Transition(name, this, triggerName, guard)));
  return(transitions.back().get());
}

Transition* State::findExecutibleTransition(const std::string& event)
{
  for(auto& currentTransition : transitions)
  {
    if(currentTransition->conditionsSatisfied(event))
    {
      return(currentTransition.get());
    }
  }
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
  return(other->isAncestorOf(this));
}

bool State::isParallel() const
{
  return(parallel);
}

bool State::isEntry() const
{
  return(isEntryState);
}

bool State::isHistoryState() const
{
  return(isHistory);
}

bool State::isCompoundState() const
{
  return(StateContainer::containsStates());
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
  if(isParallel())
  {
    std::cout << "State " << getName() << " TODO: Enter substates (because parallel)." << std::endl;
    //TODO: enter all the states
  }else if(entryState)
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

// static

State* State::findLCCA(List<State*> stateList)
{
  for(auto anc :
      stateList.head()->
        getProperAncestors(nullptr).
          filter([](State* state){return(! state || state->isCompoundState());})) //  isCompoundStateOrScxmlElement))
  {
    if(stateList.tail().every(std::bind(&State::isDescendantOf, std::placeholders::_1, anc)))
    {
      return(anc);
    }
  }
  return(nullptr);
}



} // namespace ssm

