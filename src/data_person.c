/*!
 * \file    data_person.c
 * \author  Tomas Dulik
 * \date    9/3/2015
 * \brief   Implementation of Data_Person_t specialization of Data_t abstract
 * type
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/*! \addtogroup data
 *  \{
 */
/*! \addtogroup dataPerson
 *  \{
 */
/* Includes --------------------------------------------------------------------------------------*/
#include "data_person.h"

#include "data.h"
#include "mymalloc.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
static char *data_person_get_type();
static int data_person_cmp(void *d1, void *d2);
static unsigned data_person_hash(void *data);
static char *data_person_to_string(void *data, char *str);
static void data_person_from_string(char *str, void *data);

/* Private variables -----------------------------------------------------------------------------*/
static const Data_Functions_t data_person_fncs = {
  .cmp = data_person_cmp,
  .hash = data_person_hash,
  .to_string = data_person_to_string,
  .from_string = data_person_from_string,
  .get_type = data_person_get_type,
  .destruct = NULL  // no need for special destructor => Data_Destruct will call free()
};

/* Exported functions definitions ----------------------------------------------------------------*/
void Data_Person_Init(Data_Person_t *data, const char *str, double age, double weight, double height)
{
  assert(data != NULL);
  data->fncs = &data_person_fncs;

  if (str != NULL) {
    // trim the string to 255 char, d.cstring has only 256 bytes
    strncpy(data->name, str, sizeof(data->name));
    data->name[sizeof(data->name) - 1] = '\0';  // if trimmed, insert the terminating 0.
  } else {
    data->name[0] = '\0';
  }

  data->age = age;
  data->height = height;
  data->weight = weight;
}

Data_t *Data_Person_New(const char *str, double age, double weight, double height)
{
  Data_Person_t *data = myMalloc(sizeof(Data_Person_t));
  Data_Person_Init(data, str, age, weight, height);
  return (Data_t *)data;
}

Data_Person_t Data_Person_Create(char *str, double age, double weight, double height)
{
  Data_Person_t new_data;
  Data_Person_Init(&new_data, str, age, weight, height);
  return new_data;
}

/* Private function definitions ------------------------------------------------------------------*/
static char *data_person_get_type()
{
  return "Data_Person";
}

static int data_person_cmp(void *d1, void *d2)
{
  Data_Person_t *p1 = d1, *p2 = d2;  // just a "shortcut" - typecasting alias...
  int scmp = strcmp(p1->name, p2->name);

  if (scmp != 0) {
    return scmp;
  } else {
    int acmp = p1->age - p1->age;

    if (acmp != 0) {
      return acmp;  // you can continue with other comparison criteria..
    } else {
      return 0;
    }
  }
}

static unsigned data_person_hash(void *data)
{
  Data_Person_t *p = data;  // just a "shortcut" - typecasting alias...
  char *ptr = p->name;
  char *end =
    (char *)&(p->height) + sizeof(p->height);  // points right behind the end of the data structure
  unsigned result = 0;

  while (ptr < end) {  // the hash is a simple sum of all bytes of the structure
    result += *ptr++;
  }

  return result;
}

static char *data_person_to_string(void *data, char *str)
{
  Data_Person_t *d = data;

  if (str != NULL) {
    sprintf(str, "%.255s, %0.1lf, %0.1lf, %0.1lf", d->name, d->age, d->weight, d->height);
    return str;
  } else {
    return "data_person_to_string: error - null pointer. ";
  }
}

static void data_person_from_string(char *str, void *data)
{
  (void)str;
  (void)data;
  /*!
   * TODO finish this!!
   */
}

/*! \} */
/*! \} */