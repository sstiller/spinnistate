/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine-element.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro@Wuerfelkiste>
 */

#include <stdexcept>

#include "state-machine-element.h"

namespace ssm // "Spinni state machine"
{

StateMachineElement::StateMachineElement(StateMachine* stateMachine, const std::string& name)
: stateMachine(stateMachine),
  name(name)
{
  // TODO: name may be empty here?
  if(!stateMachine)
  {
    throw(std::invalid_argument("StateMachine nullptr."));
  }
}

StateMachine* StateMachineElement::getStateMachine()
{
  return(stateMachine);
}

const std::string& StateMachineElement::getName() const
{
  return(name);
}


} // namespace ssm

