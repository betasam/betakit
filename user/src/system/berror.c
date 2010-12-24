/**
 * @file	berror.c
 * @brief	error handling routines
 * @detail	This file contains error handling functions,
 *		error handling codes, logging functions, and
 *		trace and profiling functions. The objective 
 *		is to minimise code written by the programmer
 *		in response to an error.
 *		Design intends to avoid explicit definition of
 *		strings within code unless necessary. Future
 *		internationalisation of strings is planned.
 *		All strings are therefore t_str here, which
 *		can be changed in future.
 *
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @date	2010-2012
 * @remark	Copyright (c) 2010-2012
 */


/* betakit: types and basic error header */
#include <bconst.h>
#include <btypes.h>
#include <berror.h>

/* posix/linux includes */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* betakit: dynamic memory allocation */
#include <memory.h>

/* betakit: internal error keeping data structures */
static t_berror    _bk_error_last;
static t_szerror   _bk_errorsz_last;
static t_berrorlog _bk_errlog;
static t_u8	   _bk_sz_pid_prefix[ BERR_SZ_LEN ];
static t_bool	   _bk_errorlog_ready = false;

/**
 * @fn _berror_strcpy( t_str sz_dst, const t_str sz_src )
 * @brief string copy function, not too efficient
 * @remark Yes, I know you can write this in one line.
 *	   while( *dst++ = *src++ );
 *	   better still is an optimized memcpy(...) which
 *	   can do either word size or dma style copy.
 * @return 0 on success, -ve on error
 */
t_s32 _berror_strcpy( t_str sz_dst, const t_str sz_src )
{
  t_s32 rt_error = 0;
  t_s32 s32_idx = 0;
  t_str dst;
  t_str src;

  dst = sz_dst;
  src = sz_src;			
  /**
   * @REMARK
   * sz_src is a constant
   * and cannot be manipulated.
   */

  if( (NULL == sz_dst) || (NULL == sz_src) )
    {
      /* error: NULL pointer */
      return( rt_error = -BERR_NOMEM );
    }

  while ( *src )
    {
      *dst++ = *src++;
      s32_idx++;
      if( s32_idx > BERR_SZ_LEN ) /* overflow safe guard */
	{
	  *dst = BK_CHR_NULL;		  /* zero terminator */
	  return( rt_error = -BERR_NOSPACE );
	}
    }
  *dst = BK_CHR_NULL;

  return( rt_error );
}


/**
 * @fn	_berror_strlen( t_str sz )
 * @brief simple strlen implementation
 * @return length of string if succeeded, -ve on failure
 * @warning if string is not null terminated,
 *	    function will overflow (despite the check.)
 */
t_s32 _berror_strlen( t_str sz )
{
  const t_s32 s_maxlen = BERR_SZ_LEN + BERR_SZ_HDRLEN + 1;
  t_s32 s_len = 0;
    
  if( NULL == sz ) return( s_len = -BERR_NOMEM );
  
  while( *(sz+s_len++) && (s_maxlen >= s_len) );

  return( s_len );
}

/**
 * @fn _berror_strrev( t_str revstr, t_str str )
 * @brief reverse str and store in revstr
 * @return length of string if success, or other if failure.
 */
t_s32 _berror_strrev( t_str revstrptr, t_str strptr )
{
  t_s32 s_rt = 0;
  t_s32 len;
  t_u8 *tstr, *trev;
  
  trev = (t_u8*)revstrptr;
  tstr = (t_u8*)strptr;
  if( (NULL == tstr) || (NULL == trev) ) return( s_rt = -BERR_NOMEM );

  len = _berror_strlen( strptr ) - 1;

  while( len > 0 )
    {
      *(trev) = *(tstr + len - 1);
      len--;
      trev++;
    }

  *trev = BK_CHR_NULL;

  return( s_rt = ((t_str)trev - revstrptr) );
}

/**
 * @fn _berror_ultostr( t_u32 num, t_str str_orig )
 * @brief converts (unsigned long) num to string str_orig using ASCII
 * @return 0 if success, non-zero on failure
 */
t_s32 _berror_ultostr( t_u32 u_num, t_str strptr )
{
  t_s32 s_rt = 0;
  t_u8 *str, rev[BERR_SZ_LEN];

  str = (t_u8*)strptr;
  if( NULL == str ) return( s_rt = -BERR_NOMEM );

  while( u_num != 0 )
    {
      *str = (u_num % 10) + '0';
      u_num = u_num / 10;
      str++;
    }
  *str = BK_CHR_NULL;

  str = (t_u8*)strptr;

  _berror_strrev( (t_str)rev, (t_str)str );
  _berror_strcpy( (t_str)str, (t_str)rev );
  return( s_rt = (strptr - (t_str)str) );
}

/**
 * @fn	berror_init( t_berror *error )
 * @brief initialise a berror structure
 * @return 0 if success, -ve if failure
 * @remark this is the same as berror_reset(x)
 *	   @see berror.h for berror_reset
 */
t_s32 berror_init( t_berror *error )
{
  t_s32 rt_error = 0;

  if( NULL == error )
    {
      /* Oops, we cannot initialise error 
       * structure with NULL pointer.
       */
      return( rt_error = -BERR_NOMEM );
    }

  error->err_major = BERR_TYPE0; /* no errors */
  error->err_minor = BERR_ENONE; /* no errors */

  return( rt_error );
}

/**
 * @fn szerror_init( t_szerror *szerror )
 * @brief initialise a string for a berror
 * @return 0 if success, -ve if failure
 */
t_s32 szerror_init( t_szerror *szerror )
{
  t_s32 rt_error = 0;
  
  if( NULL == szerror )
    {
      /* Oops, we cannot describe
       * errors to a NULL pointer.
       */
      return( rt_error = -BERR_NOMEM );
    }
  szerror->log_error = false;	/* last action did not log error */

  return( rt_error );
}

/**
 * @fn berror_set( t_berror *error, t_s32 err_major, t_s32 err_minor )
 * @brief set the error data structure
 * @return 0 if success, -ve if failure
 */
t_s32 berror_set( t_berror *error, t_s32 err_major, t_s32 err_minor )
{
  t_s32 rt_error = 0;
  
  if( NULL == error )
    {
      /* Oops, we cannot describe
       * errors to a NULL pointer.
       */
      return( rt_error = -BERR_NOMEM );
    }

  if( error->err_major > err_major ) 
    {
      /**
       * @note
       * error state already higher 
       *
       * simulated/user-defined errors 
       * are given higher priority.
       * @see berror.h
       */
      rt_error = BERR_LEVEL1;	/* assignment failed */
    }
  else
    {
      error->err_major = err_major;
    }

  if( (error->err_minor & err_minor) == err_minor )
    {
      /**
       * @note
       * error condition already reported 
       * no alteration to error->err_minor
       */
      rt_error = BERR_LEVEL2;
    }
  else
    {
      error->err_minor |= err_minor;
    }

  /* change sign, simple error is always 
   * reported as a -ve return value
   */
  rt_error = (rt_error >= 0) ? rt_error : -rt_error;

  return( rt_error );
}

/**
 * @fn szerror_set( t_szerror *szerror, t_berror *error )
 * @brief set the string of the error, this code is subject to revision
 * @return 0 if success, -ve if failure
 */
t_s32 szerror_set( t_szerror *szerror, t_berror *error )
{
  t_s32 rt_error = 0;
  t_s32 e_priority = 0;
  t_u8 stralloc[BERR_SZ_LEN+1];
  t_str strdesc = NULL;

  if( (NULL == szerror) || (NULL == error) )
    {
      return( rt_error = -BERR_NOMEM ); /* reference: NULL pointer */
    }

  strdesc = (t_str)stralloc;

  switch( error->err_major )
    {
    case BERR_TYPE0:
      _berror_strcpy( strdesc, SZ_BERR_TYPE0 );
      break;
    case BERR_TYPE1:
      _berror_strcpy( strdesc, SZ_BERR_TYPE1 );
      break;
    case BERR_TYPE2:
      _berror_strcpy( strdesc, SZ_BERR_TYPE2 );
      break;
    case BERR_TYPE3:
      _berror_strcpy( strdesc, SZ_BERR_TYPE3 );
      break;
    case BERR_TYPE4:
      _berror_strcpy( strdesc, SZ_BERR_TYPE4 );
      break;
    default:
      _berror_strcpy( strdesc, SZ_BERR_TYPE0 );
    } /* switch( error->err_major ) */
  _berror_strcpy( (t_str)szerror->szerr_head, strdesc );


  /* select minor error based on priority */
  switch( error->err_minor )
    {
    case BERR_ENONE:
      e_priority = error->err_minor;
      break;
    case BERR_EDEADB:
      e_priority = error->err_minor;
      break;
    default:
      {
	e_priority = error->err_minor;
	e_priority = (e_priority & BERR_ENOMEM) ? BERR_ENOMEM : e_priority;
	e_priority = (e_priority & BERR_EINVAL) ? BERR_EINVAL : e_priority;
	e_priority = (e_priority & BERR_ECRASH) ? BERR_ECRASH : e_priority;
	e_priority = (e_priority & BERR_DLOCKD) ? BERR_DLOCKD : e_priority;
	e_priority = (e_priority & BERR_ENULLP) ? BERR_ENULLP : e_priority;
	e_priority = (e_priority & BERR_EPARSE) ? BERR_EPARSE : e_priority;
	e_priority = (e_priority & BERR_ESSEGV) ? BERR_ESSEGV : e_priority;
	e_priority = (e_priority & BERR_RIGHTS) ? BERR_RIGHTS : e_priority;
	e_priority = (e_priority & BERR_ANOMAL) ? BERR_ANOMAL : e_priority;
	e_priority = (e_priority & BERR_ELOGIT) ? BERR_ELOGIT : e_priority;
      }
    }
  
  error->err_minor &= ~(e_priority); /* clear last posted error */

  /* set minor error string description */
  switch( e_priority )
    {
    case BERR_ENONE:
      _berror_strcpy( strdesc, SZ_BERR_ENONE );
      break;
    case BERR_EDEADB:
      _berror_strcpy( strdesc, SZ_BERR_EDEADB);
      break;
    case BERR_ENOMEM:
      _berror_strcpy( strdesc, SZ_BERR_ENOMEM);
      break;
    case BERR_EINVAL:
      _berror_strcpy( strdesc, SZ_BERR_EINVAL);
      break;
    case BERR_ECRASH:
      _berror_strcpy( strdesc, SZ_BERR_ECRASH);
      break;
    case BERR_DLOCKD:
      _berror_strcpy( strdesc, SZ_BERR_DLOCKD);
      break;
    case BERR_ENULLP:
      _berror_strcpy( strdesc, SZ_BERR_ENULLP);
      break;
    case BERR_EPARSE:
      _berror_strcpy( strdesc, SZ_BERR_EPARSE);
      break;
    case BERR_ESSEGV:
      _berror_strcpy( strdesc, SZ_BERR_ESSEGV);
      break;
    case BERR_RIGHTS:
      _berror_strcpy( strdesc, SZ_BERR_RIGHTS);
      break;
    case BERR_ANOMAL:
      _berror_strcpy( strdesc, SZ_BERR_ANOMAL);
      break;
    case BERR_ELOGIT:
      _berror_strcpy( strdesc, SZ_BERR_ELOGIT);
      break;
    default:
      _berror_strcpy( strdesc, SZ_BERR_DANGER);      
    } /* switch( e_priority ) */

  _berror_strcpy( (t_str)szerror->szerr_desc, (const t_str)strdesc);

  return( rt_error );
}


/**
 * @fn berror_log_lock( t_berror *log )
 * @brief lock an error log for processing
 * @return 0 on success, -ve on failure
 * @warning this call may lock, unless it is rewritten
 */
t_s32 berror_log_lock( t_berrorlog *log)
{
  t_s32 retval = 0;

  if( NULL == log) return( retval = -BERR_NOMEM);

  while( BERR_LOGLOCK == (log->u8_flags & BERR_LOGLOCK) ); 
  /**
   * @TODO
   *
   * we must sleep()
   * or sched_yield()
   */

  log->u8_flags |= BERR_LOGLOCK;

  return( retval );
}

/**
 * @fn berror_log_unlock( t_berrorlog *log )
 * @brief unlock the error log
 * @return 0 on success, -ve on error
 * @remark caller must be privileged lock holder
 */
t_s32 berror_log_unlock( t_berrorlog *log)
{
  t_s32 retval = 0;

  if( NULL == log) return( retval = -BERR_NOMEM);

  log->u8_flags &= ~(BERR_LOGLOCK);

  return( retval );
}

/**
 * @fn berror_log_init( t_berrorlog *log )
 * @brief initialise an error log
 * @return 0 on success, -ve on failure
 */
t_s32 berror_log_init( t_berrorlog *log, t_s32 logfd, t_u32 loglen, t_u8 flags  )
{
  t_s32 rt_error = 0;
  t_str szptr  = NULL;
#if defined(BERROR_FLUSHLOG_DEFAULTS)
  t_s32 rt_idx = 0;
  t_u8  u8_fpathname[BERR_SZ_LEN];
  t_str str_pathname;
  pid_t mypid = 0;
#endif

  str_pathname = (t_str)u8_fpathname;

  if( NULL == log )
    {
      return( rt_error = -BERR_NOMEM ); /* reference: NULL pointer */
    }

  if( (0 == loglen) || (NULL != log->sz_log) || (NULL != log->sz_log_ptr) )
    {
      return( rt_error = -BERR_INVALID ); /* check: invalid parameters */
    }

  szptr = (t_str)(mem_alloc(loglen + 1)); /* watch: memory allocation */
  if( NULL == szptr )
    {
      return( rt_error = -BERR_NOSPACE ); /* memory: unable to allocate */
    }
  
  log->u_loglen = loglen;
  log->sz_log = szptr;
  log->sz_log_ptr = log->sz_log;

  log->i_logfd  = logfd;
  log->u8_flags = flags;

#if defined(BERROR_FLUSHLOG_DEFAULTS)
  mypid = getpid();
  log->pid = (t_u32)mypid;

  _berror_strcpy( str_pathname, SZ_BERR_LOGPATH );
  rt_idx = _berror_strlen( str_pathname );
  _berror_strcpy( (t_str)(str_pathname + rt_idx - 1), SZ_BERR_LOGFILE );

  if( 0 > logfd )
    {
      logfd = _berror_flushlog_open( str_pathname );
      if( 0 > logfd )
	{
	  logfd = BK_STDERR;
	}
      log->i_logfd = logfd;
    }
  log->flushlog = _berror_flushlog_default; /* warning: function pointer */  
#endif  
  
  berror_log_unlock( log );
  
  return( rt_error );
}

/**
 * @fn berror_log_flush( t_berrorlog *log )
 * @brief flush log contents
 * @return 0 on success, -ve on failure
 */
t_s32 berror_log_flush( t_berrorlog *log )
{
  t_s32 rt_error = 0;
  if( NULL == log )
    {
      return( rt_error = -BERR_NOMEM ); /* reference: NULL pointer */
    }

  if( (NULL == log->flushlog) )
    {
      return( rt_error = -BERR_INVALID); /* invalid: parameters */
    }

  if( 0 > log->i_logfd )
    {
      log->i_logfd = BK_STDERR;	/* default: stderr */
    }
  
  berror_log_lock( log );
  do {
    rt_error = log->flushlog( log->i_logfd, log->sz_log );
    if( 0 <= rt_error )
      {
	log->sz_log_ptr = log->sz_log;
	log->u8_flags  &= ~(BERR_LOGFULL);
      }
  } while(0);
  berror_log_unlock( log );

  return( rt_error );
}

/**
 * @fn berror_log_free( t_berrorlog *log )
 * @brief free an existing valid log
 * @return 0 on success, -ve on error
 */
t_s32 berror_log_free( t_berrorlog *log )
{
  t_s32 rt_error = 0;
  
  if( (NULL == log) ) return( rt_error = -BERR_NOMEM );
  if(!IS_LOG_VALID(log)) return( rt_error = -BERR_INVALID );

  berror_log_lock( log );
  do {
    mem_free( log->sz_log );
    log->sz_log_ptr = NULL;
    log->sz_log = NULL;
    log->u_loglen = 0;
    log->flushlog = NULL;		/* WARNING! NULL function pointer */
  
  } while(0);
  berror_log_unlock( log );

  log->u8_flags = BERR_LOGUNDEF; /* WARNING! Log invalidated */

  return( rt_error );
}

/**
 * @fn berror_log( t_berror *error, t_szerror *szerror, const t_byte *szprefix )
 * @brief log the error in error, use szerror if necessary
 * @return 0 if success, -ve if failure
 */
t_s32 berror_log( t_berrorlog *log, t_szerror *szerror, const t_str szprefix )
{
  t_s32 rt_error = 0;
  t_s32 szlen  = 0;
  t_str prefix;
  t_str szcurr  = NULL;

  if( (NULL == log) || (NULL == szerror ) )
    {
      return( rt_error = -BERR_NOMEM ); /* reference: NULL pointer */
    }

  if(!IS_LOG_VALID(log)) 
    {
      return( rt_error = -BERR_INVALID ); /* invalid: log parameter or content */
    }

  if( BERR_LOGFULL == (log->u8_flags & BERR_LOGFULL) )
    {
      return( rt_error = -BERR_NOSPACE ); /* memory: insufficient for log */
    }

  prefix = szprefix;
  if( (NULL == prefix) || (BK_CHR_NULL == *prefix) )
    {
      prefix = (t_str)mem_alloc( BERR_SZ_HDRLEN );
      _berror_strcpy( prefix, SZ_BERR_DEFPREFIX );
    }

  if( log->pid > 0)
    {
      _berror_ultostr(log->pid, (t_str)_bk_sz_pid_prefix);
    }
 
  
  berror_log_lock( log );
  /**
   * @TODO
   * Debug the following code section
   * thoroughly, you must.
   * Dangerous to t(h)read, it is.
   * There is no try.
   */
  do {
    szlen  = 0;
    szcurr = (t_str)(log->sz_log_ptr);

    if(log->pid > 0) 
      {
	_berror_strcpy( szcurr, (t_str)_bk_sz_pid_prefix);
	szlen = _berror_strlen( (t_str) (_bk_sz_pid_prefix));
	szcurr += (szlen-1);
	_berror_strcpy( szcurr, SZ_BERR_DELIM);
	szcurr++;
      }

    _berror_strcpy( szcurr, (const t_str)prefix );

    szlen = _berror_strlen( (t_str)prefix );
    szcurr += (szlen-1);
    _berror_strcpy( (t_str)(szcurr), (const t_str)szerror->szerr_head );

    szlen = _berror_strlen( (t_str)szerror->szerr_head );
    szcurr += (szlen-1);

    _berror_strcpy( (t_str)(szcurr), SZ_BERR_DELIM );
    szcurr++;

    _berror_strcpy( (t_str)(szcurr), (const t_str)szerror->szerr_desc );

    szlen = _berror_strlen( (t_str)szerror->szerr_desc );
    szcurr += (szlen-1);
    _berror_strcpy( (t_str)(szcurr), (const t_str)BK_STR_EOL );

    szlen = _berror_strlen( BK_STR_EOL );
    szcurr += (szlen-1);
    log->sz_log_ptr = szcurr;

    if( log->u_loglen <= (log->sz_log_ptr - log->sz_log) )
      {
	log->u8_flags |= BERR_LOGFULL;
      }

  } while(0);
  berror_log_unlock( log );

  if( NULL == szprefix )
    {
      mem_free(prefix);		/* avoid dangling pointers */
      prefix = NULL;
    }

  return( rt_error );
}

#if defined(BERROR_FLUSHLOG_DEFAULTS)
/**
 * @fn t_s32 berror_flushlog_open( t_str pathfilename )
 * @brief open a file for logging if not stderr
 * @return -ve on failure, 0 or >0 on success
 */
t_s32 _berror_flushlog_open( t_str pathfilename )
{
  t_s32 rt_error;
  t_s32 tfd;
  struct stat statbuf;

  if( 0 > stat( pathfilename, &statbuf ) )
    {
      tfd = open( pathfilename, O_CREAT, S_IRWXU );
      if( tfd >= 0 )
	{
	  close( tfd );
	}
    }

  tfd = open( pathfilename, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU );
  rt_error = tfd;

  return( rt_error );
}

/**
 * @fn t_s32 _berror_flushlog_default( t_s32 fd, t_str szflush )
 * @brief flush the log to an open file descriptor
 * @return -ve on error, 0 or >0 on success
 */
t_s32 _berror_flushlog_default( t_s32 fd, t_str szflush )
{
  t_s32 rt_error = 0;
  t_s32 buflen   = 0;
  buflen = _berror_strlen( szflush );
  if( buflen > 0 )
    {
      rt_error = write( fd, szflush, buflen );
    }
  else
    {
      rt_error = buflen;
    }
  
  return( rt_error );
}
#endif	/* BERROR_FLUSHLOG_DEFAULTS */


/**
 * @fn _bk_errlog_init( t_s32 fd )
 * @brief initialise internal error log for bkit
 * @return status of initialisation: >=0 success, -ve failure
 */
t_s32 _bk_errlog_init( t_s32 fd )
{
  t_s32 rt = 0;
  if( true == _bk_errorlog_ready )
    {
      return( rt = -BERR_ANOMAL );
    }
  rt  = berror_init( &_bk_error_last );
  rt += szerror_init( &_bk_errorsz_last );
  rt += berror_log_init( &_bk_errlog, fd, BERR_LOG_SIZE, (t_u8)0 );
  _bk_errorlog_ready = true;
  return( rt );
}

/**
 * @fn _bk_errlog_post( t_s32 major, t_s32 minor )
 * @brief post an error to internal error data structure and log
 * @return status: rt>=0 on success, rt<0 on failure
 */
t_s32 _bk_errlog_post( t_s32 major, t_s32 minor )
{
  t_s32 rt = 0;
  if( false == _bk_errorlog_ready )
    {
      return( rt = -BERR_LOGUNDEF );
    }
  rt += berror_set( &_bk_error_last, major, minor );
  rt += szerror_set( &_bk_errorsz_last, &_bk_error_last );
  rt += berror_log( &_bk_errlog, &_bk_errorsz_last, SZ_BERR_DEFPREFIX );
  return( rt );
}

/**
 * @fn _bk_errlog_flush( void )
 * @brief flush internal error log
 * @return status: rt>=0 on success, rt<0 on failure
 */
t_s32 _bk_errlog_flush( void )
{
  t_s32 rt = 0;
  if( false == _bk_errorlog_ready )
    {
      return( rt = -BERR_LOGUNDEF );
    }
  rt += berror_log_flush( &_bk_errlog );
  return( rt );
}

/**
 * @fn _bk_errlog_free( void )
 * @brief free internal error log and datastructures 
 * @return status: rt>=0 on success, rt<0 on failure
 */
t_s32 _bk_errlog_free( void )
{
  t_s32 rt = 0;
  if( false == _bk_errorlog_ready )
    {
      return( rt = -BERR_LOGUNDEF );
    }
  rt = berror_log_free( &_bk_errlog );
  rt = berror_reset( &_bk_error_last );
  _bk_errorlog_ready = false;
  return( rt );
}


/** @remark end of file "berror.c" */
