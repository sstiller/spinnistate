/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-container.h
 * Copyright (C) 2016 Sandro Stiller <sandro@Wuerfelkiste>
 *
 */

#ifndef _STATE_CONTAINER_H_
#define _STATE_CONTAINER_H_

// stl
#include <map>
#include <memory> // shared_ptr

namespace ssm // "Spinni state machine"
{
  class StateContainer;
}

// local includes
#include "state.h"


namespace ssm // "Spinni state machine"
{

class StateContainer
{
public:
  StateContainer();
  virtual ~StateContainer();

  /** Adds a new (sub)state to the container.
   * @param newState The state to add. The state is copied into the local container.
   * @throws std::logic_error if a state with the same name already exists
   */
  void addState(const State& newState);

  /** Adds a new (sub)state to the container.
   * @param newState The state to add. container now owns the state and deletes it if not needed anymore.
   * @throws std::logic_error if a state with the same name already exists
   */
  void addState(State* newState);
  
  /** Creates a new (sub)state and adds it to the container.
   * If a state with the given name already exists, no new state is created.
   * @param name The name of the new state.
   */
  void addState(const std::string& name) noexcept;
  
  /** get a state from the container
   * @param name the name of the wanted state
   * @param create if true, the requested state is created and added to lhe local container if not existing
   * @throws std::out_of_range if the requested state is not existing and create == false 
   */
  std::shared_ptr<State> getState(const std::string name, bool create = false);
  
protected:
  std::map<std::string, std::shared_ptr<State> > containedStates;

private:

};

} // namespace ssm

#endif // _STATE_CONTAINER_H_

