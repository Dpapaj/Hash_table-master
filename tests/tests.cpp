/*!
 * \file       tests.cpp
 * \author     Jurena
 * \date       9. 2. 2022
 * \brief      Unit tests for hash table module.
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/* Includes --------------------------------------------------------------------------------------*/

#include "DataMock.h"
#include "fff.h"
#include "gmock/gmock.h"

#include "gtest/gtest.h"
#include <inttypes.h>
#include <limits>
#include <string.h>
#include <vector>

extern "C" {
#include "data_cstring.h"
#include "data_person.h"
#include "hash_private.h"
#include "table.h"
}

/* Private macros --------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------*/
DEFINE_FFF_GLOBALS
FAKE_VOID_FUNC(MockDestroy, void *)
FAKE_VOID_FUNC(process_table_node, Data_t *, Data_t *)

/* Private types ---------------------------------------------------------------------------------*/
class HashTableTest : public ::testing::Test
{
protected:
  void SetUp()
  {
    HashTable_Init(&mTable, 10, true);
    DataMock_SetDestroyFunction(&MockDestroy);
    RESET_FAKE(MockDestroy);
    RESET_FAKE(process_table_node);
  }
  void Teardown()
  {
    HashTable_Clear(&mTable);
    HashTable_Destruct(&mTable);
  }

  HashTable mTable;
};

/* Private function declarations -----------------------------------------------------------------*/
/* Exported functions definitions ----------------------------------------------------------------*/
/* Private function definitions ------------------------------------------------------------------*/
TEST(HashTest, init)
{
  HashTable table = {true, nullptr, 100, 100};

  ASSERT_TRUE(HashTable_Init(&table, 10, false));
  ASSERT_EQ(table.size, 10);
  ASSERT_EQ(table.count, 0);
  ASSERT_FALSE(table.take_ownership);

  std::array<void *, 10> expected;
  expected.fill(nullptr);
  ASSERT_THAT(std::vector<HashTableNode *>(table.buckets, table.buckets + 10),
              ::testing::ElementsAreArray(expected));
}

TEST(HashTest, initNull)
{
  HashTable_Init(NULL, 1000, false);
}

TEST(HashTest, destruct)
{
  HashTable table;
  HashTable_Init(&table, 10, false);

  HashTable_Destruct(&table);
  ASSERT_EQ(table.buckets, nullptr);
  ASSERT_EQ(table.count, 0);
  ASSERT_EQ(table.size, 0);
}

TEST(HashTest, destructNull)
{
  HashTable_Destruct(nullptr);
}

TEST(HashTest, insert)
{
  HashTable table;
  HashTable_Init(&table, 10, true);
  Data_t *testingKey = DataMock_Create("testKey");
  size_t i = hash(&table, testingKey);
  Data_t *person = DataMock_Create("Trillian McMillan", 12, 13, 15);

  ASSERT_TRUE(HashTable_Insert(&table, testingKey, person));
  ASSERT_EQ(table.buckets[i]->key, testingKey);
  ASSERT_EQ(table.buckets[i]->value, person);
}

TEST(HashTest, insertTwoValuesInOneBucket)
{
  HashTable table;
  HashTable_Init(&table, 10, true);
  char testBuffer[256] = "Test Name";
  Data_t *testingKey = DataMock_Create("testKey");
  size_t i = hash(&table, testingKey);
  Data_t *firstPerson = DataMock_Create(testBuffer, 12, 13, 15);

  Data_t *secondPerson = DataMock_Create(testBuffer, 16, 17, 18);

  /* Inserting item with unique key and unique data ---------------------- */
  HashTable_Insert(&table, testingKey, firstPerson);

  testingKey = DataMock_Create("tostKey");
  ASSERT_TRUE(HashTable_Insert(&table, testingKey, secondPerson));
  ASSERT_EQ(table.buckets[i]->key, testingKey);
  ASSERT_EQ(table.buckets[i]->value, secondPerson);
  ASSERT_EQ(table.buckets[i]->next->value, firstPerson);
}

TEST(HashTest, insertDuplicateKey)
{
  HashTable table;
  HashTable_Init(&table, 10, true);

  Data_t *testingKey = DataMock_Create("testKey");
  Data_t *firstPerson = DataMock_Create("Zaphpod Beeblebrox", 16, 17, 18);
  Data_t *secondPerson = DataMock_Create("Arthur Dent", 16, 17, 18);
  auto i = hash(&table, testingKey);
  HashTable_Insert(&table, testingKey, firstPerson);

  ASSERT_FALSE(HashTable_Insert(&table, testingKey, secondPerson));
  ASSERT_EQ(table.buckets[i]->key, testingKey);
  ASSERT_EQ(table.buckets[i]->value, firstPerson);
  ASSERT_EQ(table.buckets[i]->next, nullptr);
}

TEST(HashTest, insertNulls)
{
  HashTable table;
  HashTable_Init(&table, 10, true);

  Data_t *testingKey = DataMock_Create("testKey");
  Data_t *personTestValue = DataMock_Create("Ford Prefect", 16, 17, 18);

  ASSERT_FALSE(HashTable_Insert(NULL, testingKey, personTestValue));
  ASSERT_FALSE(HashTable_Insert(&table, NULL, personTestValue));
  ASSERT_FALSE(HashTable_Insert(&table, testingKey, NULL));
  ASSERT_FALSE(HashTable_Insert(NULL, NULL, NULL));
}

TEST(HashTest, clear)
{
  HashTable table;
  HashTable_Init(&table, 10, true);
  DataMock_SetDestroyFunction(&MockDestroy);

  std::unordered_map<Data_t *, Data_t *> pairs{
    {DataMock_Create("Heart of Gold"), DataMock_Create("Arthur Dent", 16, 17, 18)},
    {DataMock_Create("Magrathea"), DataMock_Create("Garkbit", 16, 17, 18)},
    {DataMock_Create("Vogsphere"), DataMock_Create("Effrafax of Wug", 16, 17, 18)},
    {DataMock_Create("Milliways"), DataMock_Create("Elders of Krikkit", 16, 17, 18)}};

  for (const auto &pair : pairs) {
    HashTable_Insert(&table, pair.first, pair.second);
  }

  HashTable_Clear(&table);
  ASSERT_EQ(MockDestroy_fake.call_count, 8);
  ASSERT_EQ(table.count, 0);
}

TEST(HashTest, clearNull)
{
  HashTable_Clear(nullptr);
}

TEST_F(HashTableTest, replaceWithOwnership)
{
  std::unordered_map<Data_t *, Data_t *> pairs{
    {DataMock_Create("testKey"), DataMock_Create("Marvin", 16, 17, 18)},
    {DataMock_Create("tostKey"), DataMock_Create("Slartibartfast", 16, 17, 18)}};
  size_t i = hash(&mTable, pairs.cbegin()->first);
  for (const auto &pair : pairs) {
    HashTable_Insert(&mTable, pair.first, pair.second);
  }

  auto newValue = (++pairs.cbegin())->second;

  ASSERT_TRUE(HashTable_Replace(&mTable, pairs.cbegin()->first, newValue));
  ASSERT_EQ(mTable.buckets[i]->value, newValue);
  ASSERT_EQ(mTable.buckets[i]->next->value, newValue);
  ASSERT_EQ(MockDestroy_fake.call_count, 1);
}

TEST_F(HashTableTest, replaceWithoutOwnership)
{
  mTable.take_ownership = false;
  std::unordered_map<Data_t *, Data_t *> pairs{
    {DataMock_Create("testKey"), DataMock_Create("Deep Thought", 16, 17, 18)},
    {DataMock_Create("tostKey"), DataMock_Create("Agrajag", 16, 17, 18)}};
  size_t i = hash(&mTable, pairs.cbegin()->first);
  for (const auto &pair : pairs) {
    HashTable_Insert(&mTable, pair.first, pair.second);
  }

  auto newValue = (++pairs.cbegin())->second;

  ASSERT_TRUE(HashTable_Replace(&mTable, pairs.cbegin()->first, newValue));
  ASSERT_EQ(mTable.buckets[i]->value, newValue);
  ASSERT_EQ(mTable.buckets[i]->next->value, newValue);
  ASSERT_EQ(MockDestroy_fake.call_count, 0);
}

TEST_F(HashTableTest, replaceNonExistingKey)
{
  Data_t *testingKey = DataMock_Create("testKey");
  Data_t *testingKey1 = DataMock_Create("testKey1");
  Data_t *firstPerson = DataMock_Create("Deep Thought", 12, 13, 15);
  Data_t *secondPerson = DataMock_Create("Agrajag", 12, 13, 15);
  HashTable_Insert(&mTable, testingKey, firstPerson);

  ASSERT_FALSE(HashTable_Replace(&mTable, testingKey1, secondPerson));
}
TEST(HashTest, replaceNull)
{
  HashTable_Replace(NULL, NULL, NULL);
}

TEST_F(HashTableTest, findExistingNode)
{
  Data_t *key = DataMock_Create("testKey");
  Data_t *firstPerson = DataMock_Create("Eddie the Computer", 12, 13, 15);
  HashTable_Insert(&mTable, key, firstPerson);

  ASSERT_EQ(HashTable_Find(&mTable, key), firstPerson);
}

TEST_F(HashTableTest, findNonExistingNode)
{
  Data_t *key = DataMock_Create("testKey");
  Data_t *firstPerson = DataMock_Create("Eddie the Computer", 12, 13, 15);
  HashTable_Insert(&mTable, key, firstPerson);

  Data_t *unknownKey = DataMock_Create("unknown");

  ASSERT_EQ(HashTable_Find(&mTable, unknownKey), nullptr);
}

TEST(HashTest, findNull)
{
  ASSERT_EQ(HashTable_Find(NULL, NULL), nullptr);
}

TEST_F(HashTableTest, deleteKnownKey)
{
  Data_t *key = DataMock_Create("testKey");
  Data_t *person = DataMock_Create("Lallafa", 12, 13, 15);

  HashTable_Insert(&mTable, key, person);

  ASSERT_TRUE(HashTable_Delete(&mTable, key));
  ASSERT_EQ(HashTable_Find(&mTable, key), nullptr);
  ASSERT_EQ(MockDestroy_fake.call_count, 2);
}

TEST_F(HashTableTest, deleteKnownKeyAtTheEndOfList)
{
  Data_t *keys[] = {DataMock_Create("testKey"), DataMock_Create("tostKey")};
  Data_t *people[] = {DataMock_Create("Loonquawl", 12, 13, 15),
                      DataMock_Create("Know-Nothing-Bozo", 12, 13, 15)};

  for (size_t i = 0; i < sizeof(keys) / sizeof(*keys); ++i) {
    HashTable_Insert(&mTable, keys[i], people[i]);
  }

  ASSERT_TRUE(HashTable_Delete(&mTable, keys[0]));
  ASSERT_EQ(HashTable_Find(&mTable, keys[1]), people[1]);
}

TEST_F(HashTableTest, deleteKnownKeyInTheMiddleOfList)
{
  Data_t *keys[] = {
    DataMock_Create("testKey"), DataMock_Create("tostKey"), DataMock_Create("tystKey")};
  Data_t *people[] = {DataMock_Create("Wowbagger Ultrajax", 12, 13, 15),
                      DataMock_Create("Yooden Vranx", 12, 13, 15),
                      DataMock_Create("Zarniwoop", 12, 13, 15)};

  for (size_t i = 0; i < sizeof(keys) / sizeof(*keys); ++i) {
    HashTable_Insert(&mTable, keys[i], people[i]);
  }

  ASSERT_TRUE(HashTable_Delete(&mTable, keys[1]));
  ASSERT_EQ(HashTable_Find(&mTable, keys[1]), nullptr);
  ASSERT_EQ(HashTable_Find(&mTable, keys[0]), people[0]);
  ASSERT_EQ(HashTable_Find(&mTable, keys[2]), people[2]);
}

TEST_F(HashTableTest, deleteUnknownKey)
{
  Data_t *keys[] = {
    DataMock_Create("testKey"), DataMock_Create("tostKey"), DataMock_Create("tystKey")};
  Data_t *people[] = {DataMock_Create("Wowbagger Ultrajax", 12, 13, 15),
                      DataMock_Create("Yooden Vranx", 12, 13, 15)};

  for (size_t i = 0; i < sizeof(people) / sizeof(*people); ++i) {
    HashTable_Insert(&mTable, keys[i], people[i]);
  }

  ASSERT_FALSE(HashTable_Delete(&mTable, keys[2]));
}

TEST_F(HashTableTest, deleteNull)
{
  Data_t *testingKey = DataMock_Create("testKey");
  ASSERT_FALSE(HashTable_Delete(NULL, testingKey));
  ASSERT_FALSE(HashTable_Delete(&mTable, NULL));
  ASSERT_FALSE(HashTable_Delete(NULL, NULL));
}

TEST_F(HashTableTest, processTable)
{
  Data_t *keys[] = {DataMock_Create("testKey"),
                    DataMock_Create("tostKey"),
                    DataMock_Create("tystKey"),
                    DataMock_Create("anotherKey"),
                    DataMock_Create("moreKeys")};
  Data_t *people[] = {
    DataMock_Create("Wowbagger Ultrajax", 12, 13, 15),
    DataMock_Create("Yooden Vranx", 12, 13, 15),
    DataMock_Create("Zarniwoop", 12, 13, 15),
    DataMock_Create("Judiciary Pag", 12, 13, 15),
    DataMock_Create("Hig Hurtenflurst", 12, 13, 15),
  };
  for (size_t i = 0; i < sizeof(people) / sizeof(*people); ++i) {
    HashTable_Insert(&mTable, keys[i], people[i]);
  }

  HashTable_Process(&mTable, process_table_node);
  ASSERT_EQ(process_table_node_fake.call_count, 5);
  ASSERT_EQ(process_table_node_fake.arg0_history[0], keys[3]);
  ASSERT_EQ(process_table_node_fake.arg0_history[1], keys[2]);
  ASSERT_EQ(process_table_node_fake.arg0_history[2], keys[1]);
  ASSERT_EQ(process_table_node_fake.arg0_history[3], keys[0]);
  ASSERT_EQ(process_table_node_fake.arg0_history[4], keys[4]);
}

TEST_F(HashTableTest, getCountMultipleOperations)
{
  Data_t *keys[] = {
    DataMock_Create("testKey"), DataMock_Create("tostKey"), DataMock_Create("tystKey")};
  Data_t *people[] = {
    DataMock_Create("Hotblack Desiato", 12, 13, 15),
    DataMock_Create("Vroomfondel", 12, 13, 15),
    DataMock_Create("Teaser", 12, 13, 15),
  };

  for (size_t i = 0; i < sizeof(people) / sizeof(*people); ++i) {
    HashTable_Insert(&mTable, keys[i], people[i]);
  }

  ASSERT_EQ(HashTable_Get_Count(mTable), 3);
  HashTable_Delete(&mTable, keys[0]);
  ASSERT_EQ(HashTable_Get_Count(mTable), 2);
  HashTable_Delete(&mTable, keys[0]);
  ASSERT_EQ(HashTable_Get_Count(mTable), 2);
}