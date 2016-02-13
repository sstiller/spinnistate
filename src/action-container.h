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

namespace ssm // "Spinni state machine"
{

class ActionContainer
{
public:
  ActionContainer();
  void setOnEntryAction(const std::string& actionCommand);
  void setOnExitAction(const std::string& actionCommand);
  const std::string& getOnEntryAction();
  const std::string& getOnExitAction();

protected:

private:
  std::string onEntryAction;
  std::string onExitAction;

};

} // namespace ssm

#endif // _ACTION_CONTAINER_H_

