/*!
 * \file    hash_private.h
 * \author  Tomas Dulik
 * \date    9/3/2015
 * \brief   Definition file of the hash module.
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/*! \addtogroup hashTable
 *  \{
 */
/*! \defgroup hashTableHash Hash function
 *  \{
 */
#ifndef HASH_PRIVATE_H
#define HASH_PRIVATE_H

#include "data.h"
#include "table.h"

/*! Computes the index to the hash table from the given key. The index is computed as sum of the
 * bytes in the key and this value is divided table size using the modulo operation.
 *
 * \param[in] table Pointer to the table.
 * \param[in] key   Pointer to the key.
 *
 * \return The computed index.
 */
unsigned int hash(HashTable *table, Data_t *key);

#endif

/*! \} */
/*! \} */