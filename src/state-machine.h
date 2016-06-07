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
#include "libspinnistate.h"
#include "ordered-set.h"
#include "hash-table.h"
#include "queue.h"
#include "event.h"
#include "data-model.h"
#include "state-container.h"
#include "state-machine-element.h"
//#include "transition.h"

namespace ssm // "Spinni state machine"
{
class Transition;

//typedef std::set<Transition*, StateMachineElement::PointerDocOrderCompare> TransitionSet;
typedef OrderedSet<Transition*> TransitionSet;
  
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

  State* getState(const std::string name, StateType stateType = StateType::State, bool create = false) override;

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

  /** External events are published to the SM using this function.
   * The processing will be done asyncronously.
   * The matching transitions / actions are processed.
   * Calls executeMacrosteps
   * @param eventName the occured event.
   */
  void processExternalEvent(const Event externalEvent);

  /** Process internal and external events
   * Here the matching transitions / actions are processed.
   * @param eventName the occured event.
   */
  void executeMacrosteps(const Event& externalEvent = Event());

  /** Check if the state machine has finished
   * @return true if final state(s) reached 
   * @todo implement!
   */
  bool finalReached() const;

  const OrderedSet<State*>& getConfiguration() const;

  List<State*> getHistoryValue(const State* state) const;
  void setHistoryValue(const State* state, List<State*>);

  /** Called by states after calling onEntry function.
   */
  void setStateActive(State* state);

  /** Called by states after calling onExit function.
   */
  void resetStateActive(State* state);

  void printConfiguration() const;
protected:

  /** start with the current cative state and look for an executible transition
   * @param event the currently active event empty if searching for eventless transitions
   * @return pointers to the selected transitions or empty set if none found
   * @throws std::logic_error if no active state 
   */
  TransitionSet selectTransitions(const Event& event = Event());

  /** remove transitions which can not be executed in parall.
   * Applies only for parallel states
   */
  TransitionSet removeConflictingTransitions(const TransitionSet& transitions);

  void microstep(List<Transition*> enabledTransitions);
  void exitStates(List<Transition*>&enabledTransitions);
  void executeTransitionContent(List<Transition*>&enabledTransitions);
  void enterStates(List<Transition*>&enabledTransitions);
  
  /// the io_service for this state machine instance
  boost::asio::io_service& ioService;

  /** Contains all active atomic states.
   * Implements the SCXML machine configuration. 
   */
  OrderedSet<State*> configuration;
#ifndef NOIMPLEMENT_INVOKE
#error implement invoke stuff here
  OrderedSet<State*> statesToInvoke;
#endif  
  /** Internal event queue */
  Queue<Event> internalQueue;

  // TODO: remove
  // /** External event queue */
  // Queue<Event> externalQueue;
  
private:
  /// needed to check if a state name already exists in a state machine and for faster access to states
  std::map <std::string, State*> statePointers;
  DataModel* dataModel;
  bool stateMachineInitialized;
  std::string dataModelInitScript;
  HashTable<const State*, List<State*> > historyValue;
  bool running;
};


} // namespace ssm
#endif // _STATE_MACHINE_H_

