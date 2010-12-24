/**
 * @file	cli_parser.c
 * @brief	CLI parser helper routines for bkit
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @date	2010-2012
 */

#include <bconst.h>
#include <btypes.h>
#include <berror.h>

#include <memory.h>

#include <cli_parser.h>

/**
 * @fn t_str *cli_tokens( t_str sz_line )
 * @brief tokenise cli input for parsing
 * @return array to tokens on success, NULL on failure
 */
t_str *cli_tokens( t_str sz_line )
{
  t_str *str_array = NULL;
  
  if( NULL == sz_line )
    {
      _bk_errlog_post( BERR_TYPE1, BERR_ENULLP );
      return( str_array );
    }

  return( str_array );
}

/* @remark end of file "cli_parser.c" */
