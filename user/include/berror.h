/**
 * @file	berror.h
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @date	2010-2012
 * @remark	Copyright 2005-2012 Sunil Beta Baskar <betasam@gmail.com>
 */

#ifndef _BERROR_H_INC
#define _BERROR_H_INC

#include <bkconfig.h>

#ifdef CONFIG_BK_SYS_ERRORHANDLER

#include <btypes.h>

#ifndef BKIT_NODEFAULTS
#define BERROR_FLUSHLOG_DEFAULTS	1
#endif


/**
 * @note
 * The following error codes
 * are for t_s32 retval, int to t_s32
 * is the common return type of functions
 * by default unless they are indicated
 * otherwise.
 */

/* Error Severity */
#define BERR_NORMAL		0x100
#define BERR_SEVERE		0x200

/* Error Codes */
#define BERR_UNKNOWN		0x01
#define BERR_NOMEM		0x02
#define BERR_INVALID		0x04
#define BERR_CRASH		0x08
#define BERR_NOSPACE		0x10

/* Error Levels */
#define BERR_LEVEL1		1
#define BERR_LEVEL2		2
#define BERR_LEVEL3		3
#define BERR_LEVEL4		4
#define BERR_LEVEL5		5

/**
 * @note
 * macro test for fundamental errors
 * indicated by t_s32 retval 
 *
 * This is the simplest error indication 
 * level.
 */
#define _IS_ERROR(x)		(0>x)

/**
 * @note
 * berror_reset(x) equivalent to berror_init
 */
#define berror_reset(x)		berror_init(x)

/**
 * @note
 * for the sake of convenience in using berror_log,
 * berr_log_lock(x) is berror_log_lock(x)
 * berr_log_unlock(x) is berror_log_unlock(x)
 */
#define berr_log_lock(x)	berror_log_lock(x)
#define berr_log_unlock(x)	berror_log_unlock(x)

/**
 * @note
 * The following error codes
 * are modeled on radio reporting
 * codes, commonly referred to as
 * 10-codes. The major "type" of 
 * an error is usually "0x10" BCD
 * and for extended errors, "0x11"
 *
 * TYPE0	0x000	no error to report
 * TYPE1	0x010	error can be handled
 * TYPE2	0x011	severe, execution halted
 * TYPE3	0x012	user defined error
 * TYPE4	0x013	simulated error or test
 */
#define BERR_TYPE0		0x0000
#define BERR_TYPE1		0x0010
#define BERR_TYPE2		0x0011
#define BERR_TYPE3		0x0012
#define BERR_TYPE4		0x0013

#define BERR_ENONE		0x0000	
#define BERR_ENOMEM		0x0001
#define BERR_EINVAL		0x0002
#define BERR_ECRASH		0x0004
#define BERR_DLOCKD		0x0008
#define BERR_EDIVZR		0x0010
#define BERR_ENULLP		0x0020
#define BERR_EPARSE		0x0040
#define BERR_ESSEGV		0x0080
#define BERR_RIGHTS		0x0100
#define BERR_ANOMAL		0x0200
#define BERR_ELOGIT		0x0400
#define BERR_EDEADB		0xFFFF

#define BERR_NEGATIVE		-1
#define BERR_POSITIVE		1

#define BERR_SZ_LEN		512
#define BERR_SZ_HDRLEN		256

#define BERR_LOG_SIZE		1 _MB

#define BERR_LOGUNDEF		0xFF
#define BERR_LOGINIT		0x01
#define BERR_LOGLOCK		0x04
#define BERR_LOGFULL		0x10

#define IS_LOG_VALID(log)	(BERR_LOGUNDEF != (log->u8_flags & BERR_LOGUNDEF))

/* String constants for berror */
#define SZ_BERR_LOGPATH		"/tmp/"
#define SZ_BERR_LOGFILE		"berror.log"
#define SZ_BERR_DEFPREFIX	"betakit/error: "
#define SZ_BERR_DELIM		" "
#define SZ_BERR_EOL		"\n"

/* Error Description Strings */

/* Series 1: Retval interpretation */
#define SZ_BERR_UNKNOWN		"Unknown"
#define SZ_BERR_NOMEM		"Memory Allocation"
#define SZ_BERR_INVALID		"Invalid Status"
#define SZ_BERR_CRASH		"Framework Crash"
#define SZ_BERR_NOSPACE		"Insufficient Memory"

#define SZ_BERR_NORMAL		"Normal"
#define SZ_BERR_SEVERE		"Severe"

/* Series 2: t_berror interpretation, usage */

#define SZ_BERR_ENONE		"No Report"
#define SZ_BERR_ENOMEM		"No memory"
#define SZ_BERR_EINVAL		"Invalid"
#define SZ_BERR_ECRASH		"Crash"
#define SZ_BERR_DLOCKD		"Deadlock"
#define SZ_BERR_EDIVZR		"Division by Zero"
#define SZ_BERR_ENULLP		"Null Pointer"
#define SZ_BERR_EPARSE		"Parse Error"
#define SZ_BERR_ESSEGV		"Segmentation Fault"
#define SZ_BERR_RIGHTS		"Rights Insufficient"
#define SZ_BERR_ANOMAL		"Anomaly"
#define SZ_BERR_EDEADB		"Bit Error"
#define SZ_BERR_ELOGIT		"Logging requested"
#define SZ_BERR_DANGER		"Undefined Danger"

#define SZ_BERR_TYPE0		"Type 0 [Code 0x00] NONE"
#define SZ_BERR_TYPE1		"Type 1 [Code 0x10] ROUT"
#define SZ_BERR_TYPE2		"Type 2 [Code 0x11] STOP"
#define SZ_BERR_TYPE3		"Type 3 [Code 0x12] USER"
#define SZ_BERR_TYPE4		"Type 4 [Code 0x13] TEST"

/**
 * @struct	berror_struct
 * @brief	64-bit error indicator structure
 * @member	err_major	indicates major number of error
 * @member	err_minor	indicates minor number of error
 * @detail	The error indicator is intended for use
 *		using a single or minimal instance. The
 *		64-bit width is to accommodate additional
 *		error numbers that may be user defined and
 *		not explicitly provided by the library.
 */
struct berror_struct {
  t_s32	err_major;
  t_s32 err_minor;
};


/**
 * @struct	szerror_struct
 * @brief	641Byte error descriptor
 * @member	szerr_desc	error description string
 * @member	szerr_head	header error description
 * @member	log_error	boolean error
 * @detail	error descriptor structure that explains
 *		a single error. This is for holding a 
 *		string describing the error just before
 *		it is sent to a log file.
 */
struct szerror_struct {
  const char szerr_desc[BERR_SZ_LEN];
  const char szerr_head[BERR_SZ_HDRLEN];
  t_bool log_error;
};

/**
 * @struct	berrorlog_struct
 * @brief	error log maintenance structure
 * @member	u_logfd		file descriptor for log
 * @member	sz_log		large contiguous string block
 * @member	u_loglen	maximum length of string block
 * @member	u_flags		flag indicating status of log
 * @member	flushlog	function to flush log to file or stream
 * @detail	the idea is to simplify, and enable
 *		a user of bkit to use an error log
 *		and flush it. The error management
 *		mechanism can later provide a timed
 *		routine to flush the log to ensure
 *		all reported errors are written to a
 *		log.
 */
struct berrorlog_struct {
  t_s32 i_logfd;
  t_str sz_log;
  t_str sz_log_ptr;
  t_u32 u_loglen;
  t_u8  u8_flags;
  t_u32 pid;
  t_s32 (*flushlog)(t_s32 fd, t_str sz);
};

typedef struct berror_struct    t_berror;
typedef struct szerror_struct   t_szerror;
typedef struct berrorlog_struct t_berrorlog;

/* betakit/berror: function declarations */
#if defined(BERROR_FLUSHLOG_DEFAULTS)
t_s32 _berror_flushlog_default( t_s32 fd, t_str szflush );
t_s32 _berror_flushlog_open( t_str pathfilename );
#endif

t_s32 _berror_strcpy( t_str sz_dst, const t_str sz_src );
t_s32 _berror_strlen( t_str sz );
t_s32 _berror_strrev( t_str str, t_str revstr );
t_s32 _berror_ultostr( t_u32 num, t_str str_orig );


t_s32 berror_log_lock( t_berrorlog *log );
t_s32 berror_log_unlock( t_berrorlog *log);

t_s32 berror_init( t_berror *error );
t_s32 szerror_init( t_szerror *szerror );
t_s32 berror_set( t_berror *error, t_s32 err_major, t_s32 err_minor );
t_s32 szerror_set( t_szerror *szerror, t_berror *error );
t_s32 berror_log_init( t_berrorlog *log, t_s32 logfd, t_u32 loglen, t_u8 flags  );
t_s32 berror_log_flush( t_berrorlog *log );
t_s32 berror_log_free( t_berrorlog *log );
t_s32 berror_log( t_berrorlog *log, t_szerror *szerror, const t_str szprefix );

t_s32 _bk_errlog_init( t_s32 fd );
t_s32 _bk_errlog_post( t_s32 major, t_s32 minor );
t_s32 _bk_errlog_flush( void );
t_s32 _bk_errlog_free( void );

#endif	/* CONFIG_BK_SYS_ERRORHANDLER */

#endif /* _BERROR_H_INC */
