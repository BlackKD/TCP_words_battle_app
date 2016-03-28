#ifndef __WRAPSOCK_H__
#define __WRAPSOCK_H__

int Socket(int family, int type, int protocol);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
void Close(int fd);

#endif
