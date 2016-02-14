/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * data-model.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 */

#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_

namespace ssm // "Spinni state machine"
{
  
class DataModel
{
public:
  DataModel();
  virtual ~DataModel();
/*TODO:
 * Virtual class for implementation
 * - create variables for actions / event evaluation (types?)
 * - evaluate boolean operations for transitions
 * - execute actions (strings)
 * - provide interface for builtin actions (timers, log, events emit by internal action)
 * - provide access to variables from outside the SM
 * - 
 */ 
  
protected:

private:

};

} // namespace ssm

#endif // _DATA_MODEL_H_

