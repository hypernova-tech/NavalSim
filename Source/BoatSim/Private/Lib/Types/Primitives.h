// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

#if defined(_MSC_VER)  // MSVC compiler
#define BYTE_ALIGNED_BEGIN  __pragma(pack(push, 1))
#define BYTE_ALIGNED_END    __pragma(pack(pop))
#elif defined(__GNUC__)  // GCC compiler
#define BYTE_ALIGNED_BEGIN
#define BYTE_ALIGNED_END    __attribute__((aligned(1)))
#else
#error "Unsupported compiler"
#endif




typedef char INT8S;
typedef unsigned char INT8U;

typedef unsigned short INT16U;
typedef short INT16S;


typedef unsigned int INT32U;
typedef  int INT32S;


typedef unsigned _int64 INT64U;
typedef  _int64 INT64S;

typedef float FLOAT32;
typedef  double FLOAT64;


#define RADTODEG							57.295779513082323 
#define DEGTORAD							0.017453292519943
#define WORLDTOUNREALUNIT				    100
#define UNREALTOWORLDLUNIT				    0.01

#define WORLD_TO_UNREAL(val) (val * WORLDTOUNREALUNIT)
#define UNREAL_TO_WORLD(val) (val * UNREALTOWORLDLUNIT)

#define TOW(val) UNREAL_TO_WORLD(val)
#define TOUE(val) WORLD_TO_UNREAL(val)


enum ECoordAxis
{
	CoordAxisUndef,
	CoordAxisX,
	CoordAxisY,
	CoordAxisZ,
	
};
enum ECamView {
	CamViewTop,
	CamViewLeft,
	CamViewRight,
	CamViewFront,
	CamViewBack,
	CamViewPerspective
};

