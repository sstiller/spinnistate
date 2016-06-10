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


StateContainer::StateContainer(StateMachine* stateMachine, const std::string& name, StateContainer* parent)
: StateMachineElement(stateMachine, name),
  entryState(nullptr),
  parentContainer(parent)
{
  if(!stateMachine)
  {
    throw(std::invalid_argument("StateMachine nullptr."));
  }
}

StateContainer* StateContainer::getParent() const
{
  return(parentContainer);
}

bool StateContainer::isDescendantOf(const StateContainer* other) const
{
  if(!other)
  {
    std::cout << getName() << "." << __func__ << "(nullptr) called." << std::endl;
    return(false);
  }
  if(getStateMachine() == other)
  {
    return(true);
  }
  std::cout << getName() << "." << __func__ << "(" << other->getName() << ") called." << std::endl;
  for(StateContainer* currentContainer = this->getParent(); currentContainer != getStateMachine(); currentContainer = currentContainer->getParent())
  {
    if(currentContainer == other)
    {
      return(true);
    }
  }
  return(false);
}

bool StateContainer::isSCXMLElement() const
{
  return(this == getStateMachine());
}

State* StateContainer::addState(const std::string name, StateType stateType)
{
  if(existingStates.find(name) != existingStates.end())
  {
    throw(std::logic_error("State with the given name already exists!"));
  }

  State* newState = new State(name, getStateMachine(), this, stateType);
  existingStates.insert(std::pair<std::string, std::unique_ptr<State> >(name, std::unique_ptr<State>(newState)));
  getStateMachine()->announceState(existingStates.at(name).get());
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
