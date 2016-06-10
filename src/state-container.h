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

// local includes
#include "libspinnistate.h"
#include "list.h"
#include "state-machine-element.h"

namespace ssm // "Spinni state machine"
{
class StateMachine;
class State;

class StateContainer : public StateMachineElement
{
public:
  StateContainer(StateMachine* stateMachine, const std::string& name, StateContainer* parent);
  virtual ~StateContainer() = default;
  
  /** get a state from the container
   * @param name the name of the wanted state
   * @param create if true, the requested state is created and added to lhe local container if not existing
   * @throws std::out_of_range if the requested state is not existing and create == false 
   * @return reference to the requested state
   */
  virtual State* getState(const std::string name, StateType stateType = StateType::State, bool create = false) = 0;

  StateContainer* getParent() const;

  /** Returns 'true' if state1 is a descendant of state2.
    * (a child, or a child of a child, or a child of a child of a child, etc.)
    * Otherwise returns 'false'.
    */
  bool isDescendantOf(const StateContainer* other) const;
  bool isSCXMLElement() const;

  List<State*> getHistory() const;
  
  protected:
  /** Create a new state and put it to the local container.
   *  The state name is checked for existance in the stateMachine before adding.
   *  After creation the new state is announced to the state machine.
   * @param name the name of the wanted state
   * @param parentState the parent state the new state belongs to 
   * @throws std::logic_error if a state with the given name already exists 
   * @return reference to the requested state
   */
  State* addState(const std::string name, StateType stateType);

  /** get a state from the container
   * @param name the name of the wanted state
   * @throws std::out_of_range if the requested state is not existing 
   * @return reference to the requested state
   */
  State* findState(const std::string& name);

  bool containsStates() const;

protected:
  std::map<std::string, std::shared_ptr<State> > existingStates;
  State* entryState;

private:
  StateContainer* parentContainer; ///< stateMachine if no parent state
};

} // namespace ssm

#endif // _STATE_CONTAINER_H_

