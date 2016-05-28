/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine-element.h
 * Copyright (C) 2016 Sandro Stiller <sandro@Wuerfelkiste>
 */

#ifndef _STATE_MACHINE_ELEMENT_H_
#define _STATE_MACHINE_ELEMENT_H_

#include <string>



namespace ssm // "Spinni state machine"
{
class StateMachine;

class StateMachineElement
{
public:
  struct PointerDocOrderCompare
  {
    bool operator()(const StateMachineElement* a, const StateMachineElement* b) const  { return(a->getElementNumber() < b->getElementNumber()); }
  };
  
  StateMachineElement(StateMachine* stateMachine, const std::string& name);
  StateMachine* getStateMachine();
  const std::string& getName() const;
  unsigned int getElementNumber() const;
private:
  StateMachine* stateMachine;
  std::string name;
  static unsigned int currentElementCount; ///< increased with every new created element
  unsigned int elementNumber; ///< the set to currentElementCount before increased (1st element = number 0)
};

} // namespace ssm

#endif // _STATE_MACHINE_ELEMENT_H_

