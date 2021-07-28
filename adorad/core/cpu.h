/*
          _____         _____           _____  
    /\   |  __ \ / __ \|  __ \    /\   |  __ \
   /  \  | |  | | |  | | |__)|   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /  / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \ / ____ \| |__| | https://github.com/adorad/adorad/
/_/_ __\_\_____/_\____/|_|__\_/_/_ __\_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef CSTL_CPU_H
#define CSTL_CPU_H

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
    #ifndef CSTL_CPU_X86
         #define CSTL_CPU_X86 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 64
    #endif

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
    #ifndef CSTL_CPU_PPC
        #define CSTL_CPU_PPC 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 128
    #endif

#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #ifndef CSTL_CPU_ARM
        #define CSTL_CPU_ARM 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 64
    #endif

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
    #ifndef CSTL_CPU_MIPS
        #define CSTL_CPU_MIPS 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 64
    #endif

#else
    #error Unknown CPU Type
#endif // CSTL_CPU_...


#endif // CSTL_CPU_H