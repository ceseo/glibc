/* powerpc hwcap info.
   Copyright (C) 2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <unistd.h>
#include <shlib-compat.h>
#include <dl-procinfo.h>
#include <ldsodefs.h>

uint32_t __tcb_hwcap __attribute__ ((visibility ("hidden")));
uint32_t __tcb_hwcap2 __attribute__ ((visibility ("hidden")));
uint32_t __tcb_platform __attribute__ ((visibility ("hidden")));
uint32_t __tcb_hwcap_init __attribute__ ((visibility ("hidden")));

void
__init_hwcapinfo (void)
{
  /* Find out what kind of processor.  */
  __tcb_platform = _dl_string_platform (GLRO(dl_platform));

  /* Find out the capabilities of processor.  */
  __tcb_hwcap = GLRO(dl_hwcap);
  __tcb_hwcap2 = GLRO(dl_hwcap2);

  /* hwcap contains only the latest supported ISA, the code checks which is
     and fills the previous supported ones.  */

  if (__tcb_hwcap & PPC_FEATURE_ARCH_2_06)	      \
      __tcb_hwcap |= PPC_FEATURE_ARCH_2_05 |	      \
		  PPC_FEATURE_POWER5_PLUS |	      \
		  PPC_FEATURE_POWER5 |		      \
		  PPC_FEATURE_POWER4;		      \
    else if (__tcb_hwcap & PPC_FEATURE_ARCH_2_05)     \
      __tcb_hwcap |= PPC_FEATURE_POWER5_PLUS |	      \
             PPC_FEATURE_POWER5 |		      \
             PPC_FEATURE_POWER4;		      \
    else if (__tcb_hwcap & PPC_FEATURE_POWER5_PLUS)   \
      __tcb_hwcap |= PPC_FEATURE_POWER5 |	      \
             PPC_FEATURE_POWER4;		      \
    else if (__tcb_hwcap & PPC_FEATURE_POWER5)	      \
      __tcb_hwcap |= PPC_FEATURE_POWER4;	      \

  __tcb_hwcap_init = 1;

}
#if IS_IN (libc)
versioned_symbol (libc, __init_hwcapinfo, init_hwcapinfo, GLIBC_2_23);
#endif

