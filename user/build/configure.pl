#!/usr/bin/perl
#
# configuration parser script
#
# input: *.bconfig
#	  Kernel CML2 like config file
# output: bconfig.
#	  C/C++ header
#
use warnings;

use bkconfig;

$::BKC_DEBUGME = 1;

my $argcount;

sub print_config_usage()
{    
    bk_print("Error! please specify root config file\n");   
}

sub main()
{
    $argcount = @ARGV;

    if ( $argcount < 1 ) 
    {
    print_config_usage;
    exit 1;
    }

    $filename = $ARGV[0];
    bk_parse($filename);

}

main;
