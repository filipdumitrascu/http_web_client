/* 323CA Dumitrascu Filip-Teodor */
#include "helpers.hpp"

int main(int argc, char *argv[])
{
    bool exit = false;
    command_t cmd_type;
    
    string_t cookie;
    string_t token;

    int epoll_fd = w_epoll_create();
    int sock_fd;

    w_epoll_add_fd_in(epoll_fd, STDIN_FILENO);

    while (true) {
        struct epoll_event rev;
        w_epoll_wait_infinite(epoll_fd, &rev);

        /* Skip no EPOLLIN event */
        if (!(rev.events & EPOLLIN)) {
            continue;
        }

        if (rev.data.fd == STDIN_FILENO) {
            handle_stdin_command(epoll_fd, sock_fd, cmd_type, cookie, token, exit);

            if (exit) {
                break;
            }
            continue;
        }

        if (rev.data.fd == sock_fd) {
            handle_server_response(epoll_fd, sock_fd, cmd_type, cookie, token);
        }
    }

    /* CLose the epoll fd */
    w_epoll_remove_fd(epoll_fd, STDIN_FILENO);
    close(epoll_fd);
    
    return 0;
}
