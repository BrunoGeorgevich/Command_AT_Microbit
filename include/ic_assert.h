#ifndef IC_ASSERT_H
#define IC_ASSERT_H


#include <misc/printk.h>

#ifdef ASSERT_ENABLED
#define __ASSERT_POST     \
    for (;;) {            \
        /* spin thread */ \
        k_cpu_idle();     \
    }

#define ASSERT(test, fmt, ...) \
    do {                                                  \
        if (!(test)) {                                    \
            printk("ASSERTION FAIL [%s] @ %s:%d:\n\t",    \
                   #test,                                 \
                   __FILE__,                              \
                   __LINE__);                             \
            printk(fmt, ##__VA_ARGS__);                   \
            __ASSERT_POST                                 \
        }                                                 \
    } while (0)
#else
#define __ASSERT_POST
#define ASSERT(test, fmt, ...)
#endif // ASSERT ENABLED

#endif // IC_ASSERT_H
