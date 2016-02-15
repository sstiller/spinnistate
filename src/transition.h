/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * transition.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#ifndef _TRANSITION_H_
#define _TRANSITION_H_

// stl
#include <string>

// local includes
#include "action-container.h"
#include "state.h"

namespace ssm // "Spinni state machine"
{

/** A transition to change the current state.
 * A transition is "executed" if
 * - trigger is empty and guard evaluates to true
 * - a trigger event with the correct name raises and
 *   + the guard is empty or
 *   + the guard evaluates to true
 * If the transition is executed,
 * - the onExit action of srcState is executed
 * - the onEntry of this trigger is called
 * - the onEntry function of dstState is executed
 */
class Transition: public ActionContainer 
{
public:
  Transition(const std::string& name,
             State* srcState,
             State* dstState,
             const std::string& triggerName,
             const std::string& guard);
  ~Transition();

protected:

private:
  std::string name;
  State* srcState;
  State* dstState;
  std::string triggerName;
  std::string guard;
};

} // namespace ssm

#endif // _TRANSITION_H_

