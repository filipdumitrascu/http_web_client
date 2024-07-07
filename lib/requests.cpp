/* 323CA Dumitrascu Filip-Teodor */
#include "requests.hpp"

string_t get_delete_request(const string_t &type, const string_t host,
                            const string_t &url, const string_t &cookie,
                            const string_t &token)
{
    std::string message;

    message += type + " " + url + " HTTP/1.1\r\n";
    message += "Host: " + host + "\r\n";
    message += "Connection: close\r\n";

    if (!cookie.empty()) {
        message += "Cookie: " + cookie + "\r\n";
    }

    if (!token.empty()) {
        message += "Authorization: Bearer " + token + "\r\n";
    }

    message += "\r\n";

    return message;
}

string_t post_request(const string_t &host, const string_t &url,
                      const string_t &cnt_type, const string_t &json_cnt,
                      const string_t &cookie, const string_t &token)
{
    std::string message;

    message += "POST " + url + " HTTP/1.1\r\n";
    message += "Host: " + host + "\r\n";
    message += "Connection: close\r\n";
    message += "Content-Type: " + cnt_type + "\r\n";
    message += "Content-Length: " + std::to_string(json_cnt.size()) + "\r\n";

    if (!cookie.empty()) {
        message += "Cookie: " + cookie + "\r\n";
    }

    if (!token.empty()) {
        message += "Authorization: Bearer " + token + "\r\n";
    }

    message += "\r\n" + json_cnt;

    return message;
}
