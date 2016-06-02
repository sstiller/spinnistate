/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * HashTable.h
 * Copyright (C) 2016 Sandro Stiller <sandro.stiller@gmx.de>
 *
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

// stl
#include <map>

namespace ssm
{

/** table[foo] returns the value associated with foo.
  * table[foo] = bar sets the value associated with foo to be bar.
  */
template <typename Tkey, typename Tvalue>
class HashTable
{
public:
  /** adds new entry if not existing. */
  Tvalue& operator[](const Tkey& key)
  {
    return(data[key]);
  }
  /** throws if entry not existing. */
  const Tvalue& at(const Tkey& key) const
  {
    return(data.at(key));
  }
private:
  std::map<Tkey, Tvalue> data;
};

} // namespace ssm

#endif // _HASHTABLE_H_

