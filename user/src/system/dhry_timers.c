/**
 * @file dhry_timers.c
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @brief provides an accurate timestamp
 *
 * Provides a timestamp based on gettimeofday. This was
 * written after reading through timers.c of the dhry 2.1
 * algorithm implementation.
 */


/**
 * @brief system includes
 */
#include <sys/time.h>
#include <time.h>


/**
 * @fn double d_time( void )
 * @brief returns current time in microsecond precision
 * @return double value of time with microsecond precison
 * @warning this is strictly not threadsafe
 */
double d_time( void )
{
  register double d_ftime;
  struct timeval tv;
  struct timezone tz;

  d_ftime = 0.0;

  if((0 == gettimeofday( &tv, &tz )))
    {
      d_ftime = tv.tv_sec;
      d_ftime += tv.tv_usec * 1.0e-06;
    }

  return( d_ftime );
}

/* @remark end of "dhry_timers.c" */
