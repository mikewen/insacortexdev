#include "stddef.h"
#include "stdarg.h"
#include "stdlib.h"

#if defined __MINILIB__
#error this file is obsolete: use minilib instead
#endif /* __MINILIB__ */

int sscanf(char *s, const char *fmt, ...)
{
	char *s0;
	va_list ap;
	long L, *Lp;
	int i, *ip, rc = 0;

	va_start(ap, fmt);
	for( ; ; ) {
		for(;;) {
			switch(i = *(unsigned char *)fmt++) {
				case 0:
					goto done;
				case '%':
					break;
				default:
					if (i <= ' ') {
						while(*s <= ' ')
							if (!*s++)
								return rc;
						}
					else if (*s++ != i)
						return rc;
					continue;
				}
			break;
			}
		switch(*fmt++) {
			case 'l':
//				if (*fmt != 'd')
//					bad(fmt);
				fmt++;
				Lp = va_arg(ap, long*);
				L = strtol(s0 = s, &s, 10);
				if (s > s0) {
					rc++;
					*Lp = L;
					continue;
					}
				return rc;
			case 'd':
				ip = va_arg(ap, int*);
				L = strtol(s0 = s, &s, 10);
				if (s > s0) {
					rc++;
					*ip = (int)L;
					continue;
					}
				return rc;
			default:
				//bad(fmt);
				rc=0;
			}
		}
 done:
	return rc;
}
