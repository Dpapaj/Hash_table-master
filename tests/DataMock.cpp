/*!
 ***************************************************************************************************
 * \file    DataMock.cpp
 * \author  Tomas Jurena
 * \brief   Mock for data interface
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/* Includes --------------------------------------------------------------------------------------*/
#include "DataMock.h"

extern "C" {
#include "mymalloc.h"
}

#include <string.h>

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
#define UNUSED(x) (void)x

/* Private function declarations -----------------------------------------------------------------*/
static int cmp(void *lhs, void *rhs);
static unsigned hash(void *data);

/* Private variables -----------------------------------------------------------------------------*/
static void (*mDestroyFnc)(void *);
static Data_Functions_t mFunctions = {.cmp = cmp,
                                      .hash = hash,
                                      .to_string = NULL,
                                      .from_string = NULL,
                                      .get_type = NULL,
                                      .destruct = NULL};

/* Exported functions definitions ----------------------------------------------------------------*/
Data_t *DataMock_Create(std::string value, int a, int b, int c)
{
  UNUSED(a);
  UNUSED(b);
  UNUSED(c);
  auto *data = (DataMock_t *)myMalloc(sizeof(DataMock_t));
  strcpy(data->value, value.c_str());
  data->fncs = &mFunctions;
  // this is hacky but it is a mock after all
  data->fncs->destruct = mDestroyFnc;
  return (Data_t *)data;
}

void DataMock_SetDestroyFunction(void (*pFunc)(void *))
{
  mDestroyFnc = pFunc;
}

/* Private function definitions ------------------------------------------------------------------*/
static int cmp(void *lhs, void *rhs)
{
  return strcmp(((DataMock_t *)lhs)->value, ((DataMock_t *)rhs)->value);
}

static unsigned hash(void *data)
{
  char *uk = ((DataMock_t *)data)->value;
  unsigned result = 0;

  while (*uk != '\0') {
    result += *uk++;
  }

  return result;
}