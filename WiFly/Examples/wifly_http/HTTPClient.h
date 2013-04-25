

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#define HTTP_CLIENT_DEFAULT_TIMEOUT         30000  // 3s

#define HTTP_MAX_HOST_LEN                   20
#define HTTP_MAX_PATH_LEN                   64
#define HTTP_MAX_BUF_LEN                    100

#define HTTP_DEFAULT_PORT                   80

#include <Arduino.h>
#include <WiFly.h>

class HTTPClient {
  public:
    HTTPClient();
    
    int get(const char *url, int timeout = HTTP_CLIENT_DEFAULT_TIMEOUT);
    int get(const char *url, const char *header, int timeout = HTTP_CLIENT_DEFAULT_TIMEOUT);
    int post(const char *url, const char *data, int timeout = HTTP_CLIENT_DEFAULT_TIMEOUT);
    int post(const char *url, const char *headers, const char *data, int timeout = HTTP_CLIENT_DEFAULT_TIMEOUT);
    
  private:
    int parseURL(const char *url, char *host, int max_host_len, uint16_t *port, char *path, int max_path_len);
    int connect(const char *url, const char *method, const char *data, int timeout = HTTP_CLIENT_DEFAULT_TIMEOUT);
    int connect(const char *url, const char *method, const char *header, const char *data, int timeout = HTTP_CLIENT_DEFAULT_TIMEOUT);
 
    WiFly* wifly;
};

#endif // __HTTP_CLIENT_H__

