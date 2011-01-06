/**
 * @file bconst.h
 * @brief constants for BKIT
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @date 2010-2012
 */

#ifndef _BCONST_H_INC
#define _BCONST_H_INC


#define _KB		* 1024
#define _MB		* 1024 * 1024
#define _kB		* 1000
#define _mB		* 1000 * 1000

#define BK_STDOUT	0
#define BK_STDIN	1
#define BK_STDERR	2

#define BK_CHR_NULL	'\0'
#define BK_CHR_EOL	'\n'
#define BK_CHR_0	'0'
#define BK_CHR_9	'9'
#define BK_CHR_A	'A'
#define BK_CHR_Z	'Z'
#define BK_CHR_a	'a'
#define BK_CHR_z	'z'

#define BK_STR_EOL	"\n"
#define BK_STR_NULL	""

#define BK_INT_ZERO	0
#define BK_FLO_ZERO	0.0f
#define BK_NUM_BASE2	2
#define BK_NUM_BASE10	10
#define BK_NUM_TEST	1234567890


#define BK_NUM_BASE	BK_NUM_BASE10

/**
 * @remark Test and Set Macro Constants
 * true
 * false
 * NULL
 * ZERO
 * ZERO_F
 */

#ifndef true
#define true		1
#endif

#ifndef false
#define false		!(true)
#endif

#ifndef NULL
#define NULL		'\0'
#endif

#ifndef ZERO
#define ZERO		0
#endif

#ifndef ZERO_F
#define ZERO_F		0.0
#endif


#endif	/* _BCONST_H_INC */
