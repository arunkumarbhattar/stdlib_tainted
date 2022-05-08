#ifndef _MALLOC_H

#include <tmalloc/tmalloc.h>

# ifndef _ISOMAC
#  include <rtld-malloc.h>

struct malloc_state;
typedef struct malloc_state *mstate;

# endif /* !_ISOMAC */

#endif
