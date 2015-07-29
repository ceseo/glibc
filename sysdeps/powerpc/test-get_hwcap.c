/* Check __ppc_get_hwcap(), __ppc_get_hwcap2() and __ppc_get_at_plaftorm()
   functionality.
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

/* Tests if the hwcap and hwcap2 data is stored in the TCB.  */

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

#include <sys/auxv.h>
#include <sys/platform/ppc.h>

#include <dl-procinfo.h>

const char _dl_powerpc_platforms[14][12]
= {
    [PPC_PLATFORM_POWER4] = "power4",
    [PPC_PLATFORM_PPC970] = "ppc970",
    [PPC_PLATFORM_POWER5] = "power5",
    [PPC_PLATFORM_POWER5_PLUS] = "power5+",
    [PPC_PLATFORM_POWER6] = "power6",
    [PPC_PLATFORM_CELL_BE] = "ppc-cell-be",
    [PPC_PLATFORM_POWER6X] = "power6x",
    [PPC_PLATFORM_POWER7] = "power7",
    [PPC_PLATFORM_PPCA2] = "ppca2",
    [PPC_PLATFORM_PPC405] = "ppc405",
    [PPC_PLATFORM_PPC440] = "ppc440",
    [PPC_PLATFORM_PPC464] = "ppc464",
    [PPC_PLATFORM_PPC476] = "ppc476",
    [PPC_PLATFORM_POWER8] = "power8",
  };

static int
do_test (void)
{
  uint32_t a1, at_platform;
  uint64_t h1, h2, hwcap, hwcap2;
  const char *at_platform_string;

  /* Testing the get function and if the data is correctly initialized by
     TLS_INIT.  */

  h1 = __ppc_get_hwcap ();
  hwcap = getauxval(AT_HWCAP);
  hwcap2 = getauxval(AT_HWCAP2);

  /* hwcap contains only the latest supported ISA, the code checks which is
     and fills the previous supported ones. This is necessary because the
     same is done in hwcapinfo.c when setting the values that are copied to
     the TCB.   */

  if (hwcap & PPC_FEATURE_ARCH_2_06)
    hwcap |= PPC_FEATURE_ARCH_2_05 | PPC_FEATURE_POWER5_PLUS |
	     PPC_FEATURE_POWER5 | PPC_FEATURE_POWER4;
  else if (hwcap & PPC_FEATURE_ARCH_2_05)
    hwcap |= PPC_FEATURE_POWER5_PLUS | PPC_FEATURE_POWER5 | PPC_FEATURE_POWER4;
  else if (hwcap & PPC_FEATURE_POWER5_PLUS)
    hwcap |= PPC_FEATURE_POWER5 | PPC_FEATURE_POWER4;
  else if (hwcap & PPC_FEATURE_POWER5)
    hwcap |= PPC_FEATURE_POWER4;

  h2 = (hwcap << 32) + hwcap2;

  if ( h1 != h2 )
    {
      printf("Fail: __ppc_get_hwcap() - HWCAP is %lx. Should be %lx\n", h1, h2);
      return 1;
    }

  at_platform_string = (const char *) getauxval (AT_PLATFORM);
  a1 = __ppc_get_at_platform ();
  at_platform = _dl_string_platform (at_platform_string);

  if ( a1 != at_platform )
    {
      printf("Fail: __ppc_get_at_platform() - AT_PLATFORM is %x. Should be %x\n", a1, at_platform);
      return 1;
    }

  printf("Pass: HWCAP, HWCAP2 and AT_PLATFORM are correctly set in the TCB.\n");

  return 0;

}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"

