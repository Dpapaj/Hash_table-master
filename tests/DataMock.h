/*!
 ***************************************************************************************************
 * \file    DataMock.h
 * \author  Tomas Jurena
 * \brief   Mock for data interface
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
#ifndef DATAMOCK_H
#define DATAMOCK_H

/* Includes --------------------------------------------------------------------------------------*/
#include "data.h"

#include <string>

/* Exported types --------------------------------------------------------------------------------*/
typedef struct {
  Data_Functions_t *fncs;
  char value[256];
} DataMock_t;

/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
Data_t *DataMock_Create(std::string value, int a = 0, int b = 0, int c = 0);
void DataMock_SetDestroyFunction(void (*pFunc)(void *));

#endif  // DATAMOCK_H
