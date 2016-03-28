#ifndef __NETPROGRAM_H__
#define __NETPROGRAM_H__

#include <sys/types.h>	/* basic system data types */
#include <sys/socket.h>	/* basic socket definitions */
#include <netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>	/* inet(3) functions */
#include <errno.h>
#include <fcntl.h>		/* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>	/* for S_xxx file mode constants */
#include <sys/uio.h>		/* for iovec{} and readv/writev */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>		/* for Unix domain sockets */

#include "err.h"

#include "wrapsock.h"

#define LISTENQ 1024   /* 2nd argument of Listen */

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

#endif
