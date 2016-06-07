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


StateContainer::StateContainer(StateMachine* stateMachine)
: entryState(nullptr),
  stateMachine(stateMachine)  
{
  if(!stateMachine)
  {
    throw(std::invalid_argument("StateMachine nullptr."));
  }
}

StateContainer::~StateContainer()
{
}

State* StateContainer::addState(const std::string name, State* parentState, StateType stateType)
{
  if(existingStates.find(name) != existingStates.end())
  {
    throw(std::logic_error("State with the given name already exists!"));
  }

  State* newState = new State(name, stateMachine, parentState, stateType);
  existingStates.insert(std::pair<std::string, std::unique_ptr<State> >(name, std::unique_ptr<State>(newState)));
  stateMachine->announceState(existingStates.at(name).get());
  if(existingStates.size() == 1)
  {
    // take the first state as antry state
    entryState = newState;
  }
  if(newState->isInitial())
  {
    entryState = newState;
  }
  return(newState);
}

State* StateContainer::findState(const std::string& name)
{
  return(existingStates.at(name).get());
}

bool StateContainer::containsStates() const
{
  return(! existingStates.empty());
}

List<State*> StateContainer::getHistory() const
{
  List<State*> retList;
  for(auto entryPair : existingStates)
  {
    if(entryPair.second->isHistoryState())
    {
      retList.addEntry(entryPair.second.get());
    }
  }
  return(retList);
}

} // namespace ssm
