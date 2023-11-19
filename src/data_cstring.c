/*!
 * \file    data_cstring.c
 * \author  Tomas Dulik
 * \date    9/3/2015
 * \brief   Implementation of Data_Cstring_t specialization of Data_t abstract type
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
/*! \addtogroup dataCstring
 *  \{
 */

/* Includes --------------------------------------------------------------------------------------*/
#include "data_cstring.h"

#include "mymalloc.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
static char *data_cstring_get_type();
static int data_cstring_cmp(void *d1, void *d2);
static unsigned data_cstring_hash(void *data);
static char *data_cstring_to_string(void *data, char *str);
static void data_cstring_from_string(char *str, void *data);

/* Private variables -----------------------------------------------------------------------------*/
static const Data_Functions_t data_cstring_fncs = {
  .cmp = data_cstring_cmp,
  .hash = data_cstring_hash,
  .to_string = data_cstring_to_string,
  .from_string = data_cstring_from_string,
  .get_type = data_cstring_get_type,
  .destruct = NULL  // no need for special destructor => Data_Destruct will call free()
};

/* Exported functions definitions ----------------------------------------------------------------*/
void Data_Cstring_Init(Data_Cstring_t *data, const char *str)
{
  assert(data != NULL);
  data->fncs = &data_cstring_fncs;

  if (str != NULL) {
    // trim the string to 255 char, d.cstring has only 256 bytes
    strncpy(data->cstring, str, sizeof(data->cstring) - 1);
    // same as sprintf(data->cstring, "%.255s", str);
    data->cstring[sizeof(data->cstring) - 1] = '\0';  // if trimmed, insert the terminating 0.
  } else {
    data->cstring[0] = '\0';
  }
}

Data_t *Data_Cstring_New(const char *str)
{
  Data_Cstring_t *data = myMalloc(sizeof(Data_Cstring_t));
  Data_Cstring_Init(data, str);
  return (Data_t *)data;
}

Data_Cstring_t Data_Cstring_Create(const char *str)
{
  Data_Cstring_t newdata;
  Data_Cstring_Init(&newdata, str);
  return newdata;
}

/* Private function definitions ------------------------------------------------------------------*/
static char *data_cstring_get_type()
{
  return "Data_Cstring";
}

static int data_cstring_cmp(void *d1, void *d2)
{
  char *str1 = ((Data_Cstring_t *)d1)->cstring;
  char *str2 = ((Data_Cstring_t *)d2)->cstring;
  return strcmp(str1, str2);
}

static unsigned data_cstring_hash(void *data)
{
  char *uk = ((Data_Cstring_t *)data)->cstring;
  unsigned result = 0;

  while (*uk != '\0') {
    result += *uk++;
  }

  return result;
}

static char *data_cstring_to_string(void *data, char *str)
{
  Data_Cstring_t *result = data;

  if (str == NULL) {
    return result->cstring;
  } else {
    return strcpy(str, result->cstring);
  }
}

static void data_cstring_from_string(char *str, void *data)
{
  (void)str;
  (void)data;
  /*
   * TODO finish this!!
   */
}
