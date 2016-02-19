/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * transition.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */


#include "state.h"
#include "transition.h"

namespace ssm // "Spinni state machine"
{

Transition::Transition(const std::string& name,
                       State* srcState,
                       State* dstState,
                       const std::string& triggerName,
                       const std::string& guard)
: StateMachineElement(srcState->getStateMachine(), name),
  ActionContainer(getStateMachine()),
  srcState(srcState),
  dstState(dstState),
  triggerName(triggerName),
  guard(guard)
{
  //TODO: check name, trigger, condition (avoid never used transitions)
  
}

Transition::~Transition()
{
}

bool Transition::conditionsSatisfied(const std::string& activeEventName)
{
  // check trigger
  if(triggerName.size())
  {
    if(activeEventName != triggerName)
    {
      return(false);
    }
  }

  // check guard
  if(guard.size())
  {
    DataModel* dm = getStateMachine()->getDataModel();
    return(dm->evaluateBool(guard));
  }
  // no conditions --> execute always
 return(true);
}

bool Transition::execute(const std::string& activeEventName)
{
  if(!conditionsSatisfied(activeEventName))
  {
    return(false);
  }
  std::cout << __PRETTY_FUNCTION__ << " Would now change to state " << dstState->getName() << std::endl;
  //TODO: execute it!
  return(true);
}

} // namespace ssm


