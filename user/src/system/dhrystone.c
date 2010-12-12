/**
 * @file dhrystone.c
 * @brief Compute system dhrystone C/2.1 MIPS
 *
 * Computes system dhrystone MIPS on the basis of 
 * Algorithm v2.1. This was written to simplify
 * extremely archaic code to remain portable 
 * exclusively on linux systems. Many options are
 * gcc friendly and the results too will be.
 */

/** @remark standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @remark betakit includes */
#include <memory.h>
#include <dhrystone.h>
#include <ops.h>

/* @remark more macro definitions */
#define		MIN_TIME	2 /* in seconds */

/* @remark global variables */
record_ptr	ptr_glob, ptr_next_glob, next_ptr_glob;
int		int_glob;
boolean		bool_glob;
char		ch_1_glob, ch_2_glob;

int		arr_1_glob[INTARRAYSIZE];
int		arr_2_glob[INTARRAYSIZE][INTARRAYSIZE];

double		begin_time, end_time, user_time;
double		uSecs, dhrystones_per_second, vax_mips;

/* @remark constants */
const char reg_define[] = "Register operands used.";

/* @remark function declaration */
identenum	func_1 (capital_letter, capital_letter);
boolean		func_2 (str_30, str_30);
boolean		func_3 (identenum);

void		proc_1 (record_ptr);
void		proc_2 (one_fifty *);
void		proc_3 (record_ptr *);
void		proc_4 (void);
void		proc_5 (void);
void		proc_6 (identenum, identenum *);
void		proc_7 (one_fifty , one_fifty , one_fifty*);
void		Proc_8 (array_1_dim , array_2_dim , int, int);


void proc_1 (_REGISTER_ record_ptr ptr_val_par)     /* @remark executed once */
{
  _REGISTER_ record_ptr next_record = ptr_val_par->ptr_comp;
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  _OPS_ASSIGN_SZ(*ptr_val_par->ptr_comp, *ptr_glob);

  ptr_val_par->variant.var_1.int_comp = 5;
  next_record->variant.var_1.int_comp = ptr_val_par->variant.var_1.int_comp;

  next_record->ptr_comp = ptr_val_par->ptr_comp;

  proc_3 (&next_record->ptr_comp);

    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                        == Ptr_Glob->Ptr_Comp */

  if(next_record->discr == ident_1)
    {
      next_record->variant.var_1.int_comp = 6;
      
      proc_6 (ptr_val_par->variant.var_1.enum_comp,
	      &next_record->variant.var_1.enum_comp);

      next_record->ptr_comp = ptr_glob->ptr_comp;

      proc_7 (next_record->variant.var_1.int_comp, 10,
	      &next_record->variant.var_1.int_comp);
    }
  else
    {
      _OPS_ASSIGN_SZ(*ptr_val_par, *ptr_val_par->ptr_comp);
    }

  return; /* proc_1 */
} 


void proc_2 (one_fifty *int_par_ref)
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */
{
  one_fifty	int_loc;
  identenum	enum_loc = ident_1;

  int_loc = *int_par_ref + 10;

  do {
    if ('A' == ch_1_glob)
    {
      int_loc -= 1;
      *int_par_ref = int_loc - int_glob;
      enum_loc = ident_1;
    } 
  } while (enum_loc != ident_1);

  return; /* proc_2 */
} 


void proc_3 (record_ptr *ptr_ref_par)
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */
{
  if(NULL != ptr_glob)
    {
      *ptr_ref_par = ptr_glob->ptr_comp;
    }

  proc_7 (10, int_glob, &ptr_glob->variant.var_1.int_comp);

  return; /* proc_3 */
} 

void proc_4 (void) 
{
  boolean bool_loc;

  bool_loc = ch_1_glob == 'A';
  bool_glob = bool_loc | bool_glob;
  ch_2_glob = 'B';
  
  return; /* proc_4 */
} 

void proc_5 (void)
{
  ch_1_glob = 'A';
  bool_glob = FALSE;

  return; /* proc_5 */
} 


void proc_6 (identenum enum_val_par, identenum *enum_ref_par)
    /* executed once */
    /* Enum_Val_Par == Ident_3, Enum_Ref_Par becomes Ident_2 */
{
  *enum_ref_par = enum_val_par;

  if ( 0 != func_3 (enum_val_par))
    {
      *enum_ref_par = ident_4;
    }

  switch (enum_val_par)
  {
    case ident_1:
      *enum_ref_par = ident_1;
      break;

    case ident_2:
      if (int_glob > 100)
	{
	  *enum_ref_par = ident_1;
	}
      else 
	{
	  *enum_ref_par = ident_4;
	}
      break;

    case ident_3: 		/* executed */
      *enum_ref_par = ident_2;
      break;

    case ident_4: 
      break;

    case ident_5:
      *enum_ref_par = ident_3;
      break;  
  } 
  return; /* proc_6 */
} 


void proc_7 (one_fifty int_1_par_val, one_fifty int_2_par_val, one_fifty *int_par_ref)
    /* executed three times                                      */
    /* first call:      Int_1_Par_Val == 2, Int_2_Par_Val == 3,  */
    /*                  Int_Par_Ref becomes 7                    */
    /* second call:     Int_1_Par_Val == 10, Int_2_Par_Val == 5, */
    /*                  Int_Par_Ref becomes 17                   */
    /* third call:      Int_1_Par_Val == 6, Int_2_Par_Val == 10, */
    /*                  Int_Par_Ref becomes 18                   */
{
  one_fifty int_loc;

  int_loc = int_1_par_val + 2;
  *int_par_ref = int_2_par_val + int_loc;
  
  return; /* proc_7 */
} 


void proc_8 (array_1_dim arr_1_par_ref, array_2_dim arr_2_par_ref, int int_1_par_val, int int_2_par_val)
    /* executed once      */
    /* Int_Par_Val_1 == 3 */
    /* Int_Par_Val_2 == 7 */
{
  _REGISTER_ one_fifty int_index;
  _REGISTER_ one_fifty int_loc;

  int_loc = int_1_par_val + 5;

  arr_1_par_ref [int_loc] = int_2_par_val;

  arr_1_par_ref [int_loc +  1] = arr_1_par_ref [int_loc];
  arr_1_par_ref [int_loc + 30] = int_loc;


  for (int_index = int_loc; int_index <= int_loc+1; ++int_index)
    {
      arr_2_par_ref [int_loc] [int_index] = int_loc;
    }

  arr_2_par_ref [int_loc] [int_loc-1] += 1;
  arr_2_par_ref [int_loc+20] [int_loc] = arr_1_par_ref [int_loc];

  int_glob = 5;

  return; /* proc_8 */
} 


identenum func_1 (capital_letter ch_1_par_val, capital_letter ch_2_par_val)
    /* executed three times                                         */
    /* first call:      Ch_1_Par_Val == 'H', Ch_2_Par_Val == 'R'    */
    /* second call:     Ch_1_Par_Val == 'A', Ch_2_Par_Val == 'C'    */
    /* third call:      Ch_1_Par_Val == 'B', Ch_2_Par_Val == 'C'    */
{
  capital_letter        ch_1_loc;
  capital_letter        ch_2_loc;

  ch_1_loc = ch_1_par_val;
  ch_2_loc = ch_1_loc;

  if (ch_2_loc != ch_2_par_val)
    {
      return (ident_1);
    }
  else  /* not executed */
    {
      ch_1_glob = ch_1_loc;
      return (ident_2);
    }

  return (ident_1); /* func_1 */
} 


boolean func_2 (str_30 str_1_par_ref, str_30 str_2_par_ref)
    /* executed once */
    /* Str_1_Par_Ref == "DHRYSTONE PROGRAM, 1ST STRING" */
    /* Str_2_Par_Ref == "DHRYSTONE PROGRAM, 2ND STRING" */
{
  _REGISTER_ one_thirty		int_loc;
  capital_letter		ch_loc = 0;

  int_loc = 2;

  while (int_loc <= 2) /* loop body executed once */
    {

      if (ident_1 == func_1 (str_1_par_ref[int_loc], str_2_par_ref[int_loc+1]))
	{
	  ch_loc = 'A';
	  int_loc += 1;
	} 
    }


  if ( (ch_loc >= 'W') && (ch_loc < 'Z'))
    {
      int_loc = 7;
    }


  if (ch_loc == 'R')
    {
      return (TRUE);
    }
  else 
    {
      if (0 < strcmp (str_1_par_ref, str_2_par_ref))
	{
	  int_loc += 7;
	  int_glob = int_loc;
	  return (TRUE);
	}
      else 
	{
	  return (FALSE);
	}
    } 

  return (FALSE); /* func_2 */
} 


boolean func_3 (identenum enum_par_val)
    /* executed once        */
    /* Enum_Par_Val == Ident_3 */
{
  identenum enum_loc;

  enum_loc = enum_par_val;

  if (enum_loc == ident_3)
    {
      return (TRUE);
    }
  else
    {
      return (FALSE);
    }

  return (FALSE); /* func_3 */
} 


t_s32 calculate_dhrystone (t_d64 time, t_dhrystone_result *results)
  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{

  _REGISTER_   one_fifty       int_2_loc = ZERO;
  _REGISTER_   char            ch_index;
  _REGISTER_   int             run_index;
  _REGISTER_   int             number_of_runs;
  _REGISTER_   unsigned long   total_runs = ZERO;

  one_fifty       int_1_loc;
  one_fifty       int_3_loc;
  identenum       enum_loc;
  str_30          str_1_loc;
  str_30          str_2_loc;


  next_ptr_glob = (record_ptr) mem_alloc (sizeof (record_type));
  ptr_glob	= (record_ptr) mem_alloc (sizeof (record_type));


  ptr_glob->ptr_comp                    = next_ptr_glob;
  ptr_glob->discr                       = ident_1;
  ptr_glob->variant.var_1.enum_comp     = ident_3;
  ptr_glob->variant.var_1.int_comp      = 40;

  strcpy (ptr_glob->variant.var_1.str_comp,
          "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (str_1_loc, "DHRYSTONE PROGRAM, 1ST STRING");

  arr_2_glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */


  begin_time = d_time();	/* Start Timer */

 dhrystone_loop:
  number_of_runs = 1000;
  total_runs += number_of_runs;
  for (run_index = 1; run_index <= number_of_runs; ++run_index)
  {

    proc_5();
    proc_4();

    int_1_loc = 2;
    int_2_loc = 3;

    strcpy (str_2_loc, "DHRYSTONE PROGRAM, 2ND STRING");

    enum_loc	= ident_2;
    bool_glob	= ! func_2 (str_1_loc, str_2_loc);

    while (int_1_loc < int_2_loc)  /* loop body executed once */
      {
	int_3_loc = 5 * int_1_loc - int_2_loc; /* 7 */

	proc_7 (int_1_loc, int_2_loc, &int_3_loc);

	int_1_loc += 1;
      } 

    proc_8 (arr_1_glob, arr_2_glob, int_1_loc, int_3_loc);
    proc_1 (ptr_glob);

    for (ch_index = 'A'; ch_index <= ch_2_glob; ++ch_index)
      {
	if (enum_loc == func_1 (ch_index, 'C'))
	  {
	    proc_6 (ident_1, &enum_loc);
	    strcpy (str_2_loc, "DHRYSTONE PROGRAM, 3RD STRING");
	    int_2_loc = run_index;
	    int_glob = run_index;
	  }
      }

    int_2_loc = int_2_loc * int_1_loc;
    int_1_loc = int_2_loc / int_3_loc;
    int_2_loc = 7 * (int_2_loc - int_3_loc) - int_1_loc;

    proc_2 (&int_1_loc);

  } 

  end_time = d_time();		/* End_Time */
  user_time = end_time - begin_time;

  if (user_time < time)
  {
    goto dhrystone_loop;
  }
  else
  {
    uSecs = user_time * uSecs_per_Second / (double) total_runs;
    dhrystones_per_second = (double) total_runs / user_time;
    vax_mips = dhrystones_per_second / VAX_DHRYSTONES_PER_SEC;
    
    results->dhrystones = dhrystones_per_second;
    results->vaxmips    = vax_mips;
    results->loop_usecs = uSecs;

    results->real_time_taken	= user_time;
    results->total_loops        = total_runs;

    return(0);
  }

  return(-1); /* This line is never executed */
}

/* @remark end of file "dhrystone.c" */
