
/*
FUNCTION
	<<isspace>>---whitespace character predicate

INDEX
	isspace

ANSI_SYNOPSIS
	#include <ctype.h>
	int isspace(int <[c]>);

TRAD_SYNOPSIS
	#include <ctype.h>
	int isspace(<[c]>);

DESCRIPTION
<<isspace>> is a macro which classifies ASCII integer values by table
lookup.  It is a predicate returning non-zero for whitespace
characters, and 0 for other characters.  It is defined only when <<isascii>>(<[c]>) is true or <[c]> is EOF.

You can use a compiled subroutine instead of the macro definition by
undefining the macro using `<<#undef isspace>>'.

RETURNS
<<isspace>> returns non-zero if <[c]> is a space, tab, carriage return, new
line, vertical tab, or formfeed (<<0x09>>--<<0x0D>>, <<0x20>>).

PORTABILITY
<<isspace>> is ANSI C.

No supporting OS subroutines are required.
*/
//#include <_ansi.h>
#include <ctype.h>

#undef isspace
int isspace(int c)
{
int status = 0;

	if (c==0x0D || //CR
		c==0x0C || //FF
 		c==0x09	|| //HT or TAB 
		c==0x0A	|| //NL or LF 
		c==0x0B)   //VT  
	{
		status = 1;
	}

	return status;
}
