/*
 * STD_types.h
 *
 *  Created on: ٢٨‏/١٢‏/٢٠٢٠
 *      Author: Mohamed_EL_Gallad
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_


typedef unsigned char u8; //define unsigned 8 bit of integer data
typedef signed char   s8; //define signed 8 bit of integer data
typedef char          c8; //define a storage for character type

typedef unsigned int u16; //define unsigned 16 bit of integer data
typedef signed int   s16; //define signed 16 bit of integer data

typedef unsigned long int u32; //define unsigned 32 bit of integer data
typedef signed long int   s32; //define signed 32 bit of integer data

typedef unsigned long long int u64;
typedef double f64;

typedef float f32; //define a float data type - 32 bit

#define TRUE   1
#define FALSE   0

#define ENABLE   1
#define DISABLE  0

#define NULL ((void *)0)


#endif /* STD_TYPES_H_ */
