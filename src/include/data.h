/*!
 * \file    data.h
 * \author  Tomas Dulik
 * \date    9/3/2015
 * \brief   Headers of Data Abstract type
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef DATA_H_
#define DATA_H_
/*! \defgroup data Abstract data type
 *  \brief Abstract data type that defines interface for all derived types. It defines basic data
 * structure that contains table of virtual functions and the pointer to the data that are
 * associated with the given instance. Thanks to this we can store the pointers in a custom data
 * structure and use the provided API to call the derived functions without further distinguishing
 * the used type.
 *  \{
 */

/* Includes --------------------------------------------------------------------------------------*/
/* Exported types --------------------------------------------------------------------------------*/
/*! Defines the interface of the data type. All derived types shall instatiate this. */
typedef struct {
  /*! Compares the the instances of Data_t.
   *
   *  \param[in] d1 LHS
   *  \param[in] d2 RHS
   *
   *  \return Returns 0 if \a d1 equals \a d2, less than 0 if \a d1 is smaller than \a d2, otherwise
   * return value bigger than 0.
   */
  int (*cmp)(void *d1, void *d2);

  /*! Computes hash from the \a data.
   *
   *  \param[in]  data Instance of the object.
   *
   *  \return Returns the integer value of hash computed from the object.
   */
  unsigned (*hash)(void *data);

  /*! Converts the \a data object to the string. The result is stored in the \a str.
   *
   *  \param[in]  data  Instance of the object to convert.
   *  \param[out] str   The output buffer where the converted string is stored.
   *
   *  \return Returns the \a str if conversion is successful, otherwise it returns error message.
   */
  char *(*to_string)(void *data, char *str);

  /*! Converts the string to the object.
   *
   *  \param[in]  str   String to convert.
   *  \param[out] data  The object where to store the result.
   */
  void (*from_string)(char *str, void *data);

  /*! Gets the name of the type. */
  char *(*get_type)();

  /*! Destroys the object and frees all allocated memory.
   *
   *  \param[in]  data  Object to destroy.
   */
  void (*destruct)(void *data);
} Data_Functions_t;

/*! Basic structure of abstract data type. */
typedef struct {
  /*! Pointer to the table of virtual functions. */
  const Data_Functions_t *fncs;

  /*! Pointer to the stored data. */
  void *data;
} Data_t;

/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
/*! \copydoc Data_Functions_t.cmp */
int Data_Cmp(void *d1, void *d2);

/*! \copydoc Data_Functions_t.hash */
unsigned Data_Hash(void *data);

/*! \copydoc Data_Functions_t.to_string */
char *Data_To_String(void *data, char *str);

/*! \copydoc Data_Functions_t.from_string */
void Data_From_String(char *str, void *data);

/*! \copydoc Data_Functions_t.get_type */
char *Data_Get_Type(void *data);

/*! \copydoc Data_Functions_t.destruct */
void Data_Destruct(void *data);

/*! \} */

#endif /* DATA_VALUE_H_ */
