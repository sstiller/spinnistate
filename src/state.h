/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#ifndef _STATE_H_
#define _STATE_H_

// stl
#include <string>
#include <memory>
#include <vector>

// local includes
#include "state-machine-element.h"
#include "state-container.h"
#include "action-container.h"
#include "state-machine.h"
#include "transition.h"

namespace ssm // "Spinni state machine"
{
class Transition;

class State : public StateContainer, public ActionContainer
{
public:
  State() = delete;
  //State(State&&) = default;
   // TODO: add parameters list instead of so many arguments?
  /** Create a new state
   * @param name the name of the new state
   * @param stateMachine the state machine the state belongs to
   * @throws std::invalid_argument if name or stateMachine not valid or a state with the same name already exists
   */
  State(const std::string& name, StateMachine* stateMachine, StateContainer* parent = nullptr, StateType stateType = StateType::State);
  virtual ~State();

  virtual State* getState(const std::string name, StateType stateType = StateType::State, bool create = false) override;

  Transition* addTransition(const std::string& name, const std::string& guard);

  /** Look if a transition of this state is executible.
   * If no transition is found, it looks in the anchestors.
   * @param event the currently active event (may be empty string)
   * @return pointer to the first transition with matching conditions or nullptr if none found
   */
  Transition* findExecutibleTransition(const Event& event);
  
  /** returns true if the state is a real ancestor of the other state.
   * Returns fals if not ancestor or this == other
   */
  bool isAncestorOf(const State* other) const;

  StateType getStateType() const;

  /** returns true if the state is a parallel state. */
  bool isParallel() const;

  /** returns true if the state is a virtual entry state. */
  bool isInitial() const;

  bool isHistoryState() const;
  bool isCompoundState() const;
  bool isFinal() const;
  bool isAtomicState() const;

  /** If state2 is null, returns the set of all ancestors of state1 in ancestry
    * order.
    * (state1's parent followed by the parent's parent, etc. up to an including
    * the <scxml> element). If state2 is non-null, returns in ancestry order the
    * set of all ancestors of state1, up to but not including state2. (A "proper
    * ancestor" of a state is its parent, or the parent's parent, or the parent's
    * parent's parent, etc.))If state2 is state1's parent, or equal to state1, or
    * a descendant of state1, this returns the empty set.
    */ 
  List<State*> getProperAncestors(const State* state2) const;

  
  void enter();
  void leave();

  /** finds the Least Common Compound Ancestor
    * The Least Common Compound Ancestor is the <state> or <scxml> element s
    * such that s is a proper ancestor of all states on stateList and no
    * descendant of s has this property. Note that there is guaranteed to be
    * such an element since the <scxml> wrapper element is a common ancestor of
    * all states. Note also that since we are speaking of proper ancestor
    * (parent or parent of a parent, etc.) the LCCA is never a member of
    * stateList.
    */ 
  StateContainer* findLCCA(List<State*> stateList);
  
private:
  std::vector<std::unique_ptr<Transition> > transitions;
  StateType stateType;
};

} // namespace ssm

#endif // _STATE_H_

