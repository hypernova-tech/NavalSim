// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <stdint.h>

#if defined(_MSC_VER)  // MSVC compiler
#define BYTE_ALIGNED_BEGIN  __pragma(pack(push, 1))
#define BYTE_ALIGNED_END    __pragma(pack(pop))
#elif defined(__GNUC__)  // GCC compiler
#define BYTE_ALIGNED_BEGIN
#define BYTE_ALIGNED_END   __attribute__((packed)) //__attribute__((aligned(1)))
#else
#error "Unsupported compiler"
#endif


/**
 * 
 */

/**
 *
 */

#define RADTODEG							57.295779513082323 
#define DEGTORAD							0.017453292519943


typedef char INT8S;
typedef unsigned char INT8U;

typedef unsigned short INT16U;
typedef short INT16S;


typedef unsigned int INT32U;
typedef  int INT32S;


typedef  uint64_t INT64U;
typedef  int64_t INT64S;

typedef float FLOAT32;
typedef  double FLOAT64;


#ifdef __linux__
#define IS_LINUX() 1
#define IS_WIN() 0
#else
#define IS_LINUX() 0
#define IS_WIN() 1
#endif


