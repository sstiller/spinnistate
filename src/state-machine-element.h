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
  StateMachineElement(StateMachine* stateMachine, const std::string& name);
  StateMachine* getStateMachine();
  const std::string& getName() const;
private:
  StateMachine* stateMachine;
  std::string name;
};

} // namespace ssm

#endif // _STATE_MACHINE_ELEMENT_H_

