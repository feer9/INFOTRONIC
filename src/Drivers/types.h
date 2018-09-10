#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef void (*callback_t)(void);

#endif /* _TYPES_H_ */
