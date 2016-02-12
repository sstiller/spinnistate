/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * state.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */
#include "state.h"

namespace ssm // "Spinni state machine"
{

State::State(const std::string& name)
: name(name)
{
  if(name.size() == 0)
  {
    throw(std::invalid_argument("No valid name given!"));
  }
}

State::~State()
{
}

const std::string& State::getName() const
{
  return(name);
}

} // namespace ssm

