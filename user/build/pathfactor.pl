#!/usr/bin/perl 
# pathfactor.pl
#   allows you to replace paths specified as $BETAKITPATH$
#   with the $TOP_DIR path
#

use strict;

#
# main( inputfile, path )
# 
#
sub main
{
    my($filename,$path)=@_;
    local *DXINFILE;

    if( $filename =~ /^$/ )
    {
	printf("usage: $ ./build/pathfactor.pl <input file> <current dir>\n");
	printf("\t\texiting!\n");
	exit(2);	
    }

    open(DXINFILE, "<" . $filename ) or die("trying to open '$filename' failed.");

    # running from $TOPDIR/user
    $path =~ s/\/user(.*)$//;

    my $TOPDIR = $path;

    printf("newpath = (%s)\n", $path );
    while( <DXINFILE> )
    {
	my $txt_line = $_;
	chomp( $txt_line );

	$txt_line =~ s/\$BETAKITPATH\$/$TOPDIR/; 

	printf("%s\n", $txt_line );
    }

    close(DXINFILE);

    exit(0);

}

if( (@ARGV[0] =~ /^$/) || (@ARGV[1] =~ /^$/) )
{
    printf("usage: $ ./build/pathfactor.pl <input file> <current dir>\n");
    printf("\t\texiting!\n");
    exit(3);	
}


main(@ARGV[0],@ARGV[1]);
