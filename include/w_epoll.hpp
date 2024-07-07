/* 323CA Dumitrascu Filip-Teodor */
#ifndef __W_EPOLL_H__
#define __W_EPOLL_H__

#include <sys/epoll.h>

#include "utils.hpp"

/**
 * @brief Creates epoll fd without size limit
 * 
 * @return the fd 
 */
static inline int w_epoll_create(void)
{
    int rc = epoll_create1(0);
    DIE(rc < 0, "epoll_create1 failed\n");

	return rc;
}

/**
 * @brief Adds the fd to epoll as EPOLLIN
 * 
 * @param epollfd the epoll fd
 * @param fd the fd
 */
static inline void w_epoll_add_fd_in(int epoll_fd, int fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;

    int rc = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    DIE(rc < 0, "epoll_ctl failed\n");
}

/**
 * @brief Removes the fd from epoll as EPOLLIN
 * 
 * @param epoll_fd the epoll fd
 * @param fd the fd
 */
static inline void w_epoll_remove_fd(int epoll_fd, int fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;

	int rc = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
    DIE(rc < 0, "epoll_ctl failed\n");
}

/**
 * @brief Thread waits for an I/O event to occur on a set of descriptors
 * 
 * @param epoll_fd the epoll fd
 * @param rev the memory area marked with events occurred
 */
static inline void w_epoll_wait_infinite(int epoll_fd, struct epoll_event *rev)
{
	int rc = epoll_wait(epoll_fd, rev, 1, EPOLL_TIMEOUT_INFINITE);
    DIE(rc < 0, "epoll_wait failed\n");
}


#endif /* EPOLL_H_ */
