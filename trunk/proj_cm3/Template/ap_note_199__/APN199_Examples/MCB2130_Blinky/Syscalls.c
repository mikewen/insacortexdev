/* System Calls Remapping */

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <string.h>
#include <errno.h>

#define putchar sendchar
#define getchar getkey

extern int putchar (int ch);
extern int getchar (void);

int _read (int fd, char * ptr, int len) {
  char c;
  int  i;

  for (i = 0; i < len; i++) {
    c = getchar();
    if (c == 0x0D) break;
    *ptr++ = c;
    putchar(c);
  }
  return (len - i);
}

int _lseek (int fd, int ptr, int dir) {
  return (0);
}

int _write (int fd, char * ptr, int len) {
  int i;

  for (i = 0; i < len; i++) putchar (*ptr++);
  return (i);
}

int _open (const char * path, int flags, ...) {
  return (-1);
}

int _close (int fd) {
  return (-1);
}

int _kill (int n, int m) {
  errno = EINVAL;
  return(-1);
}

int __attribute__((weak)) _getpid (int n __attribute__ ((unused))) {
  return (1);
}

extern int  __cs3_heap_start;
extern int  __cs3_heap_end;

caddr_t _sbrk (int incr) {
  static char * heap;
         char * prev_heap;

  if (heap == NULL) {
    heap = (char *)&__cs3_heap_start;
  }
  
  prev_heap = heap;

  if ((heap + incr) > (char *)&__cs3_heap_end) {
    errno = ENOMEM;
    return (caddr_t) -1;
  }
  
  heap += incr;

  return (caddr_t) prev_heap;
}

int __attribute__((weak)) _fstat (int fd, struct stat * st) {
  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFCHR;
  return (0);
}

int __attribute__((weak))_link (void) {
  errno = ENOSYS;
  return (-1);
}

int _unlink (const char *path) {
  errno = ENOENT;
  return (-1);
}

int isatty (int fd) {
  return (1);
}

#ifdef _USE_TIME

int _gettimeofday (struct timeval * tp, void * tzvp) {
  struct timezone *tzp = tzvp;

  if (tp) {
    /* Ask the host for the seconds since January 1, 1970 */
    tp->tv_sec  = 0; /* not implemented */
    tp->tv_usec = 0;
  }

  /* Return fixed data for the timezone  */
  if (tzp) {
    tzp->tz_minuteswest = 0;
    tzp->tz_dsttime = 0;
  }

  return (0);
}

extern long timeval;

/* Return a clock that ticks at 100Hz.  */
clock_t _times (struct tms * tp) {

  if (tp) {
    tp->tms_utime  = timeval;  /* user time */
    tp->tms_stime  = 0;        /* system time */
    tp->tms_cutime = 0;        /* user time, children */
    tp->tms_cstime = 0;        /* system time, children */
  }
  
  return timeval;
};

#endif
