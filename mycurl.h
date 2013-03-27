/* 
 * File:   mycurl.h
 * Author: newmek7
 *
 * Created on 20 March, 2013, 2:53 PM
 */

#ifndef MYCURL_H
#define	MYCURL_H

#include <curl/curl.h>
#include<string>
using std::string;

static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms);
string httpReq(string hostname, string port = "80", string method = "GET", string requestPath = "/", string contentType = "text/html", string content = "", bool ssl = false);
string SOAPReq(string hostname, string port, string requestPath, string SOAPAction, string content, bool ssl);

#endif	/* MYCURL_H */

