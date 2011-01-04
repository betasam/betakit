#!/usr/bin/perl 
# pathfactor.pl
#   allows you to replace paths specified as $BETAKITPATH$
#   with the $TOP_DIR path
#
# CHANGELOG
#
#	04-JAN-2011	More configurability through 
#			command line arguments added.
#			still, only to be called by 
#			TOP_DIR/Makefile
#

use strict;

#
# main( inputfile, path, leafdir, varname )
# 
#
sub main
{
    my($filename,$path,$leafdir,$varname)=@_;
    local *DXINFILE;

    if( $filename =~ /^$/ )
    {
	printf("usage: $ ./build/pathfactor.pl <input file> <current dir>\n");
	printf("\t\texiting!\n");
	exit(2);	
    }

    if( $leafdir =~ /^$/ )
    {
	$leafdir = "user";
    }

    if( $varname  =~ /^$/ )
    {
	$varname  = "BETAKITPATH";
    }

    open(DXINFILE, "<" . $filename ) or die("trying to open '$filename' failed.");

    # running from $TOPDIR/user
    $path =~ s/\/$leafdir(.*)$//;

    my $TOPDIR = $path;

    printf("newpath = (%s)\n", $path );
    while( <DXINFILE> )
    {
	my $txt_line = $_;
	chomp( $txt_line );

	$txt_line =~ s/\$$varname\$/$TOPDIR/; 

	printf("%s\n", $txt_line );
    }

    close(DXINFILE);

    exit(0);

}

if( (@ARGV[0] =~ /^$/) || (@ARGV[1] =~ /^$/) )
{
    printf("usage: $ ./build/pathfactor.pl <input file> <current dir> [<leaf_dir> <var_name>]\n");
    printf("\t\texiting!\n");
    exit(3);	
}


main(@ARGV[0],@ARGV[1],@ARGV[2]);
