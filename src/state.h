/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#ifndef _STATE_H_
#define _STATE_H_

// stl
#include <string>

namespace ssm // "Spinni state machine"
{
  class State;
}

// local includes
#include "state-container.h"

namespace ssm // "Spinni state machine"
{

class State : public StateContainer
{
public:
  State() = delete;
  /** Create a new state
   * @param name the name of the new state
   * @throws std::invalid_argument if name not valid (empty)
   */
  State(const std::string& name, State* parent = nullptr);
  State(const State& srcState, State* parent = nullptr);
  virtual ~State();
  
  const std::string& getName() const;
protected:
  void setParent(State *parent);
//TODO add-Methoden hier überschreiben und parent setzen?
//TODO evtl parent als optionaler Parameter in StateContainer mit rein
private:
  std::string name;
  State *parentState;

};

} // namespace ssm

#endif // _STATE_H_

