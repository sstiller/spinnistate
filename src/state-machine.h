/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 */

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

// boost
#include <boost/asio.hpp>

namespace ssm // "Spinni state machine"
{
  class StateMachine;
}

// local includes
#include "data-model.h"
#include "state.h"
#include "state-container.h"

namespace ssm // "Spinni state machine"
{

class StateMachine : public StateContainer
{
public:
  StateMachine() = delete;
  StateMachine(const StateMachine&) = delete;
  /** Constructor.
   * @param ioService the io_service for this state machine instance
   */
  StateMachine(boost::asio::io_service& ioService, DataModel* dataModel);
  virtual ~StateMachine();

  State* getState(const std::string name, bool create = false) override;

  /** add a reference to a created state to the local list
   * @param newState the new state reference to store
   * @throws std::logic_error if a state with the same name already exists
   */
  void announceState(State* newState);
  bool stateExists(const std::string& name);

  /** returns a pointer to the data model.
   * Needed to evaluate conditions for transitions and to execute actions
   */
  DataModel* getDataModel();
  
  /** Starts the state machine and enters the entry state
   * @throws std::logic_error if there is no valid entry state
   */
  void start();
protected:
  /// the io_service for this state machine instance
  boost::asio::io_service& ioService;

  State* currentState; // nullptr if not started

private:
  /// needed to check if a state name already exists in a state machine and for faster access to states
  std::map <std::string, State*> statePointers;
  DataModel* dataModel;
};


} // namespace ssm
#endif // _STATE_MACHINE_H_

