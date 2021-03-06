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
#include "event.h"
#include "state-machine-element.h"
#include "action-container.h"
#include "ordered-set.h"

namespace ssm // "Spinni state machine"
{
class State;

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
class Transition: public StateMachineElement, public ActionContainer 
{
public:
  enum TransitionType
  {
    TransitionType_External,
    TransitionType_Internal
  };
  Transition(const std::string& name,
             State* srcState,
             const std::string& guard,
             TransitionType transitionType = TransitionType_External);

  Transition() = delete;
  ~Transition() = default;

  void addEvent(const Event& ev);
  
  /** tests if the conditions of the event are satisfied
   * If the transition contains no event name, just the guard is checked.
   * If the event contains an event name, it is compared with the given one and
   * only is executed if the name matches and the condition (if any) is is
   * satisfied.
   * @param eventName the name of the current event (or or empty string)
   * @returns true if the condition is satisfied */
  bool conditionsSatisfied(const Event& activeEvent);

  /** exectues the transition if  conditionsSatisfied returns true.
   * @param eventName the name of the current event (or empty string)
   * @returns true if the transition was executed */
  bool execute(const Event& activeEvent);

  State* getSrcState() const;
  std::vector<State*> getTarget() const;
  void addTarget(State* targetState);
  bool isTargetless() const;
  
  OrderedSet<State*> computeExitSet() const;
  StateContainer* getTransitionDomain() const;
  OrderedSet<State*> getEffectiveTargetStates() const;

  bool isInternal() const;
protected:

private:
  State* srcState;
  std::vector<State*> target;
  std::vector<Event> event;
  std::string guard; //< an expression evaluated by the data model
  TransitionType transitionType;
};

} // namespace ssm

#endif // _TRANSITION_H_

