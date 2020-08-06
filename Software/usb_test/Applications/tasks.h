
#ifndef __TASKS_H_
#define __TASKS_H_
#include "rtl.h"

/* RTL no timeout value */
#define NO_TIMEOUT  (0xffff)

/* Task configuration (Priority, stacks etc.) */
#define LOWEST_PRIORITY             (1)     /* Priority 0 is reserved for the RTX idle task */
#define HIGHEST_PRIORITY            (254)   /* Priority 255 is reserved by RTX */

#endif /* __TASKS_H_ */
