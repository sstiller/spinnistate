/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * chai-script-data-model.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#ifndef _CHAI_SCRIPT_DATA_MODEL_H_
#define _CHAI_SCRIPT_DATA_MODEL_H_

// ChaiScript http://chaiscript.com/
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

// local includes
#include "data-model.h"

namespace ssm // "Spinni state machine"
{

class ChaiScriptDataModel: public DataModel 
{
public:
  ChaiScriptDataModel();
  ~ChaiScriptDataModel();

  bool evaluateBool(const std::string& expression) override;
  void executeAction(const std::string& actionString) override;

private:
  chaiscript::ChaiScript chai;

};

} // namespace ssm

#endif // _CHAI_SCRIPT_DATA_MODEL_H_

