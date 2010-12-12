/**
 * @file whetstone.c
 * @brief whetstone algorithm implementation for FLOPS
 *
 * Derived from a 1987 translation of whetstone from algol
 * designed to measure floating point performance. This is
 * heavily biased by floating point accelerators and 
 * soft-float emulators in various environments. However it
 * does give a good figure to measure floating point 
 * capability of a platform in a deterministic fashion.
 */

/* @remark standard libary include */
#include <math.h>

/* @remark betakit includes */
#include <whetstone.h>

/* @remark global variables */
double t, t1, t2, e1[5];
int j, k, l;


inline void wproc_a( double e[] )
{
  for(j = 0; j < 6; j++ )
    {
      	e[1] = ( e[1] + e[2] + e[3] - e[4]) * t;
	e[2] = ( e[1] + e[2] - e[3] + e[4]) * t;
	e[3] = ( e[1] - e[2] + e[3] + e[4]) * t;
	e[4] = (-e[1] + e[2] + e[3] + e[4]) / t2;
    }
}

inline void wproc_0( void )
{
  	e1[j] = e1[k];
	e1[k] = e1[l];
	e1[l] = e1[j];
}

inline void wproc_3(double x, double y, double *z)
{
	double x1, y1;

	x1 = x;
	y1 = y;
	x1 = t * (x1 + y1);
	y1 = t * (x1 + y1);
	*z  = (x1 + y1) / t2;

	return; /* wproc_3 */
}


inline void wfunc_1( long* n1, long* n2, long* n3, long* n4, long* n6, 
	     long* n7, long* n8, long* n9, long* n10,long* n11, long* loop )
{
  register long loops = 0;
  loops = (*loop);

  (*n1)  = 0;
  (*n2)  = 12 * loops;
  (*n3)  = 14 * loops;
  (*n4)  = 345 * loops;
  (*n6)  = 210 * loops;
  (*n7)  = 32 * loops;
  (*n8)  = 899 * loops;
  (*n9)  = 616 * loops;
  (*n10) = 0;
  (*n11) = 93 * loops;

  return; /* wfunc_1 */
}


inline void wfunc_2( double* x1, double* x2, double* x3, double* x4, long* n1 )
{
  register int i = 0;

  (*x1)  =  1.0;
  (*x2)  = -1.0;
  (*x3)  = -1.0;
  (*x4)  = -1.0;

  for (i = 1; i <= (*n1); i++) {
    (*x1) = ((*x1) + (*x2) + (*x3) - (*x4)) * t;
    (*x2) = ((*x1) + (*x2) - (*x3) + (*x4)) * t;
    (*x3) = ((*x1) - (*x2) + (*x3) + (*x4)) * t;
    (*x4) = (-(*x1)+ (*x2) + (*x3) + (*x4)) * t;
  }

  return; /* wfunc_2 */
}

t_s32 calculate_whetstones( int time, t_whetstone_result* wresults )
{
	long i;
	long n1, n2, n3, n4, n6, n7, n8, n9, n10, n11;
	double x1,x2,x3,x4,x,y,z;
	long loop, total_loops;
	int ii, jj;

	/** Betakit: Better Time Resolution for Accuracy */
	double start_time, end_time, user_time;

	/* added for this version */
	long loopstart;
	double mips = 0.0;
	double time_per_loop = 0.0;


	loopstart  = 100; /* check */
	total_loops = 0;
	start_time = d_time();

 whetrun:

	total_loops += loopstart;

	{
	  t  = 0.499975;
	  t1 = 0.50025;
	  t2 = 2.0;
	}

	{
	  loop = loopstart;
	  ii = 1;
	  jj = 1;
	}

 subloop:
	/* Loop Init */
	wfunc_1( &n1, &n2, &n3, &n4, &n6, &n7, &n8, &n9, &n10, &n11, &loop );

	/* Module 1 */
	wfunc_2( &x1, &x2, &x3, &x4, &n1 );

	/* Module 2 */
	{
	  e1[1] =  1.0;
	  e1[2] = -1.0;
	  e1[3] = -1.0;
	  e1[4] = -1.0;

	  for (i = 1; i <= n2; i++) {
	    e1[1] = ( e1[1] + e1[2] + e1[3] - e1[4]) * t;
	    e1[2] = ( e1[1] + e1[2] - e1[3] + e1[4]) * t;
	    e1[3] = ( e1[1] - e1[2] + e1[3] + e1[4]) * t;
	    e1[4] = (-e1[1] + e1[2] + e1[3] + e1[4]) * t;
	  }
	}

	/* Module 3 */
	{
	  for (i = 1; i <= n3; i++)
	    {
	      wproc_a(e1);
	    }
	}

	/* Module 4 */
	{
	  j = 1;
	  for (i = 1; i <= n4; i++) 
	    {
	      j = (j == 1) ? 2 : 3;
	      j = (j >  2) ? 0 : 1;
	      j = (j <  1) ? 1 : 0;
	    }
	}

	/* Module 6 */
	{
	  j = 1;
	  k = 2;
	  l = 3;

	  for (i = 1; i <= n6; i++) 
	    {
	      j = j * (k-j) * (l-k);
	      k = l * k - (l-j) * k;
	      l = (l-k) * (k+j);
	      e1[l-1] = j + k + l;
	      e1[k-1] = j * k * l;
	    }
	}

	/* Module 7 */
	{
	  x = 0.5;
	  y = 0.5;

	  for (i = 1; i <= n7; i++) {
	    x = t * atan(t2*sin(x)*cos(x)/(cos(x+y)+cos(x-y)-1.0));
	    y = t * atan(t2*sin(y)*cos(y)/(cos(x+y)+cos(x-y)-1.0));
	  }
	}

	/* Module 8 */
	{
	  x = 1.0;
	  y = 1.0;
	  z = 1.0;

	  for (i = 1; i <= n8; i++)
	    {
	      wproc_3(x,y,&z);
	    }
	}

	/* Module 9 */
	{
	  j = 1;
	  k = 2;
	  l = 3;
	  e1[1] = 1.0;
	  e1[2] = 2.0;
	  e1[3] = 3.0;

	  for (i = 1; i <= n9; i++)
	    {
	      wproc_0();
	    }
	}

	/* Module 10 */
	{
	  j = 2;
	  k = 3;

	  for (i = 1; i <= n10; i++) 
	    {
	      j = j + k;
	      k = j + k;
	      j = k - j;
	      k = k - j - j;
	    }
	}

	/* Module 11 */
	{
	  x = 0.75;

	  for (i = 1; i <= n11; i++)
	    {
	      x = sqrt(exp(log(x)/t1));
	    }
	}

	if( ++j <= ii ) 
	  {
	    goto subloop;
	  }

	/** --<done run>-- **/

	end_time = d_time();
	user_time = end_time - start_time;

	if( user_time < (double)time )
	  {
	    goto whetrun;
	  }


	mips = ((100 * total_loops * ii)/ (float) user_time)/WLOOPS_PER_MIPS;
	time_per_loop = user_time / (float) (total_loops);
	
	wresults->real_time_taken = user_time;
	wresults->whetstones = mips;
	wresults->loop_msecs = time_per_loop * (t_u32)(WLOOPS_PER_MIPS); /* check */
	wresults->total_loops = total_loops;

	return(0);
}

/* @remark end of file "whetstone.c" */
