/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * transition.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */


#include "state.h"
#include "transition.h"

namespace ssm // "Spinni state machine"
{

Transition::Transition(const std::string& name,
                       State* srcState,
                       State* dstState,
                       const std::string& triggerName,
                       const std::string& guard)
: ActionContainer(srcState->getStateMachine()),
  name(name),
  srcState(srcState),
  dstState(dstState),
  triggerName(triggerName),
  guard(guard)
{
  //TODO: check name, trigger, condition (avoid never used transitions)
}

Transition::~Transition()
{
  // TODO: Add implementation here
}

} // namespace ssm


