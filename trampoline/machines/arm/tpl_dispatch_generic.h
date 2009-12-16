/**
 * @file tpl_dispatch_generic.h
 *
 * @section descr File description
 *
 * Common definitions for platform dependant dispatch system (system calls)
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation and ARM port
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date: 2009-02-10 16:48:10 +0100 (Mar, 10 fév 2009) $
 * $Rev: 568 $
 * $Author: ilias $
 * $URL: https://trampoline.rts-software.org/svn/trunk/machines/arm/tpl_dispatch_generic.h $
 */
#ifndef TPL_DISPATCH_GENERIC_H
#define TPL_DISPATCH_GENERIC_H

/**
 * @def CT_ASM
 *
 * CT_ASM expand to the appropriate keyword to qualify an assembly language
 * function. If the compiler uses this kind of construct, define it here
 * If it does not, define CT_ASM with no value and put the ad hoc keywords
 * in TPL_SYSCALL
 *
 * @see #ASM
 * @see #TPL_SYSCALL
 */
#define TC_ASM

/**
 * @def TPL_SYSCALL
 *
 * Macro used by API to invoke OS services via
 * a system call (software interrupt).
 *
 * On ARM architecture, this macro is defined in a way r0-r3 should be
 * considered by the compiler as altered by the macro.
 *
 * @see #tpl_sc_handler
 */
#define TPL_SYSCALL(os_service_id)  \
        __asm__ __volatile__ ( \
             "mov r0, %[service_number];" \
             "swi %[service_number]" \
             : \
             : [service_number] "n" (os_service_id) \
             : "r0", "r1", "r2", "r3" /* this is like a function call, */ \
                                      /* all caller-saved (EABI) registers */ \
                                      /* are declared clobbered */ \
     );
    
#endif /* TPL_DISPATCH_GENERIC_H */

