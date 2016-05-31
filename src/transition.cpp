/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * transition.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#include <functional>   // std::bind
// local includes
#include "state.h"
#include "transition.h"

namespace ssm // "Spinni state machine"
{

Transition::Transition(const std::string& name,
                       State* srcState,
                       const std::string& triggerName,
                       const std::string& guard,
                       Transition::TransitionType transitionType)
: StateMachineElement(srcState->getStateMachine(), name),
  ActionContainer(getStateMachine()),
  srcState(srcState),
  triggerName(triggerName),
  guard(guard),
  transitionType(transitionType)
{
  //TODO: check name, trigger, condition (avoid never used transitions)
  
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
  //std::cout << __PRETTY_FUNCTION__ << " Would now change to states " << dstState->getName() << std::endl;
  //TODO: execute it!
  return(true);
}

State* Transition::getSrcState() const
{
  return(srcState);
}

std::vector<State*> Transition::getTarget() const
{
  return(target);
}

void Transition::addTarget(State* targetState)
{
  target.push_back(targetState);
}

bool Transition::isTargetless() const
{
  return(target.empty());
}


OrderedSet<State*> Transition::computeExitSet() const
{
  OrderedSet<State*> statesToExit;
  if(!isTargetless())
  {
    auto domain = getTransitionDomain();
    for(auto s : getStateMachine()->getConfiguration())
    {
      if(domain->isAncestorOf(s))
      {
        statesToExit.addElement(s);
      }
    }
  }
  return(statesToExit);   
}

State* Transition::getTransitionDomain() const
{
  auto tStates = getEffectiveTargetStates();
  if(tStates.empty())
  {
    return(nullptr);
  }else if((isInternal()) &&
           (srcState->isCompoundState()) &&
           (tStates.every(std::bind(&State::isAncestorOf, srcState, std::placeholders::_1)))
           //(tstates.every(lambda s: isDescendant(s,srcState)))
          )
  {
    return(srcState);
  }
  // else
  //TODO: add findLCCA to... State as static?
  return findLCCA(tStates.toList().append(srcState));
}

OrderedSet<State*> Transition::getEffectiveTargetStates() const
{
  OrderedSet<State*> targets;
  for(auto s : target)
  {
    if(s->isHistoryState())
    {
      if(historyValue[s.id])
      {
        targets.union(historyValue[s.id])
      }else
      {
        targets.union(getEffectiveTargetStates(s.transition))
      }
    }else
    {
      targets.addElement(s);
    }
  }
  return(targets);
}

bool Transition::isInternal() const
{
  return(transitionType == TransitionType_Internal);
}


} // namespace ssm


