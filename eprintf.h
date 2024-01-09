// Provided by Dr. Bob Lewis for CPT_S 360 at WSUTC

#ifndef _INCLUDED_EPRINTF
#include <stdio.h>  // for fprintf() and stderr

/*
 * eprintf() is just like printf(), but outputs to stderr instead of
 * stdout.
 *
 * To emphasize the printf() similarity, we violate the usual practice
 * of putting macros in upper case.
 */

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define _INCLUDED_EPRINTF
#endif