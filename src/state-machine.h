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
#include "state.h"
#include "state-container.h"

namespace ssm // "Spinni state machine"
{

class StateMachine : public StateContainer
{
public:
  /** Constructor.
   * @param ioService the io_service for this state machine instance
   */
  StateMachine(boost::asio::io_service& ioService);
  virtual ~StateMachine();

protected:
  boost::asio::io_service& ioService; // the io_service for this state machine instance

private:

};


} // namespace ssm
#endif // _STATE_MACHINE_H_

