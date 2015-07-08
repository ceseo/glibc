/* Check __ppc_get_hwcap() and __ppc_set_hwcap() functionality
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

static int
do_test (void)
{
  uint32_t h1, h2, h3, hwcap, hwcap2;

  /* Testing the get function and if the data is correctly initialized by
     TLS_INIT.  */

  h1 = __ppc_get_hwcap ();
  h2 = __ppc_get_hwcap2 ();
  hwcap = getauxval(AT_HWCAP);
  hwcap2 = getauxval(AT_HWCAP2);

  /* hwcap contains only the latest supported ISA, the code checks which is
     and fills the previous supported ones. This is necessary because the
     same is done in tls.h when setting the values to the TCB.   */

  if (hwcap & PPC_FEATURE_ARCH_2_06)
    hwcap |= PPC_FEATURE_ARCH_2_05 | PPC_FEATURE_POWER5_PLUS |
	     PPC_FEATURE_POWER5 | PPC_FEATURE_POWER4;
  else if (hwcap & PPC_FEATURE_ARCH_2_05)
    hwcap |= PPC_FEATURE_POWER5_PLUS | PPC_FEATURE_POWER5 | PPC_FEATURE_POWER4;
  else if (hwcap & PPC_FEATURE_POWER5_PLUS)
    hwcap |= PPC_FEATURE_POWER5 | PPC_FEATURE_POWER4;
  else if (hwcap & PPC_FEATURE_POWER5)
    hwcap |= PPC_FEATURE_POWER4;

  if ( h1 != hwcap )
    {
      printf("Fail: __ppc_get_hwcap() - HWCAP is %x. Should be %x\n", h1, hwcap);
      return 1;
    }

  if ( h2 != hwcap2 )
    {
      printf("Fail: __ppc_get_hwcap2() - HWCAP2 is %x. Should be %x\n", h2, hwcap2);
      return 1;
    }

  /* Testing the set functions.  */

  h3 = 0xDEADBEEF;

  __ppc_set_hwcap(h3);
  __ppc_set_hwcap2(h3);

  hwcap = __ppc_get_hwcap();
  hwcap2 = __ppc_get_hwcap2();

  if ( h3 != hwcap )
    {
      printf("Fail: __ppc_set_hwcap() - HWCAP is %x. Should be %x\n", h3, hwcap);
      return 1;
    }

  if ( h3 != hwcap2 )
    {
      printf("Fail: __ppc_set_hwcap2() - HWCAP2 is %x. Should be %x\n", h3, hwcap2);
      return 1;
    }


  printf("Pass: HWCAP and HWCAP2 are correctly set in the TCB.\n");

  return 0;

}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"

