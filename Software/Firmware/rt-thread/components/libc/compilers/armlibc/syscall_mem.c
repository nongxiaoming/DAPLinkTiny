/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2014-08-03     bernard      Add file header
 * 2021-11-13     Meco Man     implement no-heap warning
 */

#include <rtthread.h>
#include <stddef.h>

#ifndef RT_USING_HEAP
#define DBG_TAG    "armlibc.syscall.mem"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#define _NO_HEAP_ERROR()  do{LOG_E("Please enable RT_USING_HEAP");\
                             RT_ASSERT(0);\
                            }while(0)
#endif /* RT_USING_HEAP */

#ifdef __CC_ARM
    /* avoid the heap and heap-using library functions supplied by arm */
    #pragma import(__use_no_heap)
#endif /* __CC_ARM */

#if defined (__clang__) && defined (RT_USING_CPLUSPLUS)
    #define RT_USING_POSIX_MEMALIGN
#endif

#include <sys/errno.h>

void *malloc(size_t n)
{
#ifdef RT_USING_HEAP
    #ifndef RT_USING_POSIX_MEMALIGN
    return rt_malloc(n);
    #else
    return rt_malloc_align(n, RT_ALIGN_SIZE);
    #endif
#else
    _NO_HEAP_ERROR();
    return RT_NULL;
#endif
}
RTM_EXPORT(malloc);

void *realloc(void *rmem, size_t newsize)
{
#ifdef RT_USING_HEAP
    return rt_realloc(rmem, newsize);
#else
    _NO_HEAP_ERROR();
    return RT_NULL;
#endif
}
RTM_EXPORT(realloc);

void *calloc(size_t nelem, size_t elsize)
{
#ifdef RT_USING_HEAP
    return rt_calloc(nelem, elsize);
#else
    _NO_HEAP_ERROR();
    return RT_NULL;
#endif
}
RTM_EXPORT(calloc);

void free(void *rmem)
{
#ifdef RT_USING_HEAP
    #ifndef RT_USING_POSIX_MEMALIGN
    rt_free(rmem);
    #else
    rt_free_align(rmem);
    #endif
#else
    _NO_HEAP_ERROR();
#endif
}
RTM_EXPORT(free);

#if defined RT_USING_POSIX_MEMALIGN
int posix_memalign(void **ptr, size_t align, size_t size)
{
    if ((align & (align - 1)) != 0)
    {
        return -EINVAL;
    }
    
    if(align != RT_ALIGN(align, sizeof(void *)))
    {
        return -EINVAL;
    }
    
    *ptr = rt_malloc_align(size, align);
    
    if(*ptr == RT_NULL)
    {
        return -ENOMEM;
    }
    return 0;
}
#endif
