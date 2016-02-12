/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state-machine.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#include "state-machine.h"

namespace ssm // "Spinni state machine"
{
  
StateMachine::StateMachine(boost::asio::io_service& ioService)
: ioService(ioService)
{
}

StateMachine::~StateMachine()
{
}

  
} // namespace ssm 
