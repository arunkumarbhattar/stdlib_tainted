/* Copyright (C) 1999-2022 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <libc-diag.h>

static int errors = 0;

static void
merror (const char *msg)
{
  ++errors;
  printf ("Error: %s\n", msg);
}

static int
do_test (void)
{
  void *p, *q;
  int save;

  errno = 0;

  DIAG_PUSH_NEEDS_COMMENT;
#if __GNUC_PREREQ (7, 0)
  /* GCC 7 warns about too-large allocations; here we want to test
     that they fail.  */
  DIAG_IGNORE_NEEDS_COMMENT (7, "-Walloc-size-larger-than=");
#endif
  p = tmalloc (-1);
  DIAG_POP_NEEDS_COMMENT;
  save = errno;

  if (p != NULL)
    merror ("tmalloc (-1) succeeded.");

  if (p == NULL && save != ENOMEM)
    merror ("errno is not set correctly");

  p = tmalloc (10);
  if (p == NULL)
    merror ("tmalloc (10) failed.");
  /* realloc (p, 0) == free (p).  */
  p = trealloc (p, 0);
  if (p != NULL)
    merror ("realloc (p, 0) failed.");

  p = tmalloc (0);
  if (p == NULL)
    merror ("tmalloc (0) failed.");

  p = trealloc (p, 0);
  if (p != NULL)
    merror ("realloc (p, 0) failed.");

  p = tmalloc (513 * 1024);
  if (p == NULL)
    merror ("tmalloc (513K) failed.");

  DIAG_PUSH_NEEDS_COMMENT;
#if __GNUC_PREREQ (7, 0)
  /* GCC 7 warns about too-large allocations; here we want to test
     that they fail.  */
  DIAG_IGNORE_NEEDS_COMMENT (7, "-Walloc-size-larger-than=");
#endif
  q = tmalloc (-512 * 1024);
  DIAG_POP_NEEDS_COMMENT;
  if (q != NULL)
    merror ("tmalloc (-512K) succeeded.");

  free (p);

  return errors != 0;
}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
