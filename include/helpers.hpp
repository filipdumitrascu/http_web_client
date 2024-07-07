/* 323CA Dumitrascu Filip-Teodor */
#ifndef _HELPERS_
#define _HELPERS_

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#include "common.hpp"
#include "requests.hpp"
#include "utils.hpp"
#include "w_epoll.hpp"
#include "json.hpp"

/**
 * @brief Handles the command entered on keyboard 
 * 
 * @param epoll_fd the epoll fd
 * @param sock_fd the server fd
 * @param cmd_type the command type
 * @param cookie the cookie
 * @param token the token
 * @param exit the exit boolean
 */
void handle_stdin_command(int epoll_fd, int &sock_fd, command_t &cmd_t,
                          string_t &cookie, string_t &token, bool &exit);

/**
 * @brief Handles the server response 
 * 
 * @param epoll_fd the epoll fd
 * @param sock_fd the server fd
 * @param cmd_type the command type
 * @param cookie the cookie
 * @param token the token
 */
void handle_server_response(int epoll_fd, int &sock_fd, command_t cmd_type,
                            string_t &cookie, string_t &token);

/**
 * @brief Handle a post request
 * 
 * @param epoll_fd the epoll fd
 * @param sock_fd the server fd
 * @param cmd_type the command type
 * @param cookie the cookie
 * @param token the token
 */
void handle_post_req(int epoll_fd, int &sock_fd, command_t cmd_type,
                     const string_t &cookie, const string_t &token);

/**
 * @brief Handle the get/delete request
 * 
 * @param epoll_fd the epoll fd
 * @param sock_fd the server fd
 * @param cmd_type the command type
 * @param cookie the cookie
 * @param token the token
 */
void handle_get_delete_req(int epoll_fd, int &sock_fd, command_t cmd_type,
                           const string_t &cookie, const string_t &token);

/**
 * @brief Opens a connection between the server and the client
 * 
 * @param host_ip the host ip
 * @param port_num the port number
 * @param ip_type the ip type
 * @param socket_type  the socket type
 * @param flag flags
 * @param epoll_fd the epoll file descriptor 
 * 
 * @return the fd
 */
int open_connection(const string_t &host_ip, int port_num, int ip_type,
                    int socket_type, int flag, int epoll_fd);

/**
 * @brief Get the input object
 * 
 * @return string_t 
 */
string_t get_input(void);

/**
 * @brief Get the id object
 * 
 * @return string_t 
 */
string_t get_id(void);

/**
 * @brief Get the user object
 * 
 * @return nlohmann::json 
 */
nlohmann::json get_user(void);

/**
 * @brief Get the book object
 * 
 * @return nlohmann::json 
 */
nlohmann::json get_book(void);

/**
 * @brief Checks if the input is valid
 * 
 * @param input the input 
 * @param type the valid input type
 * @return true 
 * @return false 
 */
bool is_input_valid(string_t &input, input_t type);

/**
 * @brief Parses the response from the received buffer
 * 
 * @param buf the buffer
 * @return string_t 
 */
string_t parse_response(string_t &buf);

/**
 * @brief Parses the cookie from the received buffer
 * 
 * @param buf the buffer
 * @return string_t 
 */
string_t parse_cookie(string_t &buf);

/**
 * @brief Parses the token from the received buffer
 * 
 * @param buf the buffer
 * @return string_t 
 */
string_t parse_token(string_t &buf);

/**
 * @brief Parses the error from the response 
 * 
 * @param response the response
 * @return string_t 
 */
string_t parse_error(string_t &response);

#endif
