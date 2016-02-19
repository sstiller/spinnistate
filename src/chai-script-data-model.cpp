/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * chai-script-data-model.cpp
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

// stl
#include <iostream>

// local includes
#include "chai-script-data-model.h"

namespace ssm // "Spinni state machine"
{

ChaiScriptDataModel::ChaiScriptDataModel()
: chai(chaiscript::Std_Lib::library())
{
  // TODO: add functions to ChaiScript here
}

ChaiScriptDataModel::~ChaiScriptDataModel()
{
}

bool ChaiScriptDataModel::evaluateBool(const std::string& expression)
{
  //std::cout << __PRETTY_FUNCTION__ << " called (" << expression << ")" << std::endl;
  //TODO: catch exception and throw StateMachineException
  return(chai.eval<bool>(expression));
  
}

void ChaiScriptDataModel::executeAction(const std::string& actionString)
{
  if(actionString.empty())
  {
    return;
  }
  //TODO: forward to data model
  std::cout << __PRETTY_FUNCTION__ << "() Implement me: execute \"" << actionString << "\"." << std::endl;
}

} // namespace ssm
