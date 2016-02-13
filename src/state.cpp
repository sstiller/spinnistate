/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */
#include "state.h"

namespace ssm // "Spinni state machine"
{

State::State(const std::string& name, State* parent)
: name(name),
  parentState(parent)
{
  if(name.size() == 0)
  {
    throw(std::invalid_argument("No valid name given!"));
  }
}

State::State(const State& srcState, State* parent)
: name(srcState.name),
  parentState(parent)
{
}

State::~State()
{
}

const std::string& State::getName() const
{
  return(name);
}

void State::setParent(State *parent)
{
  parentState = parent;
}

} // namespace ssm

