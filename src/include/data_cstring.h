/*!
 * \file    data_cstring.h
 * \author  Tomas Dulik
 * \date    9/3/2015
 * \brief   Headers of Data_Cstring_t specialization of Data_t abstract type
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef DATA_CSTRING_H_
#define DATA_CSTRING_H_
/*! \addtogroup data
 *  \{
 */
/*! \defgroup dataCstring Data C-String
 *  \brief This module is a specification of the \ref Data_t interface that handles the c-string.
 *  \{
 */
/* Includes --------------------------------------------------------------------------------------*/
#include "data.h"

/* Exported types --------------------------------------------------------------------------------*/
/*! Definition of the C-string specification of the \ref Data_t. */
typedef struct {
  /*! Pointer to the table of virtual functions. */
  const Data_Functions_t *fncs;

  /*! Pointer to the stored data. */
  char cstring[256];
} Data_Cstring_t;

/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
/*! Initialize the \a obj instance.
 *
 * \param[out]  obj The object to inizialize.
 * \param[in]   str Optional string that is copied into the instance. If not passed, the empty
 * string is created.
 */
void Data_Cstring_Init(Data_Cstring_t *obj, const char *str);

/*! Allocates memory for new object instance on the heap and initialize it with \a str.
 *
 * \param[in] str String that is copied into the created object.
 *
 * \return Pointer to the created object.
 *
 * \sa Data_Cstring_Init
 */
Data_t *Data_Cstring_New(const char *str);

/*! Creates new instance of the object on the stack, initialize it with \a str, and returns it to
 * the caller.
 *
 * \param[in] str String that is copied into the created object.
 *
 * \return Returns the created object.
 *
 * \sa Data_Cstring_Init
 */
Data_Cstring_t Data_Cstring_Create(const char *str);

/*! \} */
/*! \} */

#endif /* DATA_CSTRING_H_ */
