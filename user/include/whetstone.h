/**
 * @file whetstone.h
 * @brief support for whetstone.c
 */

#ifndef _WHETSTONE_H_INC
#define _WHETSTONE_H_INC

#include <btypes.h>

/* Betakit defaults */
#define WLOOPS_PER_MIPS		1000.0
#define WHETSTONE_TEST_TIME	5.0

double d_time( void );

typedef struct whetstone_result_s {
  t_d64 real_time_taken;
  t_d64 whetstones;
  t_d64 loop_msecs;
  t_l32 total_loops;
} t_whetstone_result;

t_s32 calculate_whetstones( int time, t_whetstone_result* wresults );

#endif /* _WHETSTONE_H_INC */
