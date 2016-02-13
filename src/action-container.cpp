/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * action-container.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#include "action-container.h"

namespace ssm // "Spinni state machine"
{
  
ActionContainer::ActionContainer()
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

} // namespace ssm

