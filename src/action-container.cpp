/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * action-container.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */
#include "state-machine.h"
#include "action-container.h"

namespace ssm // "Spinni state machine"
{
  
ActionContainer::ActionContainer(StateMachine* stateMachine)
: stateMachine(stateMachine),
  dataModel(stateMachine->getDataModel())
{
  if(!stateMachine)
  {
    throw(std::invalid_argument("StateMachine nullptr."));
  }  
  if(!dataModel)
  {
    throw(std::invalid_argument("DataModel nullptr."));
  }  
}

void ActionContainer::addOnEntryAction(const std::string& actionCommand)
{
  onEntryActions.addEntry(actionCommand);
}

void ActionContainer::addOnExitAction(const std::string& actionCommand)
{
  onExitActions.addEntry(actionCommand);
}

void ActionContainer::executeOnEntry()
{
  for(auto onEntryAction : onEntryActions)
  {
    dataModel->executeAction(onEntryAction);
  }
}

void ActionContainer::executeOnExit()
{
  for(auto onExitAction : onExitActions)
  {
    dataModel->executeAction(onExitAction);
  }
}

} // namespace ssm

