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
# CHANGELOG
#
#	04-JAN-2011	bk_parse takes 4 arguments
#			minor changes
#
use warnings;

use bkparse;

my $default_header_fnpath = "../include/bkconfig.h";
my $default_oldconfig     = "../.bkconfig";

my $argcount;

sub print_config_usage()
{    
    bk_print("Error! please specify root config file\n");   
    bk_print("expected: uname\@host:~/betakit/user/build\$ ./configure ../root.bconfig\n");
    bk_print("exiting!\n");
}

#
# main()
#	argv[1] = root.bconfig
#	argv[2] = include/bkconfig.h
#	argv[3] = .bkconfig
#	argv[4] = (y/n) (force)
#
sub main()
{
    $argcount = @ARGV;

    bk_setdebug(0);

    if ( $argcount < 1 ) 
    {
    print_config_usage;
    exit 1;
    }

    $header_fnpath = $default_header_fnpath;
    $oldconfig_fnpath = $default_oldconfig;
    $force = 'n';

    if( $argcount >= 2 )
    {
	$header_fnpath = $ARGV[1];
    }

    if( $argcount >= 3 )
    {
	$oldconfig_fnpath = $ARGV[2];
    }

    if( $argcount >= 4 )
    {
	$force = $ARGV[3];
    }

    $filename = $ARGV[0];


#    printf("file(%s), hdr(%s), old(%s), force(%s)\n",
#	   $filename, $header_fnpath, $oldconfig_fnpath, $force);

    bk_parse($filename, $header_fnpath, $oldconfig_fnpath, $force);

}

main;
