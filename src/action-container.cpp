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
  
ActionContainer::ActionContainer(StateMachine& stateMachine)
: stateMachine(stateMachine),
  dataModel(stateMachine.getDataModel())
{
}

void ActionContainer::setOnEntryAction(const std::string& actionCommand)
{
  onEntryAction = actionCommand;
}

void ActionContainer::setOnExitAction(const std::string& actionCommand)
{
  onExitAction = actionCommand;
}

const std::string& ActionContainer::getOnEntryAction()
{
  return(onEntryAction);
}
const std::string& ActionContainer::getOnExitAction()
{
  return(onExitAction);
}

void ActionContainer::executeOnEntry()
{
  dataModel.executeAction(onEntryAction);
}

void ActionContainer::executeOnExit()
{
  dataModel.executeAction(onExitAction);
}

} // namespace ssm

