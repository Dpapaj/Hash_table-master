/*!
 * \file       table.h
 * \author     Ondřej Ševčík
 * \date       6/2019
 * \brief      Header file for Hashtable.
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/*! \defgroup hashTable Hash table
 *  \{
 */
#ifndef _TABLE_H_
#define _TABLE_H_

/* Includes --------------------------------------------------------------------------------------*/
#include "data.h"

#include <stdbool.h>
#include <stddef.h>

/* Exported types --------------------------------------------------------------------------------*/
/*! One item stored in the \ref HashTable. */
typedef struct _HashTableNode {
  /*! Key of an item. */
  Data_t *key;

  /*! Value of an item. */
  Data_t *value;

  /*! Pointer at next item with the same index. */
  struct _HashTableNode *next;
} HashTableNode;

/*! Definition of the hash table container. */
typedef struct _HashTable {
  /*! Flag that specifies if the data in the node should be de-initialized when node is
   * destructed.
   */
  bool take_ownership;

  /*! Items of a table (Array of pointers to the items = items with same hash). */
  HashTableNode **buckets;

  /*! Size (Number of items) of an bucket array. */
  size_t size;

  /*! Number of items saved in container. */
  size_t count;

} HashTable;

/*! Callback function for processing of hash-table items.
 *
 * \param[in] key   Item's key.
 * \param[in] value Item's value.
 */
typedef void (*TableNodeProc)(Data_t *key, Data_t *value);

/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
/*! Initializes the hash table. It creates an array of pointers to items of the table (the \ref
 * HashTable.buckets), sets the \ref HashTable.size to \a size and the \ref HashTable.size to 0, and
 * sets every bucket pointer to NULL.
 *
 * \param[out]  table         Pointer at initialized hash-table.
 * \param[in]   size          Our desired size of table
 * \param[in]   takeOwnership If the argument takeOwnership is true, the value of item will be
 * deleted as well
 *
 * \return Returns true if table was initialized, otherwise it returns false.
 */
bool HashTable_Init(HashTable *table, size_t size, bool takeOwnership);

/*! Destructs the table. Frees all the nodes and sets the \ref HashTable.size and \ref
 * HashTable.count to 0.
 *
 * \param[in] table Pointer to the table we want to destruct.
 */
void HashTable_Destruct(HashTable *table);

/*! Creates a new item and puts it into table at the begin of the bucket that is selected by
 * computing the hash of the key. If there's an duplicate key, the insert won't happen.
 *
 * \param[in] table Pointer at the table where to store the key-value pair.
 * \param[in] key   Pointer at the key associated with the value.
 * \param[in] value Pointer at value that are inserted.
 *
 * \return Returns true if item was inserted, return false otherwise.
 */
bool HashTable_Insert(HashTable *table, Data_t *key, Data_t *value);

/*! Replaces an value of item associated with the \a key. If takeOwnership is true, the original
 * value is freed.
 *
 * \param[in] table Pointer at the table.
 * \param[in] key   Pointer at the item's key.
 * \param[in] value Pointer at the new value.
 *
 * \return Returns true if the item's data were replaced, otherwise returns false.
 */
bool HashTable_Replace(HashTable *table, Data_t *key, Data_t *value);

/*! Deletes an item that is associated with key. If there's no such an item, nothings happens. If
 * takeOwnership is true, removes the item's data as well. To remove data use \ref Data_Destruct
 * function defined in data.h.
 *
 * \param[in] table Pointer at the table.
 * \param[in] key   Pointer at item's key.
 *
 * \return Return true if the item was deleted, otherwise returns false.
 */
bool HashTable_Delete(HashTable *table, Data_t *key);

/*! Looks for the value associated with the \a key in the \a table.
 *
 * \param[in] table Pointer at the table.
 * \param[in] key   Pointer at the item's key.
 *
 * \return Returns the pointer to the associated value if \a key is found, otherwise returns NULL.
 */
Data_t *HashTable_Find(HashTable *table, Data_t *key);

/*! Gets the count of the items in the table.
 *
 * \param[in] table Pointer at the table.
 *
 * \return Returns number of items in table.
 */
size_t HashTable_Get_Count(HashTable table);

/*! Remove every item from table. If takeOwnership is true, also removes associated data. To remove
 * data use \ref Data_Destruct function defined in data.h as well.
 *
 * \param[in] table Pointer at the table.
 */
void HashTable_Clear(HashTable *table);

/*! Process every item in the \a table with \a proc function.
 *
 * \param[in] table Pointer at the table.
 * \param[in] proc Pointer at callback function.
 */
void HashTable_Process(HashTable *table, TableNodeProc proc);

#endif  //_TABLE_H_

/*! \} */