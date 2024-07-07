/* 323CA Dumitrascu Filip-Teodor */
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>

#define DIE(assertion, call_description)                                       \
    do {                                                                       \
        if (assertion) {                                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
            perror(call_description);                                          \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

/* Epoll */
#define EPOLL_TIMEOUT_INFINITE -1

/* Connection */ 
#define BUFLEN 4096
#define PORT 8080
#define HOST "34.246.184.49" 
#define HOST_PORT "34.246.184.49:8080"

/* Parse send and response message */
#define INDENT_LVL 4
#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

/* Stdout messages */
#define INVALID_COMMAND "ERROR: Invalid command!\n"
#define INVALID_SPACES "ERROR: Please enter input without spaces!\n"
#define INVALID_DIGITS "ERROR: Please enter input only with digits!\n"
#define INVALID_EMPTY "ERROR: Please enter some input!\n"

#define USER_ALREADY_HAS_ACCESS "ERROR: You already have access!\n"
#define USER_ALREADY_LOGGED "ERROR: You are already logged in!\n"
#define USER_NO_LOGIN "ERROR: You need to login first!\n"
#define USER_NO_ACCESS "ERROR: You need access to the library first!\n"

#define USER_CREATED "SUCCESS: User registered!\n"
#define USER_LOGGEDIN "SUCCESS: User logged in!\n"
#define USER_ACCESS "SUCCESS: Access granted to the library!\n"
#define USER_BOOK "SUCCESS: These are the books:\n"
#define USER_ADD "SUCCESS: Book added!\n"
#define USER_DEL "SUCCESS: Book deleted!\n"
#define USER_LOGOUT "SUCCESS: Logged out!\n"

/* Input types */
typedef enum {
    NUMERIC,
    NO_SPACES,
    NO_EMPTY
} input_t;

/* Command types */
typedef enum {
    REGISTER,
    LOGIN,
    ENTER_LIB,
    GET_ALL,
    GET_ONE,
    ADD,
    DELETE,
    LOGOUT,
    EXIT
} command_t;

#endif
