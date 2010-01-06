/*
 * Missing definition for minilib C library
 * GPL licensed code (S. Di Mercurio and P. Acco)
 */

#ifndef __MISSING_DEFS__
#define __MISSING_DEFS__

/* Definition of FILE struct */
struct __FILE
{
	int _file; // only field required for our lib C */
};

#endif /* __MISSING_DEFS__ */
