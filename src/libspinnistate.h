#ifndef _LIBSPINNISTATE_H
#define _LIBSPINNISTATE_H
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * libspinnistate.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 * 
 * libspinnistate is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libspinnistate is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */


#include <cstdint>

#define NOIMPLEMENT_INVOKE

namespace ssm
{

enum class StateType : uint8_t
{
  State = 0,
  Parallel,
  Final,
  Initial,
  HistoryDeep,
  HistoryShallow,
};

} // namespace ssm

#include "state.h"
//#include "state-machine.h"

#endif // _LIBSPINNISTATE_H
