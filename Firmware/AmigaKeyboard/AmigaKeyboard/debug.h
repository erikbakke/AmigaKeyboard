/*
 * debug.h
 *
 * Created: 16/02/2022 20:47:21
 *  Author: ebakke
 */ 

#include <stdio.h>

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG
#define debugf( args... ) printf( args )
#define debug( str ) printf( "%s", str )
#else
#define debugf( args... )
#define debug( str )
#endif


#endif /* DEBUG_H_ */