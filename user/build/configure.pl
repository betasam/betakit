#!/usr/bin/perl
#
# configuration parser script
#
# input: *.bconfig
#	  Kernel CML2 like config file
# output: bconfig.
#	  C/C++ header
#
# use:	this is meant for use only through
#	make and not to be called directly
#
# exec: will be run in betakit/user/build 
#	by GNU/Make
# 
use warnings;

use bkparse;

my $default_header_fnpath="../include/bkconfig.h";

my $argcount;

sub print_config_usage()
{    
    bk_print("Error! please specify root config file\n");   
}

sub main()
{
    $argcount = @ARGV;

    bk_setdebug(0);

    if ( $argcount < 1 ) 
    {
    print_config_usage;
    exit 1;
    }

    if( $argcount == 2 )
    {
	$header_fnpath = $ARGV[1];
    }
    else
    {
	$header_fnpath = $default_header_fnpath;
    }

    $filename = $ARGV[0];
    bk_parse($filename, $header_fnpath);

}

main;
