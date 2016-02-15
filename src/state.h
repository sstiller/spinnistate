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

namespace ssm // "Spinni state machine"
{
  class State;
}

// local includes
#include "state-machine.h"
#include "state-container.h"
#include "transition.h"
#include "action-container.h"

namespace ssm // "Spinni state machine"
{

class State : public StateContainer, public ActionContainer
{
public:
  State() = delete;
  //State(State&&) = default;
  /** Create a new state
   * @param name the name of the new state
   * @param stateMachine the state machine the state belongs to
   * @throws std::invalid_argument if name or stateMachine not valid or a state with the same name already exists 
   */
  State(const std::string& name, StateMachine& stateMachine, State* parent = nullptr);
  virtual ~State();

  virtual State* getState(const std::string name, bool create = false) override;

  void addTransition(const std::string& name, State* dstState, const std::string& triggerName, const std::string& guard);
  const std::string& getName() const;
  State* getParent();
  StateMachine& getStateMachine();

  void enter();
  void leave();

private:
  std::string name; ///< name of this state
  StateMachine& stateMachine; ///< needed for actions and evaluations of transition conditions 
  State* parentState; ///< nullptr if no parent state
  std::vector<Transition> transitions;
};

} // namespace ssm

#endif // _STATE_H_

