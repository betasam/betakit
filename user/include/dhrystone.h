/**
 * @file dhrystone.h
 * @brief header file for dhrystone calculation
 *
 * Provides basic definitions used for providing
 * dhrystone 2.1 calculations from libbsys.
 */

#ifndef _DHRYSTONE_H_INC
#define _DHRYSTONE_H_INC

/* Macro definitions */
#define uSecs_per_Second	1000000.0
#define TRUE			1
#define FALSE			0
#define INTARRAYSIZE		50
#define CHRARRAYSIZE		31

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
  double real_time_taken;
  double dhrystones;
  double vaxmips;
  double loop_usecs;
  long total_loops;
} dhrystone_result_t;

/* function declarations */
double d_time( void );
int calculate_dhrystone (double , dhrystone_result_t *);

#endif /* _DHRYSTONE_H_INC */
