/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * data-model.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 */

#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_

// stl
#include <string>

namespace ssm // "Spinni state machine"
{
  
class DataModel
{
public:
  DataModel();
  virtual ~DataModel();

  // to be implemented by inheriting classes
  virtual bool evaluateBool(const std::string& expression) = 0; //TODO: replace by template function for return value?
  virtual void executeAction(const std::string& actionString) = 0; // TODO: throw StateMachine::exception on error and catch it
  /** Init the DataModel */
  virtual void init(); // TODO: throw StateMachine::exception on error and catch it

/*TODO:
 * Virtual class for implementation
 * - create variables for actions / event evaluation (types?)
 * - evaluate boolean operations for transitions
 * - execute actions (strings)
 * - provide interface for builtin actions (timers, log, events emit by internal action)
 * - provide access to variables from outside the SM
 * - Ideas for DataModels
 *   - ChaiScript (I think I will try this)
       --> return value type using template: double d = chai.eval<double>("function(3, 4.75);");
       --> Calling C++ functions no problem
       --> Sharing variables using add? http://chaiscript.com/docs/4/classchaiscript_1_1_chai_script.html
       --> Memory usage 5MB http://discourse.chaiscript.com/t/chaiscript-memory-usage/48
     - muParser http://muparser.beltoforion.de/ (a fast mathematical expression evaluator)
       --> sharing variables no problem, but only double supported
       --> Calling C++ functions possible (but it would be some kind of misusage because the functions are used for calculation)
     - Lua http://www.lua.org/ (available in uscxml)
       --> Sharing variables seems possible: http://stackoverflow.com/questions/6297245/how-to-directly-map-a-lua-variable-to-a-c-variable
       --> Memory usage seems  < 1MB ... Source?
     - ECMA (preferred in SCXML standard)
       --> Maybe using vp8? (used in uscxml)
       --> V8 memory usage min 12MB http://stackoverflow.com/questions/11438833/v8-memory-usage
 */ 

};

} // namespace ssm

#endif // _DATA_MODEL_H_

