#!/usr/bin/perl
# @brief configuration parser module
#
# @requires Exporter, Parse::RecDescent
# @provides bkparse
#
# @author Sunil Beta <betasam@gmail.com>
#


#
# CHANGELOG
#
# 04 Jan 2011		Support to read/write .bkconfig
#			.config like files
#			DEFINE VARNAME, DEFAULT VALUE
#			support added to .bconfig files
#			for values, users cannot modify
#
# 03 Jan 2011		Revised header file generator, 
#			minor update to "discard" rule
#
# 30 Dec 2010		Rewrote this version with heavy hash look-ups
#			to replace bkconfig.pm
#


package bkparse;
require Exporter;

use Parse::RecDescent;

@ISA=qw(Exporter);
@EXPORT = qw(bk_print bk_printf bk_parse bk_setdebug);

$::RD_ERRORS = 1; # Make sure the parser dies when it encounters an error
$::RD_WARN   = 1; # Enable warnings. This will warn on unused rules &c.
$::RD_HINT   = 1; # Give out hints to help fix problems.

#
# TODO
# Update strings using $version$
#

#
# VERSION Strings
#
$bkprogramname = "BetaKit";
$bkprogramvers = "1.0.3";
$bkprogrampref = $bkprogramname . " " . $bkprogramvers . " > ";
#
# end VERSION Strings
#

#
# special variables
#
$BKC_DEBUGME   = 0;
$BKC_LASTVARIDX= 0;

my @BKC_FILE;

my @CONFIG_VAR_NAME;		# array holding list of variable names
my %CONFIG_VAR_DEPS;		# hash holding variable dependency
my %CONFIG_VAR_DESC;
my %CONFIG_VAR_TYPE;		# hash holding variable type
my %CONFIG_VAR_VALS;		# hash holding variable values
my $CONFIG_VAR_INIT;		# hash holding variable default values
my %CONFIG_IDX_HASH;		# hash storing index of variable
my %CONFIG_VAR_USER;		# can user define variable value
my %CONFIG_VAR_STAT;		# hash to compute 
				#  if dependency allows variable
my $CONFIG_VAR_LOCK = false;	# lock for config variable structure
my $CONFIG_VAR_USED = 0;	# used is also 0 before lock
				# except while parsing sub-files

my $CONFIG_MNU_ROOT;		# name of Root menu
my @CONFIG_MNU_NODE;		# array of menu nodes
my $CONFIG_MNU_CNTR = 0;	# counter for defined menu structures

#
# end variable export.
#

#
# internal global variables
#
my $begin_block;
my $finis_block;

my $last_boolean;

my $this_rule;
my $this_parser;
#
# end of global variables
#


#
# GRAMMAR rules
#

#
# grammar for root.bconfig and *.bconfig files
#
$bkparsegrammar= << '_EODEF_';

      integer: /(0-9)+/
       string: /(\".+\")|(\'.+\')/
     variable: /[a-zA-Z_][a-zA-Z0-9_]*/
      boolean: /[yn]/i
     tristate: /[ymn]/i
     constant: boolean | tristate | integer | string
   binoperand: '&&' | '||' | /and/i | /or/i 
 constoperand: /=/ | /!=/ | /=~/ | /!~/
      vartype: /bool/i | /string/i | /integer/i | /tristate/i
    directive: /source/i | /include/i
      keyword: /config/i
       select: /select/i | /define/i | /set/i
      depends: /depends on/i
       setval: /def_bool/i | /default/i | /def_string/i
     rootmenu: /mainmenu/i 
    multiline: /--help--/i
      include: directive string
{ return bkparse::bk_parse_source(@item) }
      menudef: rootmenu string
{ return bkparse::bk_setmenu_name(@item) }
    assertion: keyword variable
  nonuserconf: select variable
   assignment: setval constant
{ return bkparse::bk_setvariable(@item) }
      typedef: vartype string
{ return bkparse::bk_typevariable(@item) }
             | vartype
{ return bkparse::bk_typevariable(@item) }       
         expr: variable binoperand variable binoperand variable binoperand variable binoperand variable
{ return bkparse::bk_handle_dependency( @item ) }
             | variable binoperand variable binoperand variable binoperand variable
{ return bkparse::bk_handle_dependency( @item ) }
             | variable binoperand variable binoperand variable
{ return bkparse::bk_handle_dependency( @item ) }
             | variable binoperand variable
{ return bkparse::bk_handle_dependency( @item ) }
             | variable binoperand expr
{ return bkparse::bk_handle_dependency( @item ) }
             | variable
{ return bkparse::bk_handle_dependency( @item ) }
    brackexpr: '(' expr ')' | expr
   dependency: depends brackexpr
    condition: /if/i brackexpr
 endcondition: /endif/i
       choice: /choice/i
    endchoice: /endchoice/i
        blank: /^$/ 
      comment: /^#/
      discard: blank | comment
        block: condition 
             | endcondition
             | choice
             | endchoice
   configline: menudef
             | assertion
{ return bkparse::bk_addvariable(@item) }
             | nonuserconf
{ return bkparse::bk_sysvariable(@item) }
             | typedef
             | dependency
             | condition
             | endcondition
             | choice
             | endchoice
             | discard
             | assignment
             | include
    startrule: configline      

_EODEF_

#
# grammar rules for "depends on <expr>"
# where expr is VARIABLE (OP VARIABLE)*
#
$bksolvegrammar = << '_EORULE_';
      integer: /(0-9)+/
       string: /(\".+\")|(\'.+\')/
     variable: /[a-zA-Z_][a-zA-Z0-9_]*/
      boolean: /y/i | /n/i | /t/i | /f/i | /true/i | /false/i | /1/ | /0/
     tristate: /[ymn]/i
     constant: boolean | tristate | integer
     boperand: /&&/ 
             | '||' 
             | 'AND'
             | 'OR'
     coperand: '=='
             | '!='
         expr: variable boperand expr <commit>
{ return bkparse::bk_resolvexpr(@item) }
	     | variable boperand constant
{ return bkparse::bk_resolvexpr(@item) }
             | variable 
{ return bkparse::bk_resolvexpr(@item) }
    brackexpr: '(' expr ')' | expr
    startrule: brackexpr           


_EORULE_
#
# end of grammar
#

#
# grammar for bkconfig.save
# configuration repositories
# similar to ".config" for linux kernels
#
$bkconfiggrammar = << '_EOGRAMMAR_';
      integer: /(0-9)+/
       string: /(\".+\")|(\'.+\')/
     variable: /[a-zA-Z_][a-zA-Z0-9_]*/
      boolean: /[yn]/i
     tristate: /[ymn]/i
     constant: boolean | tristate | integer | string
     variable: /[a-zA-Z_][a-zA-Z0-9_]*/
      comment: /^#/
      blanked: /^$/
       ignore: comment | blanked
      command: variable '=' constant
{ return bkparse::bk_read_var(@item) }
    startrule: command | ignore

_EOGRAMMAR_


#
# exported print functions
#
sub bk_print($)
{
    my($bk_arg)=$_[0];
    printf( "%s %s", $bkprogrampref, $bk_arg );    
}

sub bk_printf
{
    printf("%s ", $bkprogrampref );
    printf(@_);
}
#
# end print functions
#

#
# exported debug level functions
#
sub bk_setdebug($)
{
    my($debugmode)=@_;

    $BKC_DEBUGME = $debugmode;

    return( $BKC_DEBUGME );
}
#
# end debug level functions
#

#
# internal functions
#
sub dprint
{
    print @_ if $BKC_DEBUGME;
}

sub dprintf
{
    printf(@_) if $BKC_DEBUGME;
}
#
# end internal functions
#


#
# lock management functions
#	lock binary semaphore
#	trivial lock with 1 second sleep 
#	to allow thread/schedule execution
#	WARNING! Here there be deadlocks.
#
sub _bk_lock
{
    while( $CONFIG_VAR_LOCK =~ true )
    {
	sleep 1;
    }

    if( $CONFIG_VAR_LOCK =~ false )
    {
	$CONFIG_VAR_LOCK = true;
	$CONFIG_VAR_USED++;
    }
}

#
# sub _bk_unlock
#	unlock a previously gained lock
#	trivial binary semaphore implementation
#
sub _bk_unlock
{
    if( $CONFIG_VAR_LOCK =~ true )
    {
	$CONFIG_VAR_USED--;
	$CONFIG_VAR_LOCK = false;
    }
}

#
# sub _bk_get_lastidx
#	return ($BKC_LASTVARIDX - 1)
#	hopefully, rewritten to be smarter
#	in future.
#
sub _bk_get_lastidx
{
    my $idx;

    if( $BKC_LASTVARIDX > 0 )
    {
	$idx = ($BKC_LASTVARIDX - 1);
    }
    else
    {
	$idx = -1;
    }
    
    return( $idx );
}

#
# _bk_increment_usage
#	increment counting semaphore
#	for debugging "source <filenamepath>" directives
#
sub _bk_increment_usage
{
    $CONFIG_VAR_USED++;
}

#
# _bk_decrement_usage
#    decrement counting semaphore
#	for debugging "source <filenamepath>" directives
#
sub _bk_decrement_usage
{
    $CONFIG_VAR_USED--;
}

#
# sub _bk_strip_quotes(\$refstring)
#	remove quotes from string $refstring
#	callers, take care 
#	as $$ref here is a careless reference.
#
sub _bk_strip_quotes($)
{
    my($ref) = @_;

    $$ref =~ s/(^[\"\'])|([\"\']$)//g;   

    return;
}
#
# end lock management functions
#

#
# parser direct call functions
#
sub bk_setmenu_name
{
    my($menu_name) = $_[2];

    # remove quotes at start and end, but not within
    # $menu_name =~ s/(^[\"\'])|([\"\']$)//g;   
    _bk_strip_quotes( \$menu_name );

    dprintf("bk_setmenu_name: name(%s)\n", $menu_name );

    if( $CONFIG_MNU_ROOT =~ /^$/ )
    {
	$CONFIG_MNU_ROOT = $menu_name;
    }
    else
    {
	my $idx;
	$idx = $CONFIG_MNU_CNTR;
	$CONFIG_MNU_NODE[$idx] = $menu_name;
	$CONFIG_MNU_CNTR = $idx + 1;
    }
    
    return( $menu_name );
}

#
# sub bk_setvariable( $this_varval )
#	set last known variable's 'default' value
#
sub bk_setvariable
{
    my($this_varval) = $_[2];
    my $retval       = 0;
    my $last_varidx;

    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }

    if( $this_varval =~ /^$/ )
    {
	return( $retval );
    }

    _bk_lock;

#    dprintf("bk_setvariable: name = %s, select default value = %s\n", 
#	     $CONFIG_VAR_NAME[$last_varidx], $this_varval );

    $CONFIG_VAR_VALS{$CONFIG_VAR_NAME[$last_varidx]} = $this_varval;

    # default is assigned here for later comparison
    $CONFIG_VAR_INIT{$CONFIG_VAR_NAME[$last_varidx]} = $this_varval;

    _bk_unlock;
    
    $retval = 1;

    return( $retval );
}

#
# sub bk_typevariable( $this_vartype, $this_vardesc )
#	change the type of the last known variable
#	using _bk_get_lastidx() 
#	to $this_vartype
#	and set $CONFIG_VAR_DESC if $this_vardesc
#	is defined.
# 
#
sub bk_typevariable
{
    my($this_vartype) = $_[1];
    my($this_vardesc) = $_[2];

    my $last_varidx;


    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }

    _bk_strip_quotes(\$this_vardesc);

#   dprintf("bk_typevariable: this_vartype = %s", $this_vartype); 

    _bk_lock;

    $CONFIG_VAR_TYPE{$CONFIG_VAR_NAME[$last_varidx]} = $this_vartype;
    if( $this_vardesc !~ /^$/ )
    {
#	dprintf("\tthis_vardesc = %s", $this_vardesc);
	$CONFIG_VAR_DESC{$CONFIG_VAR_NAME[$last_varidx]} = $this_vardesc;
    }
    _bk_unlock;

#   dprintf("\n");

    return( $last_varidx );
}

#
# sub bk_addvariable( $this_varname )
#	add a new variable to CONFIG_VAR_NAMS
#	if not already defined 
#
sub bk_addvariable
{    
    my($this_varname) = $_[1];
    my $bk_idx;
    my $bk_maxvars = $BKC_LASTVARIDX;
    my $retval;

    _bk_lock;

#   dprintf("bk_addvariable: this_varname = %s\n", $this_varname );
    
    for( $bk_idx = 0; $bk_idx < $bk_maxvars; $bk_idx++ )
    {
	# dangerous comparison
	# CONFIG_SYS will not be allowed if CONFIG_SYS_NAMES 
	# is defined first. FIXME!
	if( $CONFIG_VAR_NAME[$bk_idx] =~ $this_varname )
	{
	    $retval = $bk_idx;
	    return( $retval );
	}
    }

    $CONFIG_VAR_NAME[ $bk_maxvars++ ] = $this_varname;
    $CONFIG_IDX_HASH{ $this_varname } = $bk_maxvars - 1;
    $CONFIG_VAR_USER{ $this_varname } = 1; # user can modify this variable

#   dprintf("%s: [%d] %s\n", "bkparse", ($bk_maxvars-1), $CONFIG_VAR_NAME[($bk_maxvars-1)]);

    $BKC_LASTVARIDX = $bk_maxvars;

    _bk_unlock;

    return( $retval = 0 );

}

#
# sub bk_sysvariable( $this_varname )
#	add a new variable to CONFIG_VAR_NAMS
#	if not already defined 
#	WARNING! variable cannot be modified by 
#	user using config, oldconfig, menuconfig
#
sub bk_sysvariable
{    
    my($this_varname) = $_[1];
    my $bk_idx;
    my $bk_maxvars = $BKC_LASTVARIDX;
    my $retval;

    _bk_lock;

#   dprintf("bk_addvariable: this_varname = %s\n", $this_varname );
    
    for( $bk_idx = 0; $bk_idx < $bk_maxvars; $bk_idx++ )
    {
	# dangerous comparison
	# CONFIG_SYS will not be allowed if CONFIG_SYS_NAMES 
	# is defined first. FIXME!
	if( $CONFIG_VAR_NAME[$bk_idx] =~ $this_varname )
	{
	    $retval = $bk_idx;
	    return( $retval );
	}
    }

    $CONFIG_VAR_NAME[ $bk_maxvars++ ] = $this_varname;
    $CONFIG_IDX_HASH{ $this_varname } = $bk_maxvars - 1;
    $CONFIG_VAR_USER{ $this_varname } = 0; # user cannot modify this variable

#   dprintf("%s: [%d] %s\n", "bkparse", ($bk_maxvars-1), $CONFIG_VAR_NAME[($bk_maxvars-1)]);

    $BKC_LASTVARIDX = $bk_maxvars;

    _bk_unlock;

    return( $retval = 0 );

}


#
# sub _bk_normalise_operator( $operator )
#	convert && and || to AND and OR
#	to aid future evaluation
#
#
sub _bk_normalise_operator($)
{
    my($operator)=@_;

    $operator =~ s/\&\&/AND/g;
    $operator =~ s/\|\|/OR/g;

    return( $operator );
}

#
# sub bk_handle_dependency( $expr )
#	handle dependency expressions from the parser
#	convert expressions to AND/OR deplying 
#	_bk_normalise_operator($)
#
sub bk_handle_dependency
{

    my $depth = 0;
    shift;

    my $reply;
    my $temp;
    
    foreach $itemname (@_) 
    {
	$temp = $itemname;
	if( $this_parser->binoperand( $temp ) )
	{
	    $temp  = _bk_normalise_operator( $temp );
	    $reply = $reply . ' ' . $temp . ' ';
	}
	else
	{
	    $reply = $reply . $itemname;
	}
    }
  
#   dprintf(">> %s\n", $reply );

    my $this_dependency = $reply;

    my $last_varidx;
    
    chomp( $this_dependency );

    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }

    _bk_lock;

    $last_varname = $CONFIG_VAR_NAME[$last_varidx];
    $prev_deps    = $CONFIG_VAR_DEPS{$last_varname};

    if( $prev_deps =~ /^$/ )
    {
	$prev_deps = $this_dependency;
	$prev_deps =~ s/^ +//g;
	$prev_deps =~ s/ +$//g;
	$CONFIG_VAR_DEPS{$last_varname} = $prev_deps;
    }
    else
    {
	$prev_deps = $prev_deps . " AND " . $this_dependency;
	$prev_deps =~ s/^ +//g;
	$prev_deps =~ s/ +$//g;
	$CONFIG_VAR_DEPS{$last_varname} = $prev_deps;
    }

    _bk_unlock;

#    dprintf("bk_add_dependency: V(%s) P(%s) +D(%s) C(%d)\n", 
#	    $last_varname, $prev_deps, $this_dependency, $this_argcount);

}

#
# sub bk_resolvexpr
#	process rule for 'expr' grammar
#	or '(' expr ')' grammar
#	and assemble a boolean expression
#
# WARNING! misleading, this name is.
#
sub bk_resolvexpr
{
    shift;
    my ($lhs,$op,$rhs) = @_;
    my $retval = 0;
    my $varval;

    dprintf("bk_resolvexpr: %s <%s> %s\n", $lhs, $op, $hs );

    $varval = $CONFIG_VAR_VALS{$lhs};
    
    if( ($last_boolean !~ /^$/) && ($op !~ /^$/) )
    {
	$last_boolean = $last_boolean . " " . $op . " " . $varval;
    }
    else
    {
	$last_boolean = $varval;
    }

    return( $retval );
}

#
# sub bk_evaluate( $expr )
#	evaluate a boolean expression
#	by assigning 1, 0 and '*', '+'
#	and evaluating arithmetically.
#
sub bk_evaluate($)
{
    my($bool_expr) = @_;

    my $result = 0;

    if( $bool_expr =~ /^$/ )
    {
	return( $result );
    }

    $bool_expr =~ s/and/\*/ig;
    $bool_expr =~ s/or/\+/ig;

    $bool_expr =~ s/y/1/ig;
    $bool_expr =~ s/m/1/ig;
    $bool_expr =~ s/n/0/ig;

#    dprintf("bk_evaluate: expr (%s)\n", $bool_expr );
    $result = eval $bool_expr;

    return( $result );
}

#
# sub bk_read_var( $var, $op, $val )
#	read a variable from a .bkconfig file
#	to fill defaults and ease user input
#	for configuration.
#
sub bk_read_var
{
    shift;

    my($variable, $op, $value) = @_;
    my $refidx;
    my $varusr;

    $varusr = $CONFIG_VAR_USER{$variable};
    $refidx = $CONFIG_IDX_HASH{$variable};
    $value  = $CONFIG_VAR_INIT{$variable} if( $varusr == 0 );
    $CONFIG_VAR_VALS{$variable} = $value;  

    dprintf("read> variable(%s) <= value(%s) [%d] \n", $variable, $value, $refidx );
}

#
# sub _bk_refresh_deps( $idx )
#	refresh dependency for $CONFIG_VAR_NAME[$idx]
#	by using $CONFIG_VAR_DEPS and $CONFIG_VAR_VALS hash
# Results in
#	and ultimately setting $CONFIG_VAR_STAT
#
# WARNING
#	uses _bk_lock
#	cannot be called from a function 
#	which also uses _bk_lock and 
#	cannot call a function which 
#	uses _bk_lock. Here there be deadlocks.
#
sub _bk_refresh_deps($)
{
    my($idx) = @_;
    my $config_varname;
    my $config_vardeps;
    my $config_resolvdeps;
    my $config_result;

    my $resolv_parser = Parse::RecDescent->new( $bksolvegrammar );

    $config_varname = $CONFIG_VAR_NAME[$idx];
    $config_vardeps = $CONFIG_VAR_DEPS{$config_varname};

{
    _bk_lock;

    if( $config_vardeps =~ /^$/ )
    {
	$config_resolvdeps = 1;
	$config_result = $config_resolvdeps;
	$CONFIG_VAR_STAT{$config_varname} = $config_resolvdeps;
	$last_boolean = "";	# global

    }
    else
    {
	$last_boolean = "";	# global

	# compute dependency on expression
	$resolv_parser->startrule( $config_vardeps );

	dprintf("refresh_deps: resolv expr(%s) rule(%s)\n", $config_vardeps, $last_boolean);
	$config_result = bk_evaluate( $last_boolean );
	$CONFIG_VAR_STAT{$config_varname} = $config_result;
	
    }

    _bk_unlock;

}

    return( $config_result );

}

#
# sub _bk_update_deps
#	O(N) update all deps
#	heavy in hash usage
#	@see _bk_refresh_deps( $idx )
#
sub _bk_update_deps
{
    my $last_varidx = _bk_get_lastidx();
    my $idx = 0;

    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }

    for( $idx = 0; $idx < $last_varidx; $idx++ )
    {
	_bk_refresh_deps( $idx );
    }

    return( $idx );
}

#
# sub _bk_sync_deps
#	O(N) worst case per call
#	revalidates $CONFIG_VAR_STAT after being called
#
sub _bk_sync_deps
{
    my($config_var_updated)=@_;
    my $retval = 0;
    
    if( $config_var_updated =~ /^$/ )
    {
	return ( $retval = -1 );
    }

    my $ctr;
    my $varname;
    my $varidx;

    for( $ctr = 0; $ctr < $last_varidx; $ctr++ )
    {
	$varname = $CONFIG_VAR_NAME[$ctr];

	if( $CONFIG_VAR_DEPS{$varname} =~ $config_var_updated )
	{
	    _bk_refresh_deps( $ctr );
	}
    }

    return( $retval );
}

#
# sub _bk_is_input_valid($$)
#	check if input is valid
#	and return 1 if yes, 0 if not
#
sub _bk_is_input_valid($$)
{
    my($varval,$vartype) = @_;

    my $retval = 0;

    if( $vartype =~ /bool/i )
    {
	if( $varval =~ /[yn]/i && (length( $varval ) == 2))
	{
	    return( $retval = 1 );
	}
       
    }

    if( $vartype =~ /tristate/i )
    {
	if( ($varval =~ /[ymn]/i) && (length( $varval ) == 2) )
	{
	    return( $retval = 1 );
	}	
    }

    if( $vartype =~ /integer/i )
    {
	if( $varval =~ /[+-]*(0-9)+/ )
	{
	    return( $retval = 1 );
	}
    }

    if( $vartype =~ /string/i )
    {
	if( ($varval !~ /^$/ ) && 
	    ($varval =~ /[a-zA-Z0-9_=.+-\<\>\.,`;:\(\)\{\}\[\]\&\$\@\!\~\"\'\!\?\#\*\^\t ]*/ ) )
	{
	    return( $retval = 1 );
	}
    }

    return( $retval = 0 );
}

#
# sub bk_config_commandline
#	similar to make configure
#	for linux kernels, excepting that no input is not accepted
# FIXME! allow no input, if default is specified
#
sub bk_config_commandline
{
    my $n_idx;

    printf("MENU .-( %s )-.\n", $CONFIG_MNU_ROOT );
    
    printf("\n");

    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }  
    
    my $user_choice;
    my $user_prompt;
    my $default_str;
    my $input_valid;

    for( $n_idx = 0; $n_idx < $last_varidx; $n_idx++ )
    {
	$user_choice = "";

	$varname = $CONFIG_VAR_NAME[$n_idx];
	$vartype = $CONFIG_VAR_TYPE{ $varname };
	$varuser = $CONFIG_VAR_USER{ $varname };
	$varstat = $CONFIG_VAR_STAT{ $varname };

	if( ($varstat == 1) && ($varuser == 1) )
	{
	    printf(  "  %s (CONFIG_%s) ", $CONFIG_VAR_DESC{ $varname }, $varname );

	    $user_prompt = "[y/n]"    if ($vartype =~ /bool/i );
	    $user_prompt = "[y/m/n]"  if ($vartype =~ /tristate/i );
	    $user_prompt = "[number]" if ($vartype =~ /integer/i );
	    $user_prompt = "[string]" if ($vartype =~ /string/i );

	    if( ($vartype =~ /bool/i) || ($vartype =~ /tristate/i ) )
	    {
		$default_str = $CONFIG_VAR_INIT{ $varname };
		if( $default_str =~ /y/i )
		{
		    $user_prompt =~ s/y/Y/;
		}
		if( $default_str =~ /n/i )
		{
		    $user_prompt =~ s/n/N/;
		}
		if( $default_str =~ /m/i )
		{
		    $user_prompt =~ s/m/M/;
		}
	    }

	    printf("%s ", $user_prompt );

	    printf("Default(%s) ", $CONFIG_VAR_VALS{ $varname }) if ( $CONFIG_VAR_VALS{$varname} !~ /^$/ );
	    
	    printf("> ");
	    $user_choice = <STDIN>;
	    
	    $input_valid = _bk_is_input_valid( $user_choice, $vartype );
	    while( ($user_choice =~ /^$/) || (0 == $input_valid) )
	    {
		if( ($default_str !~ /^$/) )
		{
		    $user_choice = $default_str;
		    $input_valid = 1;
		}
		else
		{
		    printf("[Invalid! try again] %s ", $user_prompt );
		    printf("Default(%s) ", $CONFIG_VAR_VALS{ $varname }) 
			if( $CONFIG_VAR_VALS{ $varname } !~ /^$/ );
		    printf("> ");
		    $user_choice = <STDIN>;
		    $input_valid = _bk_is_input_valid( $user_choice, $vartype );
		}
	    }
	    
	    printf("\n");

	    chomp( $user_choice );
	    
	    $CONFIG_VAR_VALS{ $varname } = $user_choice;

	    _bk_sync_deps( $varname );
	}

    }

    printf("MENU <Ends>\n\n");

    return( $n_idx );
}

#
# sub _bk_dump_vars
#	dump vars 0 ... $idx
#	for debug purposes
#	provide NAME, VALS, TYPE, DESC, DEPS, STAT
#
#	Usage: For debugging only
#
sub _bk_dump_vars
{
    my $last_varidx;

    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }

    _bk_lock;
    my $idx;
    for ( $idx = 0; $idx < $last_varidx; $idx++ )
    {
	printf("BK_VAR: N(%s) = V(%s) / \n\tT(%s) / D(%s) / R(%s) / S(%s)\n",
	       $CONFIG_VAR_NAME[$idx], 
	       $CONFIG_VAR_VALS{$CONFIG_VAR_NAME[$idx]},
	       $CONFIG_VAR_TYPE{$CONFIG_VAR_NAME[$idx]},
	       $CONFIG_VAR_DESC{$CONFIG_VAR_NAME[$idx]},
	       $CONFIG_VAR_DEPS{$CONFIG_VAR_NAME[$idx]},
	       $CONFIG_VAR_STAT{$CONFIG_VAR_NAME[$idx]});
    }
    _bk_unlock;
}

#
# sub bk_write_config( $filenamepath )
#	write configuration to ".config" like
#	file specified in '$filenamepath'
#
sub bk_write_config
{
    my($filenamepath) = @_;
    my $ret = 0;
    my $last_varidx;

    local *BKCONFIG;

    open( BKCONFIG, ">" . $filenamepath ) or return( $ret = -1 );

    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }  


    printf( BKCONFIG "#\n");
    printf( BKCONFIG "# config.bkconfig automatically generated\n");
    printf( BKCONFIG "#\n");

    _bk_lock;
    {
	my $idx;
	my $var;
	my $val;
	my $dep;
	for( $idx = 0; $idx < $last_varidx; $idx++ )
	{
	    $var = $CONFIG_VAR_NAME[$idx];
	    $val = $CONFIG_VAR_VALS{$var};
	    $dep = $CONFIG_VAR_STAT{$var};

	    printf( BKCONFIG "%s = %s\n", $var, $val ) if ($dep == 1);
	}

    }
    _bk_unlock;

    printf( BKCONFIG "#\n");
    printf( BKCONFIG "# end of config.bkconfig\n");
    printf( BKCONFIG "#\n");

    close( BKCONFIG );

    return( $ret );

}

#
# sub _bk_dump_header( $filenamepath )
#	dump header file, usually bkconfig.h
#	takes O(N) time always, N being the number
#	of variables asserted using *.bconfig
#
sub _bk_dump_header
{
    my($filenamepath) = @_;
    my $ret = 0;
    my $last_varidx;
    
    local *BKCONFHEADER;

    if( 0 > ($last_varidx = _bk_get_lastidx()) )
    {
	return( $last_varidx );
    }  

    # overwrite any existing files without check, default behavior
    open( BKCONFHEADER, ">" . $filenamepath ) or return( $ret = -1);

    printf(BKCONFHEADER "\n/*\n");
    printf(BKCONFHEADER " * Betakit Autogenerated %s \n", $filenamepath);
    printf(BKCONFHEADER " * Manual editing is not advised. \n");
    printf(BKCONFHEADER " */\n\n");

    printf(BKCONFHEADER "#ifndef _BKCONFIG_H_INC\n");
    printf(BKCONFHEADER "#define _BKCONFIG_H_INC\n\n");

    _bk_lock;
    {
	my $ctr;
	my $var;
	my $val;
	for( $ctr = 0; $ctr < $last_varidx; $ctr++ )
	{
	    $var = $CONFIG_VAR_NAME[$ctr];
	    $val = $CONFIG_VAR_VALS{$var};
	    $com = $CONFIG_VAR_DESC{$var};

	    $var = "CONFIG_" . $var;

	    if( $BKC_DEBUGME == 1 )
	    {
		if( $com !~ /^$/ )
		{
		    printf(BKCONFHEADER "\t\t/* (%s) %s */\n", $var, $com );
		}
	    }

	    if( $val !~ /n/i )
	    {
		printf(BKCONFHEADER "#define %s\t%s\n", $var, $val);
		
	    }
	    else
	    {
		printf(BKCONFHEADER "#undef %s\n", $var);
	    }
	}


    }
    _bk_unlock;

    printf(BKCONFHEADER "\n");
    printf(BKCONFHEADER "#endif /* _BKCONF_H_INC */\n");

    printf(BKCONFHEADER "\n/*\n");
    printf(BKCONFHEADER " * end of file (%s)\n", $filenamepath);
    printf(BKCONFHEADER " */\n\n");

    close( BKCONFHEADER );

    return( $ret );
}

#
# bk_parse_source cannot lock
# because it can be called recursively
# hence _bk_increment_usage is used to 
# set depth of recursion without locking.
#
sub bk_parse_source
{
    my($source_filename) = $_[2];
    my $ret = 0;
    my $_this_parser;

    _bk_strip_quotes( \$source_filename );

    local *BKSUBPARSEFILE;
    open( BKSUBPARSEFILE, "<" . $source_filename ) or return($ret = -1);
    
    _bk_increment_usage();
    {

#	dprintf("%s: using source file (%s)\n", "bk_parse_source", $source_filename);
	$_this_parser = Parse::RecDescent->new( $bkparsegrammar );

	while(<BKSUBPARSEFILE>)
	{
	    my $_this_text = $_;
	    $_this_parser->startrule($_this_text);
	}
    }
    _bk_decrement_usage();

    close( BKSUBPARSEFILE );

    return($ret);
}

#
# bk_parse_config ($save_filename)
#	parse a ".bkconfig" file
#	which has 'VARIABLE = <value>'
#	format
#
sub bk_parse_config
{
    my($save_filename) = @_;
    my $ret = 0;

    local *BKCONFIGFILE;

    dprintf("oldconfig: file <%s>\n", $save_filename );

    open( BKCONFIGFILE, "<" . $save_filename ) or (return $ret = -1);

    _bk_lock;
    {
	my $_config_parser;
	$_config_parser = Parse::RecDescent->new( $bkconfiggrammar );
	while( <BKCONFIGFILE> )
	{
	    my $_txt = $_;
	    $_config_parser->startrule($_txt);
	}
    }
    _bk_unlock;

    close( BKCONFIGFILE );

    return( $ret );
}

#
# end direct call functions
#

#
# end function set
#



#
# exported parse functions
# sub _bk_parse( $filenamepath )
#	parse a config file which 
#	uses *.bconfig style config
#	definitions.
#
sub bk_parse
{
    my($configfile, $headerfile, $oldconfig, $force) = @_;

    my $fnamepath = $configfile;

    dprintf("bk_parse: %s (begin)\n", $fnamepath);

    $Parse::RecDescent::skip = '([ \s\t]+)|(\s*)';

    $this_parser = Parse::RecDescent->new( $bkparsegrammar );

    open( BKPARSEFILE, "<" . $fnamepath ) or die("error: cannot open $fnamepath\nerror: exiting!\n\n");

    while( <BKPARSEFILE> )
    {
	my $text_line = $_;	# read the line
	$this_parser->startrule( $text_line );
    }

    dprintf("bk_parse: %s (finis)\n", $fnamepath);

    bk_parse_config( $oldconfig );

    _bk_update_deps;

    _bk_dump_vars if ( 1 == $BKC_DEBUGME );

    # using $force = 'y' will avoid user input
    if( $force =~ /y/i )
    {
	bk_printf("using default options from (%s)\n", $oldconfig );
    }
    else
    {
	bk_config_commandline;
    }

    close( BKPARSEFILE );

    _bk_dump_header($headerfile);

    if( $oldconfig !~ /^$/ )
    {
	bk_write_config($oldconfig); # save choices to .bkconfig
    }

}
#
# end parse set
#

1;

# END OF FILE
