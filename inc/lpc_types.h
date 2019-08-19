#ifndef _TYPES_H_
#define _TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

/* NULL pointer */
#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef INLINE
#define INLINE inline
#endif

#define		__R		volatile const
#define		__W		volatile
#define		__RW	volatile

/*
typedef 	signed int		int32_t;
typedef 	signed short	int16_t;
typedef 	signed char		int8_t;
typedef 	unsigned int 	uint32_t;
typedef 	unsigned short 	uint16_t;
typedef 	unsigned char 	uint8_t;
*/

//typedef enum {false = 0, true = !false} bool;
typedef enum {RESET = 0, SET = !RESET} flagStatus, intStatus, setState;
typedef enum {ERROR = 0, SUCCESS = !ERROR} status;


/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#ifndef _BIT
#define _BIT(n) (1UL << (n))
#endif

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
#ifndef _SBF
#define _SBF(f, v) ((v) << (f))
#endif

/* _BITMASK constructs a symbol with 'field_width' least significant
 * bits set.
 * e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
 * The symbol is intended to be used to limit the bit field width
 * thusly:
 * <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
 * If "any_expression" results in a value that is larger than can be
 * contained in 'x' bits, the bits above 'x - 1' are masked off.  When
 * used with the _SBF example above, the example would be written:
 * a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
 * This ensures that the value written to a_reg is no wider than
 * 16 bits, and makes the code easier to read and understand.
 */
#define _BITMASK(field_width) ( _BIT(field_width) - 1)

#define _RESET_BIT(n) &= ~ _BIT(n)
#define _SET_BIT(n)   |=   _BIT(n)
#define clearBIT(n)   &= ~ _BIT(n)
#define setBIT(n)     |=   _BIT(n)


/* Number of elements in an array */
#define NELEMENTS(array)  (sizeof(array) / sizeof(array[0]))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))


typedef volatile int8_t* pTimer_id_t;
typedef int8_t timer_id_t;
typedef void (*timer_cb_t)(void);
typedef void (*timer_cb2_t)(void*);

#endif /* _TYPES_H_ */
