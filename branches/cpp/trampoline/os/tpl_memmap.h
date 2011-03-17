/**
 * @file tpl_memmap.h
 *
 * @section desc File description
 *
 * Trampoline wrapper to memory mapping platform specific header
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date: 2009-12-18 17:43:30 +0100 (Ven 18 déc 2009) $
 * $Rev: 1020 $
 * $Author: jlb $
 * $URL: https://trampoline.rts-software.org/svn/trunk/os/tpl_memmap.h $
 */

#include "tpl_app_objects.h"

#if WITH_MEMMAP != YES && WITH_MEMMAP != NO
  #error "Wrong configuration of the OS, WITH_MEMMAP should be YES or NO"
#endif

#if WITH_MEMMAP == YES
  #include "MemMap.h"
#endif

/* End of file tpl_memmap.h */
