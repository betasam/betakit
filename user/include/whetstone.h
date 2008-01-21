/**
 * @file whetstone.h
 * @brief support for whetstone.c
 */

#ifndef _WHETSTONE_H_INC
#define _WHETSTONE_H_INC

double d_time( void );

typedef struct whetstone_result_s {
  double real_time_taken;
  double whetstones;
  double loop_msecs;
  long total_loops;
} whetstone_result_t;

int calculate_whetstones( int time, whetstone_result_t* wresults );

#endif /* _WHETSTONE_H_INC */
