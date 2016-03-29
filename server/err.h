#ifndef __ERR_H__
#define __ERR_H__

#include <errno.h>
#include <stdio.h>

static inline void err_sys(char *str) {
	perror(str);
	exit(1);
}

#endif
