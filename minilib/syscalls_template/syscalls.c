/*
 * System call stub for minilib C library
 * GPL licensed code (S. Di Mercurio and P. Acco)
 * _________________
 * USAGE 
 *  copy this minilib/syscalls_template/syscalls.c in the project dir as syscalls.c
 * 	add path to minilib/include in your compiler include path
 *  add #include "missing_defs.h" in order to have struct FILE declared
 *  modify your copy of syscalls.c to have it work as you like (current function implementations are just stubs)
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "missing_defs.h"

#ifdef __GNUC__
#if !defined (__AVR__)
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <reent.h>

	struct _reent *_impure_ptr __ATTRIBUTE_IMPURE_PTR__;
#else
	//#include <sys/types.h>
	#define ENOMEM -2
	#define EAGAIN -3
	#define EINVAL -4
	#define EMLINK -5
	#define ENOENT -6
	#define ECHILD -7

#endif /* !defined __AVR__ */
#else
	#define ENOMEM -2
	#define EAGAIN -3
	#define EINVAL -4
	#define EMLINK -5
	#define ENOENT -6
	#define ECHILD -7
	
	#define S_IFCHR 0x12
	#define caddr_t int	
	
	struct stat
	{
		int st_mode;	
	}; 

	struct tms
	{
		int a;
	};
#endif /* __GNUC__ */

#undef errno
int errno;
	   
/*
 * _exit
 *   Exit a program without cleaning up files. If your system doesn't provide this, it is best to avoid linking with subroutines that require it (exit, system).
 */
void _exit(int exit_code)
{
	/* Nothing to do */
	for (;;);
}

/*
 * _kill
 * Send a signal. Minimal implementation:
 */
int _kill(int pid, int sig) 
{
	errno = EINVAL;
	return -1;
}

/*
 * _open
 * Open a file. Minimal implementation:
 */
int _open(const char *name, int flags, int mode) 
{
	return -1;
}

/*
 * _read
 * Read from a file. 
 * 	file : file identifier (indicate on which channel/peripheral read data)
 * 	ptr : pointer to an array of char (provided by caller) in which store data
 *	len : amount of data to read
 */
int _read(int file, char *ptr, int len) 
{
char *loc_ptr=ptr;

	while (loc_ptr-ptr < len) 
	{
  		// insert your code here / call appropriate functions
		loc_ptr++;
	}	
	
	return len;
}

/*
 *	_write
 *  Write to a file. 
 * 	file : file identifier (indicate in which channel/peripheral write data)
 * 	ptr : pointer to an array of char (provided by caller) on which take data to write
 *	len : amount of data to write
 */
int _write(int file, char *ptr, int len) 
{
char *loc_ptr=ptr;

	while (loc_ptr-ptr < len) 
	{
  		// insert your code here / call appropriate functions
		loc_ptr++;
	}	
	
	return len;
}

/*
 * _malloc
 * Allocate memory. Minimal implementation:
 */
void *_malloc(size_t nbytes) 
{
	return NULL;
}

/*
 * _free
 * Release memory. Minimal implementation:
 */
void _free(void *aptr) 
{
}

