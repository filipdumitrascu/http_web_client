/* 323CA Dumitrascu Filip-Teodor */
#ifndef _REQUESTS_
#define _REQUESTS_

#include <string>

using string_t = std::string;

/**
 * @brief Get the delete request message
 * 
 * @param type the request type
 * @param host the host
 * @param url the url
 * @param cookie the cookie
 * @param token the token
 * @return string_t 
 */
string_t get_delete_request(const string_t &type, const string_t host,
                            const string_t &url, const string_t &cookie,
                            const string_t &token);

/**
 * @brief Get the post request message
 * 
 * @param host the host
 * @param url the url
 * @param cnt_type  the content type
 * @param json_cnt the json content
 * @param cookie the cookie
 * @param token the token
 * @return string_t 
 */
string_t post_request(const string_t &host, const string_t &url,
                      const string_t &cnt_type, const string_t &json_cnt,
                      const string_t &cookie, const string_t &token);

#endif
