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

class State : public StateMachineElement, public StateContainer, public ActionContainer
{
public:
  State() = delete;
  //State(State&&) = default;
  /** Create a new state
   * @param name the name of the new state
   * @param stateMachine the state machine the state belongs to
   * @throws std::invalid_argument if name or stateMachine not valid or a state with the same name already exists 
   */
  State(const std::string& name, StateMachine* stateMachine, State* parent = nullptr);
  virtual ~State();

  virtual State* getState(const std::string name, bool create = false) override;

  Transition* addTransition(const std::string& name, State* dstState, const std::string& triggerName, const std::string& guard);

  /** Look if a transition of this state is executible
   * @param event the currently active event (may be empty string)
   * @return pointer to the first transition with matching conditions or nullptr if none found
   */
  Transition* findExecutibleTransition(const std::string& event);
  State* getParent();

  void enter();
  void leave();

private:
  State* parentState; ///< nullptr if no parent state
  std::vector<std::unique_ptr<Transition> > transitions;
};

} // namespace ssm

#endif // _STATE_H_

