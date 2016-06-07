/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * action-container.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#ifndef _ACTION_CONTAINER_H_
#define _ACTION_CONTAINER_H_

// stl
#include <string>

// local includes
#include "list.h"
#include "data-model.h"
#include "state-machine.h"

namespace ssm // "Spinni state machine"
{
class StateMachine;
  
class ActionContainer
{
public:
  ActionContainer() = delete;
  ActionContainer(StateMachine* stateMachine);
  
  void addOnEntryAction(const std::string& actionCommand);
  void addOnExitAction(const std::string& actionCommand);
protected:
  void executeOnEntry();
  void executeOnExit();

private:
  List<std::string> onEntryActions;
  List<std::string> onExitActions;
  StateMachine* stateMachine;
  DataModel* dataModel;
};

} // namespace ssm

#endif // _ACTION_CONTAINER_H_

