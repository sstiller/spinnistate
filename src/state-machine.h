/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 */

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_
// stl
#include <set>

// boost
#include <boost/asio.hpp>

// local includes
#include "ordered-set.h"
#include "data-model.h"
#include "state-container.h"
#include "state-machine-element.h"
//#include "transition.h"

namespace ssm // "Spinni state machine"
{
class Transition;

typedef std::set<Transition*, StateMachineElement::PointerDocOrderCompare> TransitionSet;
  
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
  
  /** Initializes the state machine and sets the init script for the data model.
   * The init script is executed after start() is called.
   * @param dataModelInitScript the initScript for the data model (called later after start of the SM!)
   * |throws std::logic_error if already initialized
   */
  void init(const std::string& dataModelInitScript);

  /* returns true if the SM is initialized (by init()).
   */
  bool isInitialized(); 

  /** Starts the state machine and enters the entry state.
   * @throws std::logic_error if there is no valid entry state or the SM is not initialized
   */
  void start();

  /** An event occured.
   * Here the matching transitions / actions are processed.
   * @todo rename to executeMacrostep?
   * @param eventName the occured event.
   */
  void processEvent(const std::string& eventName);

  /** Check if the state machine has finished
   * @return true if final state(s) reached 
   * @todo implement!
   */
  bool finalReached() const;

  const OrderedSet<State*>& getConfiguration() const;

protected:

  /** start with the current cative state and look for an executible transition
   * @param event the currently active event empty if searching for eventless transitions
   * @return pointers to the selected transitions or empty set if none found
   * @throws std::logic_error if no active state 
   */
  TransitionSet selectTransitions(const std::string& event = std::string());

  /** remove transitions which can not be executed in parall.
   * Applies only for parallel states
   */
  TransitionSet removeConflictingTransitions(const TransitionSet& transitions);
  
  /// the io_service for this state machine instance
  boost::asio::io_service& ioService;

  /** Contains all active atomic states.
   * This simple vector implements the SCXML machine configuration. 
   */
  OrderedSet<State*> configuration;

private:
  /// needed to check if a state name already exists in a state machine and for faster access to states
  std::map <std::string, State*> statePointers;
  DataModel* dataModel;
  bool stateMachineInitialized;
  std::string dataModelInitScript;
};


} // namespace ssm
#endif // _STATE_MACHINE_H_

