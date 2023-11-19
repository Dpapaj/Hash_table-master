/*!
 * \file    hash_private.c
 * \author  Tomas Dulik
 * \date    9/3/2015
 * \brief   Declaration file of the hash module.
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
#include "hash_private.h"

unsigned int hash(HashTable *table, Data_t *key)
{
  return Data_Hash(key) % table->size;
}
