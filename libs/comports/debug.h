#ifndef __LIB_DEBUG_H
#define __LIB_DEBUG_H
#include <stdio.h>
#include <stdlib.h>
/* GCC lets us add "attributes" to functions, function
   parameters, etc. to indicate their properties.
   See the GCC manual for details. */
/* Halts the OS, printing the source file name, line number, and
   function name, plus a user-specific message. */
#define PANIC(line, argument) printf(line, argument)

#endif



/* This is outside the header guard so that debug.h may be
   included multiple times with different settings of NDEBUG. */
#undef ASSERT

#ifndef NDEBUG
#define ASSERT(CONDITION)                                       \
        if (CONDITION) { /*do notthing!*/} else {                               \
                PANIC ("assertion `%s' failed.", #CONDITION);   \
                exit(-1);               \
        }
        
#define PRINT(POSITION, BYTE) \
        do{                     \
                printf("%s %p\n", #POSITION, #BYTE); \
        }while(0);
#else
#define ASSERT(CONDITION) ((void) 0)
#define PRINT(POSITION, BYTE) ((void) 0)
#endif /* lib/debug.h */

