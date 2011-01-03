/**
 * @file dhrystone.h
 * @brief header file for dhrystone calculation
 *
 * Provides basic definitions used for providing
 * dhrystone 2.1 calculations from libbsys.
 */

#ifndef _DHRYSTONE_H_INC
#define _DHRYSTONE_H_INC

#include <bkconfig.h>

#ifdef CONFIG_BK_SYS_DHRYSTONES

/* Macro definitions */
#define uSecs_per_Second	1000000.0
#define TRUE			1
#define FALSE			0
#define INTARRAYSIZE		50
#define CHRARRAYSIZE		31
#define VAX_DHRYSTONES_PER_SEC	1757.0

/* Betakit Defaults */
#define DHRYSTONE_TEST_TIME	5.0

#define _REGISTER_		register

/* Type definitions */
typedef enum {ident_1, ident_2, ident_3, ident_4, ident_5} identenum; 
typedef char	str_30[CHRARRAYSIZE];
typedef int	array_1_dim[INTARRAYSIZE]; 
typedef int	array_2_dim[INTARRAYSIZE][INTARRAYSIZE];
typedef int	boolean;

typedef int	one_thirty;
typedef int	one_fifty;
typedef char	capital_letter;

/* Structured type definitions */
typedef struct record {
    struct record	*ptr_comp;
    identenum		discr;

    union {
        struct {
            identenum	enum_comp;
            int         int_comp;
            char        str_comp [CHRARRAYSIZE];
        } var_1;

        struct {
            identenum   e_comp_2;
            char        str_2_comp [CHRARRAYSIZE];
        } var_2;

        struct {
            char        ch_1_comp;
            char        ch_2_comp;
        } var_3;

    } variant;

} record_type, *record_ptr;


typedef struct dhrystone_result_s {
  t_d64 real_time_taken;
  t_d64 dhrystones;
  t_d64 vaxmips;
  t_d64 loop_usecs;
  t_l32 total_loops;
} t_dhrystone_result;

/* function declarations */
t_d64 d_time( t_void );
t_s32 calculate_dhrystone (t_d64, t_dhrystone_result*);

#endif /* CONFIG_BK_SYS_DHRYSTONES */

#endif /* _DHRYSTONE_H_INC */
