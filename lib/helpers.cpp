/* 323CA Dumitrascu Filip-Teodor */
#include "helpers.hpp"

void handle_stdin_command(int epoll_fd, int &sock_fd, command_t &cmd_type,
                          string_t &cookie, string_t &token, bool &exit)
{
    string_t command;
    command = get_input();

    if (!is_input_valid(command, NO_SPACES)) {
        std::cout << INVALID_SPACES;
        return;
    }

    /* If the input is a valid command handles it */
    if (!command.compare("register")) {
        cmd_type = REGISTER;
        handle_post_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }

    if (!command.compare("login")) {
        cmd_type = LOGIN;
        handle_post_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }

    if (!command.compare("enter_library")) {
        cmd_type = ENTER_LIB;
        handle_get_delete_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }

    if (!command.compare("get_books")) {
        cmd_type = GET_ALL;
        handle_get_delete_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }

    if (!command.compare("get_book")) {
        cmd_type = GET_ONE;
        handle_get_delete_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }

    if (!command.compare("add_book")) {
        cmd_type = ADD;
        handle_post_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }

    if (!command.compare("delete_book")) {
        cmd_type = DELETE;
        handle_get_delete_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }
    
    if (!command.compare("logout")) {
        cmd_type = LOGOUT;
        handle_get_delete_req(epoll_fd, sock_fd, cmd_type, cookie, token);
        return;
    }

    if (!command.compare("exit")) {
        exit = true;
        return;
    }

    std::cout << INVALID_COMMAND;
}

void handle_server_response(int epoll_fd, int &sock_fd, command_t cmd_type,
                            string_t &cookie, string_t &token)
{
    string_t buf;
    buf.resize(BUFLEN);

    /* Receive the server response */
    recv_all(sock_fd, buf.data(), BUFLEN);
    
    /* Parse the status code and the content */
    int code = std::stoi(buf.substr(9, 3));
    string_t response = parse_response(buf);

    /* If the code in an error one prints an error message
    otherwise prints a success message */
    if (code / 100 == 4 || code / 100 == 5) {
        std::cout << "ERROR: " + parse_error(response);
    
    } else {
        switch (cmd_type) {
            case REGISTER:
                std::cout << USER_CREATED; 
                break;

            case LOGIN:
                std::cout << USER_LOGGEDIN;
                cookie = parse_cookie(buf);
                break;

            case ENTER_LIB:
                std::cout << USER_ACCESS;
                token = parse_token(buf);
                break;

            case GET_ONE:
            case GET_ALL:
                std::cout << USER_BOOK + response;
                break;

            case ADD:
                std::cout << USER_ADD;
                break;

            case DELETE:
                std::cout << USER_DEL;
                break;

            case LOGOUT:
                std::cout << USER_LOGOUT;
                cookie.clear();
                token.clear();
                break;

            default:
                std::cout << INVALID_COMMAND;
        }
    }

    /* Close the connection */
    w_epoll_remove_fd(epoll_fd, sock_fd);

    int rc = close(sock_fd);
    DIE(rc < 0, "close failed\n");
}

void handle_post_req(int epoll_fd, int &sock_fd, command_t cmd_type,
                     const string_t &cookie, const string_t &token)
{
    nlohmann::json object;

    /* If there is no access token, the user cannot add books */
    if (cmd_type == ADD && token.empty()) {
        std::cout << USER_NO_ACCESS;
        return;
    }

    /* If the user is already logged in, return */
    if (cmd_type == LOGIN && !cookie.empty()) {
        std::cout << USER_ALREADY_LOGGED;
        return;
    }

    string_t url_type;

    switch (cmd_type) {
        case REGISTER:
            object = get_user();
            url_type = "/api/v1/tema/auth/register";
            break;

        case LOGIN:
            object = get_user();
            url_type = "/api/v1/tema/auth/login";
            break;

        case ADD:
            object = get_book();
            url_type = "/api/v1/tema/library/books";
            break;

        default:
            break;
    }

    if (object.empty()) {
        return;
    }

    /* Serialize the json object into a string */
    string_t json_cnt = object.dump(INDENT_LVL);

    string_t message = post_request(HOST_PORT, url_type, "application/json",
                                    json_cnt, cookie, token);;

    /* Open the connection and send the hhtp requst to the server */
    sock_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0, epoll_fd);
    send_all(sock_fd, message.data(), message.size());
}

void handle_get_delete_req(int epoll_fd, int &sock_fd, command_t cmd_type,
                           const string_t &cookie, const string_t &token)
{
    /* If the user already has access, return */
    if (cmd_type == ENTER_LIB && !token.empty()) {
        std::cout << USER_ALREADY_HAS_ACCESS;
        return;
    }

    /* If the user is not logged in, it cannot logout or enter the library */
    if ((cmd_type == ENTER_LIB || cmd_type == LOGOUT) && cookie.empty()) {
        std::cout << USER_NO_LOGIN;
        return;
    }

    /* If there is no access token, the user cannot get books or delete one */
    if ((cmd_type == GET_ALL || cmd_type == GET_ONE || cmd_type == DELETE)
        && token.empty()) {
        std::cout << USER_NO_ACCESS;
        return;
    }

    string_t id;
    string_t url;
    string_t req_type;

    switch (cmd_type) {
        case ENTER_LIB :
            url = "/api/v1/tema/library/access";
            req_type = "GET";
            break;

        case GET_ALL :
            url = "/api/v1/tema/library/books";
            req_type = "GET";
            break;

        case GET_ONE :
            id = get_id();
            if (!id.compare("")) {
                return;
            }

            url = "/api/v1/tema/library/books/" + id;
            req_type = "GET";
            break;

        case LOGOUT :
            url = "/api/v1/tema/auth/logout";
            req_type = "GET";
            break; 
        
        case DELETE :
            id = get_id();
            if (!id.compare("")) {
                return;
            }

            url = "/api/v1/tema/library/books/" + id;
            req_type = "DELETE";
            break;

        default:
            break;
    }

    string_t message = get_delete_request(req_type, HOST_PORT, url, cookie, token);

    /* Open the connection and send the hhtp requst to the server */
    sock_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0, epoll_fd);
    send_all(sock_fd, message.data(), message.size());
}

int open_connection(const string_t &host_ip, int port_num, int ip_type,
                    int socket_type, int flag, int epoll_fd)
{
    int sock_fd = socket(ip_type, socket_type, flag);
    DIE(sock_fd < 0, "socket failed\n");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(port_num);

    inet_aton(host_ip.data(), &serv_addr.sin_addr);

    int rc = connect(sock_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    DIE(rc < 0, "connect failed\n");

    w_epoll_add_fd_in(epoll_fd, sock_fd);

    return sock_fd;
}

string_t get_input(void)
{
    string_t str;

    std::getline(std::cin, str);
    str[str.size()] = '\0';

    return str;
}

string_t get_id(void)
{
    std::cout << "id=";
    string_t id = get_input();
    
    if (!is_input_valid(id, NUMERIC)) {
        std::cout << INVALID_DIGITS;
        return "";
    }

    return id;
}

nlohmann::json get_user(void)
{
    std::cout << "username=";
    string_t username = get_input();

    std::cout << "password=";
    string_t password = get_input();
    
    if (!is_input_valid(username, NO_SPACES) || 
        !is_input_valid(password, NO_SPACES)) {
        std::cout << INVALID_SPACES;
        return nlohmann::json();
    }

    /* Create the json object */
    nlohmann::json object = {
        {"username", username},
        {"password", password}
    };

    return object;
}

nlohmann::json get_book(void)
{
    std::cout << "title=";
    string_t title = get_input();

    std::cout << "author=";
    string_t author = get_input();

    std::cout << "genre=";
    string_t genre = get_input();

    std::cout << "publisher=";
    string_t publisher = get_input();
    
    std::cout << "page_count=";
    string_t page_count = get_input();
    
    if (!is_input_valid(title, NO_EMPTY) ||
        !is_input_valid(author, NO_EMPTY) ||
        !is_input_valid(genre, NO_EMPTY) ||
        !is_input_valid(publisher, NO_EMPTY)) {
        std::cout << INVALID_EMPTY;
        return nlohmann::json();
    }

    if (!is_input_valid(page_count, NUMERIC)) {
        std::cout << INVALID_DIGITS;
        return nlohmann::json();
    }

    nlohmann::json object = {
        {"title", title},
        {"author", author},
        {"genre", genre},
        {"publisher", publisher},
        {"page_count", page_count}
    };

    return object;
}

bool is_input_valid(string_t &input, input_t type) 
{
    /* If the input is empty, is invalid */
    if (!input.compare("") || !input.compare("\n")) {
        return false;
    }

    /* Else every alphanumeric and punctuation input type is good */
    if (type == NO_EMPTY) {
        return true;
    }

    /* For the other types, iterates through every input char
    and checks if there are any spaces or non digit chars */
    for (auto chr : input) {
        if (type == NUMERIC && !isdigit(chr)) {
            return false;
        }

        if (type == NO_SPACES && chr == ' ') {
            return false;
        }
    }

    return true;
}

string_t parse_response(string_t &buf)
{
    /* The response is between the header end and it's final char */
    size_t hdr_end = buf.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    size_t cnt_len_start = buf.find(CONTENT_LENGTH) + CONTENT_LENGTH_SIZE;

    size_t cnt_len_end = buf.find("\r\n", cnt_len_start);
    string_t cnt_len_str = buf.substr(cnt_len_start,
                                      cnt_len_end - cnt_len_start);

    size_t cnt_len = std::stoi(cnt_len_str);

    /* Returns the content */
    return buf.substr(hdr_end, cnt_len) + "\n";
}

string_t parse_cookie(string_t &buf)
{   
    /* Set-Cookie: ....; */
    size_t pos = buf.find("Set-Cookie");
    pos = buf.find(' ', pos) + 1;

    size_t end_pos = buf.find(';', pos);

    return buf.substr(pos, end_pos - pos);
}

string_t parse_token(string_t &buf)
{
    /* /"token/":/"...." */
    size_t pos = buf.find("token");
    pos = buf.find(":", pos) + 2;

    size_t end_pos = buf.find("\"", pos);

    return buf.substr(pos, end_pos - pos);
}

string_t parse_error(string_t &response)
{
    /* "{\"error\":\"....\"}\n" */
    size_t pos = response.find(":") + 2;
    size_t end_pos = response.find("\"", pos);

    return response.substr(pos, end_pos - pos) + "\n";
}
