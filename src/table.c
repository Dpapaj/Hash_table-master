/*!
 * \file       table.c
 * \author     Ondřej Ševčík
 * \date       6/2019
 * \brief      Implementation of functions for HashTable.
 * **************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/*! \addtogroup hashTable
 *  \{
 */
/* Includes --------------------------------------------------------------------------------------*/
#include "table.h"

#include "hash_private.h"
#include "mymalloc.h"

#include <stdio.h>
#include <string.h>

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
#define UNUSED(x) (void)x

/* Private variables -----------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
/* Exported functions definitions ----------------------------------------------------------------*/
bool HashTable_Init(HashTable *table, size_t size, bool deletecontents)
{
  if (table == NULL)
  {
    return false;
  }
  table->count = 0;
  table->take_ownership = deletecontents;
  table->size = size;
  table->buckets = myMalloc(size * sizeof(HashTableNode*));
  if (table->buckets == NULL) {
    return false;
  }
  for (size_t i = 0; i < size; i++) {
    table->buckets[i] = NULL;
  }
  return true;
}

void HashTable_Destruct(HashTable *table)
{
    if(table == NULL)
    {
        return;
    }

    myFree(table->buckets);
    table->size = 0;

    table->count = 0;
    table->buckets = 0;
}

bool HashTable_Insert(HashTable *table, Data_t *key, Data_t *value)
{
  if (table == NULL || key == NULL || value == NULL) {
    return false;
  }

  size_t i = hash(table, key);
  HashTableNode *node = table->buckets[i];
  while (node) {
    if (Data_Cmp(key, node->key) == 0) {
      return false;
    }
    node = node->next;
  }
  HashTableNode *newNode = myMalloc(sizeof(HashTableNode));
  if (!newNode) {
    return false;
  }
  newNode->key = key;
  newNode->value = value;
  newNode->next = table->buckets[i];
  table->buckets[i] = newNode;
  table->count++;
  return true;
}

bool HashTable_Replace(HashTable *table, Data_t *key, Data_t *value)
{
    if(table == NULL || key == NULL || value == NULL)
    {
        return false;
    }

    unsigned int i = hash(table, key);
    HashTableNode *nodeSec = table->buckets[i];
    while(nodeSec != NULL)
    {
        if(Data_Cmp(key, nodeSec->key) == 0)
        {
            if(table->take_ownership)
            {
                Data_Destruct(nodeSec->value);
            }
            nodeSec->value = value;
            return true;
        }
        nodeSec = nodeSec->next;
    }
    return false;
}

bool HashTable_Delete(HashTable *table, Data_t *key)
{
    if(table == NULL || key == NULL)
    {
        return false;
    }

    unsigned int x = hash(table, key);

    HashTableNode *prev= NULL;
    HashTableNode *node = table->buckets[x];

    while(node != NULL)
    {
        int y = Data_Cmp(node->key,key);
        if(y == 0)
        {
            if(prev)
            {
                prev->next = node->next;
                if(table->take_ownership)
                {
                    Data_Destruct(node->value);
                    Data_Destruct(node->key);
                }
            }
            else
            {
                table->buckets[x] = node->next;
                if(table->take_ownership)
                {
                    Data_Destruct(node->value);
                    Data_Destruct(node->key);
                }
            }
            table->count--;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}

Data_t *HashTable_Find(HashTable *table, Data_t *key)
{
    if(table == NULL || key == NULL)
    {
        return NULL;
    }

    unsigned int i = hash(table,key);

    HashTableNode * nodeFourth = table->buckets[i];
    while(nodeFourth != NULL)
    {

        if(Data_Cmp(key, nodeFourth->key) == 0)
        {

            return nodeFourth->value;
        }
        else
        {
            nodeFourth=nodeFourth->next;
        }
    }
    return NULL;
}

size_t HashTable_Get_Count(HashTable table)
{
   return table.count;
}

void HashTable_Clear(HashTable *table)
{
    if(table == NULL)
    {
        return;
    }

    unsigned int i;
    for(i = 0; i < table->size; i++)
    {
        HashTableNode* nodeFifth = table->buckets[i];
        if(nodeFifth != NULL)
        {
            HashTableNode *nodeNext;
            while(nodeFifth != NULL)
            {
                nodeNext = nodeFifth->next;
                if (table->take_ownership == true)
                {
                    Data_Destruct(nodeFifth->key);
                    Data_Destruct(nodeFifth->value);
                }
                myFree(nodeFifth);
                nodeFifth = nodeNext;
            }
            table->buckets[i] = NULL;
        }
    }
    table->count = 0;
}

void HashTable_Process(HashTable *table, TableNodeProc proc)
{
  if (table == NULL || proc == NULL) {
    return;
  }
  for (size_t i = 0; i < table->size; i++) {
    HashTableNode *node = table->buckets[i];
    while (node) {
      proc(node->key, node->value);
      node = node->next;
    }
  }
}

/* Private function definitions ------------------------------------------------------------------*/

/*! \} */
