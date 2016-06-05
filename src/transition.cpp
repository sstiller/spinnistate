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
                       const std::string& guard,
                       Transition::TransitionType transitionType)
: StateMachineElement(srcState->getStateMachine(), name),
  ActionContainer(getStateMachine()),
  srcState(srcState),
  guard(guard),
  transitionType(transitionType)
{
  //TODO: check name, trigger, condition (avoid never used transitions)
  
}

void Transition::addEvent(const Event& ev)
{
  event.push_back(ev);
}

bool Transition::conditionsSatisfied(const Event& activeEvent)
{
  std::cout << __func__ << "() in " << getName() << ": checking if \"" <<  activeEvent.getDescriptor() << "\" matches local events." << std::endl;
  // check trigger
  if(event.size())
  {
    bool eventMatches = false;
    for(auto currentEvent : event)
    {
      if(currentEvent.matches(activeEvent))
      {
        eventMatches = true;
        break;
      }
    }
    if(! eventMatches)
    {
      // we have events and they don't match.
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

bool Transition::execute(const Event& activeEvent)
{
  if(!conditionsSatisfied(activeEvent))
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
  return State::findLCCA(tStates.toList().append(srcState));
}

OrderedSet<State*> Transition::getEffectiveTargetStates() const
{
  OrderedSet<State*> targets;
  for(auto s : target)
  {
    if(s->isHistoryState())
    {
      auto historyValue = getStateMachine()->getHistoryValue(s);
      if(! historyValue.empty())
      {
        /*TODO: Error in the standard?
         * HistoryValue is a List:
         *   procedure exitStates(enabledTransitions):
         *     ...
         *     historyValue[h.id] = configuration.toList().filter(f)
         * But...
         *   datatype OrderedSet
         *     ...
         *     procedure union(s)            // Adds all members of s that are not already members of the set (s must also be an OrderedSet)
         *
         * I will add a unite() accepting a list.
         */
        targets.unite(historyValue);
      }else
      {
        auto defaultTransition = s->findExecutibleTransition(Event());
        if(defaultTransition)
        {
          targets.unite(defaultTransition->getEffectiveTargetStates());
        }
        //TODO: else error (missing transition)
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


