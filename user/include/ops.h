/**
 * @file ops.h
 * @author Sunil Beta <betasam@gmail.com>
 * @brief Minimal data operation macros
 */

#ifndef _OPS_H_INC
#define _OPS_H_INC

/**
 * @def _OPS_SWAP(X,Y)
 * @brief swaps X and Y as long as they are basic types.
 */
#define _OPS_SWAP(X,Y) { X ^= Y; Y ^= X; X ^= Y; }

/**
 * @def _OPS_BIT(N)
 * @brief provides the Nth bit.
 *
 * N = {0 ... Register width for CPU}
 *
 */
#define _OPS_BIT(N) (1<<N)

/**
 * @def _OPS_BITSTATE(X,N)
 * @brief returns the state of the Nth bit in X.
 *
 * N = {0 ... width of datatype of X}
 */
#define _OPS_BITSTATE(X,N) ( (X>>N)&1 )


#define _OPS_ASSIGN(L,R)	{ L = R; }
#define _OPS_ASSIGN_SZ(L,R)	{ mem_copy( &(L), &(R), sizeof(L)); }

/**
 * @def _OPS_CMP(X,Y)
 * @brief compares X and Y (of same type)
 * @return 1 if x == y or x or y = 1, x = 0 (for all bits of x,y)
 */
#define _OPS_CMP(X,Y)		((0 == (X ^ Y)) ? 1 : 0 )

#endif /* _OPS_H_INC */
