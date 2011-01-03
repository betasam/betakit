#!/usr/bin/perl
# @brief configuration parser module
#
# @requires Exporter, Parse::RecDescent
# @provides bkconfig
#
# @author Sunil Beta <betasam@gmail.com>
#

package bkconfig;
use Parse::RecDescent;
require Exporter;

@ISA=qw(Exporter);
@EXPORT = qw(bk_print bk_printf bk_parse BKC_DEBUGME);

$::RD_ERRORS = 1; # Make sure the parser dies when it encounters an error
$::RD_WARN   = 1; # Enable warnings. This will warn on unused rules &c.
$::RD_HINT   = 1; # Give out hints to help fix problems.

$bkprogramname = "BetaKit";
$bkprogramvers = "1.0.2";
$bkprogramprom = $bkprogramname . " " . $bkprogramvers . " > ";

# set DEBUGME in the file deploying the module as indicate above
my $BKC_DEBUGME;

my @CONFIG_VARIABLE;
my @CONFIG_VAR_TYPE;
my @CONFIG_VAR_VALS;
my @CONFIG_VAR_DEPS;
my @CONFIG_VAR_INFO;
my $bk_rootmenu_str;

my $last_var;
my $last_idx;

my $DEBUGME = $BKC_DEBUGME;

##
# 
# variable dependency GRAMMAR
#
##
$bkdepgrammar = << '_EODEPS_';

    variant: /[y|m|n]/i
   operator: /and/i | /or/i | /\&\&/ | /\|\|/
 dependency: variant operator variant operator variant
{ return(bkconfig::bk_ternary_deps(@item)) }
           | variant operator variant
{ return(bkconfig::bk_binary_deps(@item)) } 
	   | variant operator variant operator variant operator variant
{ return(bkconfig::bk_quaternary_deps(@item)) }
      unary: variant
{ return(bkconfig::bk_unary_deps(@item)) }
  startrule: dependency | unary

_EODEPS_
# end of variable dependency grammar


##
#
# bkconfig full GRAMMAR
# specification.
#
$bkconfiggrammer = << '_EOSPEC_';

    integer: /(0-9)+/

     string: /(\".+\")|(\'.+\')/

    boolean: /[y|n]/i

  tristatus: /[y|m|n]/i

   variable: /[A-Za-z_][A-Za-z0-9_]*/

         op: /\&\&/ | /\|\|/ | /and/i | /or/i

 assignment: /default/i

    actions: /select/i
    
  specifier: /mainmenu/i

  directive: /config/i | /depends on/i 
    
       type: /bool/i | /tristate/i

     values: boolean | tristatus | integer | string

 expression: variable 
           | variable op expression

  condition: /if/i variable

  closecond: /endif/i | /else/i

      title: specifier string
            { return bkconfig::bk_parse_title(@item) }

 imperative: directive expression
            { return bkconfig::bk_parse_imperative(@item) }

declaration: type string
	    { return bkconfig::bk_parse_declaration(@item) }

     assign: assignment values
	    { return bkconfig::bk_parse_assignment(@item) }

    include: /include/i string
            { return bkconfig::bk_parse_include(@item) }

      block: condition | closecond

  startrule: title | imperative | declaration | assign | include


_EOSPEC_
#
# grammar ends here.
#___________________

#
# bk_print, bk_printf, dprintf routines
# 
sub dprintf
{
    printf(@_) if ($DEBUGME =~ 1);
}

sub bk_print($)
{
    my($bk_arg)=$_[0];
    printf( "%s %s", $bkprogramprom, $bk_arg );    
}

sub bk_printf
{
    printf("%s ", $bkprogramprom );
    printf(@_);
}
# end of print routines


##
#
# bkconfig variable handlers
#
##

sub bk_add_variable($)
{
    my($new_var) = @_[0];
    my $var_idx;
    
    for( $var_idx = 0; $var_idx < $last_idx; $var_idx++ )
    {
	if( $CONFIG_VARIABLE[$var_idx] =~ $new_var ) {
	    return;
	}
    }
    dprintf("bk: adding variable %s\n", $this_variable);

    $last_idx++;
    $CONFIG_VARIABLE[$last_idx-1] = $new_var;
    return;
}

sub bk_add_variable_dep($)
{
    my($new_dep) = $_[0];
    $add_dep = "";
    $cur_dep = $CONFIG_VAR_DEPS[$last_idx-1];
    if( $cur_dep !~ /^$/ )
    {
	if( ($new_dep =~ /and /i) || ($new_dep =~ /or /i) )
	{
	    $add_dep = $cur_dep . " " . $new_dep;
	}
	else
	{
	$add_dep = $cur_dep . " AND " . $new_dep;
	}
    }
    else
    {
	$add_dep = $new_dep;
    }

    $CONFIG_VAR_DEPS[$last_idx-1] = $add_dep;
    
}

sub bk_set_variable_type($)
{
    my($new_var_type) = @_[0];

    $CONFIG_VAR_TYPE[$last_idx-1] = $new_var_type;
    dprintf("bk: variable type %s\n", $new_var_type);
}

sub bk_set_variable_info($)
{
    my($new_var_info) = @_[0];
    $new_var_info =~ s/\"//g;
    $CONFIG_VAR_INFO[$last_idx-1] = $new_var_info;
    dprintf("bk: variable info %s\n", $new_var_info );
}

sub bk_set_variable_val($)
{
    my($new_var_val) = $_[0];
    $CONFIG_VAR_VALS[$last_idx-1] = $new_var_val;
}

# variable handlers end here
#____________________________


##
#
# PARSE ROUTINES
# not exported.
#
##

#
# parse title
# example: MAINMENU <TITLE_STRING>
#          where TITLE_STRING is <".+">
#
sub bk_parse_title
{
# specifier string
    $bk_rootmenu_str = $_[2];
    $bk_rootmenu_str =~ s/\"//g;

  dprintf("bk: title = %s\n", $bk_rootmenu_str);
}


#
# parse imperative
# example: CONFIG <VARIABLE_NAME>
#     (or) DEPENDS ON <VARIABLE_NAME>
#
# where VARIABLE_NAME is an IDENTIFIER (A-Z)[(A-Z)(0-9)]*
#
sub bk_parse_imperative
{
  $this_directive = $_[1];
  $this_variable  = $_[2];

  dprintf("bk: [impr] got %s %s %s\n", $_[1], $_[2]);

  if( $this_directive =~ /config/i )
  {
      bk_add_variable( $this_variable );
  }

  if( $this_directive =~ /depends on/i )
  {
      bk_add_variable_dep( $this_variable );

      $conjunction = $_;
      $conjunction =~ s/.+depends on $this_variable//ig;
      chomp($conjunction);
      if( ($conjunction =~ /and/i) || ($conjunction =~ /or/i) || ($conjunction =~ /\&\&/) || ($conjunction =~ /\|\|/) )
      {
	  $conjunction =~ s/^ //g;
	  $conjunction =~ s/\|\|/OR/g;
	  $conjunction =~ s/\&\&/AND/g;
	  bk_add_variable_dep( $conjunction );
      }
  }

}

#
# parse declaration
# example: <type> "description"
#	   type is (bool|tristate|integer|string)
#
sub bk_parse_declaration
{
    my $var_type_cur = $_[1];
    my $var_type_info = $_[2];

    bk_set_variable_type( $var_type_cur );
    bk_set_variable_info( $var_type_info );

    dprintf("bk: [decl] got %s %s\n", $_[1], $_[2]);
}

#
# parse assignment
# example: default y
#          default 386
#
sub bk_parse_assignment
{
    my $assign_type = $_[1];
    my $assign_val  = $_[2];

    if( $assign_type =~ "default" )
    {
	$assign_val =~ s/\"//g;
	bk_set_variable_val( $assign_val );
    }

  dprintf("bk: [assn] got %s %s\n", $_[1], $_[2]);

}

#
# parse any submenu
# assume: $parser is initialised
# (any level of nesting theoretically)
#
sub bk_parse_subconfig($)
{
    my($bk_file)=$_[0];

    open(BKSUBCONFIG,"<".$bk_file) or return;
    
    while(<BKSUBCONFIG>)
    {
	$line = $_;
	chomp($line);

	if( ($line !~ m/^#/) && ($line !~ m/^$/) )
	{
	    $line =~ s/(\t)+/ /g;
	    $line =~ s/^ +//gi;
	    $parser->startrule($line);
	}
    }

    close(BKSUBCONFIG);
}

#
# parse included file 
# (any level of nesting theoretically)
# example: INCLUDE "path/filename"
#
sub bk_parse_include
{
# /include/i string
    my $submenu_file = $_[2];

    $submenu_file =~ s/\"//g;

    bk_parse_subconfig( $submenu_file );
    dprintf("bk: [incl] got %s %s\n", $_[1], $submenu_file);
}


#
# evaluate boolean expression
# of the form w OP x OP y 
#
sub bk_ternary_deps
{
    shift;
    my($l,$o,$m,$n,$q) = @_;
    
    $expr = $l . " " . $o . " " . $m . " " . $n . " " . $q;
    $expr =~ s/and/\*/ig;
    $expr =~ s/or/\+/ig;
    $expr =~ s/y/1/ig;
    $expr =~ s/m/1/ig;
    $expr =~ s/n/0/ig;

    $result = eval $expr;

    dprintf(">> [expr] %s = [result] %s\n", $expr, $result );
    return $result;
} 

# TODO: Rewrite grammar, maybe LALR
#
# evaluate long boolean expression 
# of form w OP x OP y OP z
#      or w l  x m  y n  z
#
sub bk_quaternary_deps
{
    shift;
    my($w,$l,$x,$m,$y,$n,$z) = @_;

    $expr = $w . " " . $l . " " . $x . " " . $m . " " . $y . " " . $n . " " . $z;

    $expr =~ s/and/\*/ig;
    $expr =~ s/or/\+/ig;
    $expr =~ s/y/1/ig;
    $expr =~ s/m/1/ig;
    $expr =~ s/n/0/ig;

    $result = eval $expr;

    dprintf(">>> [expr] %s = [result] %s\n", $expr, $result );
    
    return $result;
}

#
# evaluate boolean expression
# of the form x OR y
#
sub bk_binary_deps
{
    shift;
    my($lhs,$op,$rhs) = @_;

    $expr = $lhs . " " . $op . " " . $rhs;

    $expr =~ s/and/\*/ig;
    $expr =~ s/or/\+/ig;

    $expr =~ s/y/1/ig;
    $expr =~ s/m/1/ig;
    $expr =~ s/n/0/ig;


   dprintf(">> eval %s\n", $expr);
   return eval "$expr";
}

#
# evaluate boolean 
# of the form x, such that x <<= {y,n}
# 
sub bk_unary_deps
{
    shift;
    my ($lhs) = $_[0];
    
    $lhs =~ s/y/1/ig;
    $lhs =~ s/m/1/ig;
    $lhs =~ s/n/0/ig;

    dprintf(">> u_eval %s\n", $lhs);
    return eval "$lhs";
}

#
# end of parse routines
#________________________.

##
#
# Parser execution helpers
#

# 
# bk_get_value( $variable_name )
#     return the value of a variable if known or
#     'UNDEF' if cannot be computed.
#
sub bk_get_value($)
{
    my $varname=$_[0];
    my $idx;
    my $val="UNDEF";

    for( $idx = 0; $idx < $last_idx; $idx++ )
    {
	if( $CONFIG_VARIABLE[$idx] =~ $varname )
	{
	    $val = $CONFIG_VAR_VALS[$idx];
	    return($val);
	}
    }

    return($val);
}

#
# bk_resolve_deps
#     resolve variable dependencies and modify
#     current values (which are originally defaults.)
#
sub bk_resolve_deps
{
    my $idx;
    my $jdx;

    my $result;

    for( $idx = 0; $idx < $last_idx; $idx++ )
    {
	if( $CONFIG_VAR_DEPS[$idx] !~ /^$/ )
	{
	    $curdep = $CONFIG_VAR_DEPS[$idx];
	    dprintf("> %d> Resolving %s\n", $idx, $curdep);

	    for( $jdx = 0; $jdx < $last_idx; $jdx++ )
	    {
		$curvar = $CONFIG_VARIABLE[$jdx];
		if( $curdep =~ $curvar )
		{
		    $curval = $CONFIG_VAR_VALS[$jdx];
		    $curdep =~ s/$curvar /$curval /g;
		    $curdep =~ s/$curvar$/$curval/g;
		    $CONFIG_VAR_DEPS[$idx] = $curdep;
		}
	    }
	}
	
    }
}

#
# resolve using bk_compute_deps
#
sub bk_compute_deps
{
    my $idx;
    my $result;
    my $bool_expr;

    my $dep_parser = Parse::RecDescent->new($bkdepgrammar);
    
     
    for( $idx = 0; $idx < $last_idx; $idx++ )
    {
	$bool_expr = $CONFIG_VAR_DEPS[$idx];
	if( $bool_expr !~ /^$/ )
	{
	    $result = $dep_parser->startrule($bool_expr);
	    if( $result >= 0 )
	    {
		$CONFIG_VAR_DEPS[$idx] = $result;
	    }
	    dprintf("\t[dep] (%s) = (%s)\n", $bool_expr, $result );
	}
    }
    
}

#
# bk_get_vars
#
sub bk_get_vars
{
    my $idx;
    my $bool_expr;
    my $result;

    printf("Config Menu: < %s >\n\n", $bk_rootmenu_str );

    for( $idx = 0; $idx < $last_idx; $idx++ )
    {
	$bool_expr = $CONFIG_VAR_DEPS[$idx];
	if( $bool_expr !~ /^$/ )
	{
	    $result = $CONFIG_VAR_DEPS[$idx];
	    if(($result =~ /^$/) || ($result !~ 0))
	    {
		printf("(%s) %s [y/m/n] > ", $CONFIG_VARIABLE[$idx], $CONFIG_VAR_INFO[$idx]);

		$user_choice = <STDIN>;
		printf("\n");
		
		$CONFIG_VAR_VALS = $user_choice if ($CONFIG_VAR_VALS =~ /[y|n|m]/i);
		# ghastly
		$user_choice = /^$/;
		
	    }
	}
    }
    bk_compute_deps;
}

#
# dump variables for debug
#
sub bk_dump_vars
{
    my $idx;

    for( $idx = 0; $idx < $last_idx; $idx++ )
    {
	printf("%d: VAR[%s] : type[%s] = (%s) < %s >\n", $idx,
	       $CONFIG_VARIABLE[$idx], $CONFIG_VAR_TYPE[$idx], $CONFIG_VAR_VALS[$idx], 
	       $CONFIG_VAR_DEPS[$idx] );
	printf("\t <%s>\n", $CONFIG_VAR_INFO[$idx] );
    }
}

#
# generate config.h file
# for use in C/C++ sources
#
sub bk_gen_header($)
{
    my $headerfile=$_[0];

    open(OUTPUTHEADER,">".$headerfile);

    printf(OUTPUTHEADER "\n/*\n");
    printf(OUTPUTHEADER " * Betakit Autogenerated bkconfig.h \n");
    printf(OUTPUTHEADER " */\n\n");
    
    for( $idx = 0; $idx < $last_idx; $idx++ )
    {
	if( $CONFIG_VAR_VALS[$idx] !~ /n/i )
	{
	    printf(OUTPUTHEADER "#define CONFIG_%s\t\t%s\n", $CONFIG_VARIABLE[$idx], $CONFIG_VAR_VALS[$idx] ) 
		 if ($CONFIG_VAR_DEPS[$idx] !~ 0);
	}
	else
	{
	    printf(OUTPUTHEADER "#undef  CONFIG_%s\n", $CONFIG_VARIABLE[$idx] );
	}
    }

    printf(OUTPUTHEADER "\n/*\n");
    printf(OUTPUTHEADER " * end of file\n");
    printf(OUTPUTHEADER " */\n\n");
    
    close(OUTPUTHEADER);
    
    return;
}

#
# parse configuration file
#
sub bk_parse($)
{
    my($bk_file)=$_[0];
    
    my @ROOTMENU;
    my $lastline;

    open(BKCONFIGFILE,"<".$bk_file) or die("cannot open file: $bk_file\n");

    $parser = Parse::RecDescent->new($bkconfiggrammer);
    
    $last_var = "";
    $last_idx = 0;
    
    while(<BKCONFIGFILE>)
    {
	$line = $_;
	chomp($line);

	# ignore comments, discard blank lines, squeeze
	if( ($line !~ m/^#/) && ($line !~ m/^$/) ) 
	{
	    $line =~ s/(\t)+/ /g;
	    $line =~ s/^ +//gi;
	    $parser->startrule($line);
	}
    }

    close(BKCONFIGFILE);

    bk_resolve_deps;
    bk_compute_deps;

##   DEBUG
#    bk_dump_vars;		
    bk_get_vars;

    bk_gen_header("../include/bkconfig.h");
}

1;

# end of bkconfig.pm code
# <EOF>
