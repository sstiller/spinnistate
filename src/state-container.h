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
#include "state-machine.h"
#include "state.h"


namespace ssm // "Spinni state machine"
{

class StateContainer
{
public:
  StateContainer(StateMachine* stateMachine);
  virtual ~StateContainer();
  
  /** get a state from the container
   * @param name the name of the wanted state
   * @param create if true, the requested state is created and added to lhe local container if not existing
   * @throws std::out_of_range if the requested state is not existing and create == false 
   * @return reference to the requested state
   */
  virtual State* getState(const std::string name, bool create = false) = 0;
  
  protected:
  /** Create a new state and put it to the local container.
   *  The state name is checked for existance in the stateMachine before adding.
   *  After creation the new state is announced to the state machine.
   * @param name the name of the wanted state
   * @param parentState the parent state the new state belongs to 
   * @throws std::logic_error if a state with the given name already exists 
   * @return reference to the requested state
   */
  State* addState(const std::string name, State* parentState);

  /** get a state from the container
   * @param name the name of the wanted state
   * @throws std::out_of_range if the requested state is not existing 
   * @return reference to the requested state
   */
  State* findState(const std::string& name);

protected:
  std::map<std::string, State> existingStates;
  State* entryState;

private:
  StateMachine* stateMachine;
};

} // namespace ssm

#endif // _STATE_CONTAINER_H_

