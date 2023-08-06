// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

typedef bool BOOLEAN;

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