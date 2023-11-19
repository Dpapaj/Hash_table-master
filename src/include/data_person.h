/*!
 * \file    data_person.h
 * \author  Tomas Dulik
 * \date    9/3/2015
 * \brief   Headers of Data_Person_t specialization of Data_t abstract type
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
/*! \defgroup dataPerson Data person
 *  \brief This module is a specification of the \ref Data_t interface that handles the person.
 *  \{
 */
#ifndef DATA_PERSON_H_
#define DATA_PERSON_H_

/* Includes --------------------------------------------------------------------------------------*/
#include "data.h"

/* Exported types --------------------------------------------------------------------------------*/
/*! Type Data_Person_t is a data type with contant size and implementing the \ref Data_t interface.
 */
typedef struct {
  /*! Pointer to the table of virtual functions. */
  const Data_Functions_t *fncs;

  /*! Person's name. */
  char name[255];

  /*! Person's age. */
  double age;

  /*! Person's weight. */
  double weight;

  /*! Person's height. */
  double height;
} Data_Person_t;

/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/

/*! Initialize the \a obj instance.
 *
 * \param[out]  obj     The object to inizialize.
 * \param[in]   str     Name of the person. If not passed, the empty string is created.
 * \param[in]   age     The age of the person.
 * \param[in]   weight  The weight of the person.
 * \param[in]   height  The height of the person.
 */
void Data_Person_Init(Data_Person_t *obj, const char *str, double age, double weight, double height);

/*! Allocates memory for new object instance on the heap and initialize it with \a str, \a age, \a
 * weight, and \a height.
 *
 * \param[in]   str     Name of the person. If not passed, the empty string is created.
 * \param[in]   age     The age of the person.
 * \param[in]   weight  The weight of the person.
 * \param[in]   height  The height of the person.
 *
 * \return Pointer to the created object.
 * \sa  Data_Person_Init
 */
Data_t *Data_Person_New(const char *str, double age, double weight, double height);

/*! Creates new instance of the object on the stack, initialize it with \a str, \a age, \a
 * weight, \a height, and returns it to the caller.
 *
 * \param[in]   str     Name of the person. If not passed, the empty string is created.
 * \param[in]   age     The age of the person.
 * \param[in]   weight  The weight of the person.
 * \param[in]   height  The height of the person.
 *
 * \return  Returns the created object.
 * \sa  Data_Person_Init
 */
Data_Person_t Data_Person_Create(char *str, double age, double weight, double height);

#endif /* DATA_VALUE_H_ */

/*! \} */
/*! \} */