
#include <string.h>
#include "HTTPClient.h"
#include "Debug.h"

HTTPClient::HTTPClient()
{
  wifly = WiFly::getInstance();
}

int HTTPClient::get(const char *url, int timeout)
{
  return connect(url, "GET", NULL, NULL, timeout);
}

int HTTPClient::get(const char *url, const char *headers, int timeout)
{
  return connect(url, "GET", headers, NULL, timeout);
}

int HTTPClient::post(const char *url, const char *data, int timeout)
{
  return connect(url, "POST", NULL, data, timeout);
}

int HTTPClient::post(const char *url, const char *headers, const char *data, int timeout)
{
  return connect(url, "POST", headers, data, timeout);
}

int HTTPClient::connect(const char *url, const char *method, const char *data, int timeout)
{
  return connect(url, method, NULL, data, timeout);
}

int HTTPClient::connect(const char *url, const char *method, const char *headers, const char *data, int timeout)
{
  char host[HTTP_MAX_HOST_LEN];
  uint16_t port;
  char path[HTTP_MAX_PATH_LEN];
  
  if (parseURL(url, host, sizeof(host), &port, path, sizeof(path)) != 0) {
    DBG("Failed to parse URL.\r\n");
    return -1;
  }
  
  if (!wifly->connect(host, port, timeout)) {
    DBG("Failed to connect.\r\n");
    return -2;
  }
  
  // Send request
  char buf[HTTP_MAX_BUF_LEN];
  snprintf(buf, sizeof(buf), "%s %s HTTP/1.1\r\n", method, path);
  wifly->send(buf);
  
  // Send all headers
  snprintf(buf, sizeof(buf), "Host: %s\r\nConnection: close\r\n", host);
  wifly->send(buf);
  
  if (data != NULL) {
    snprintf(buf, sizeof(buf), "Content-Length: %d\r\nContent-Type: text/plain\r\n", strlen(data));
    wifly->send(buf);
  }
  
  if (headers != NULL) {
    wifly->send(headers);
  }
  
  // Close headers
  wifly->send("\r\n");
  
  // Send body
  if (data != NULL) {
    wifly->send(data);
  }
  
  return 0;
}

int HTTPClient::parseURL(const char *url, char *host, int max_host_len, uint16_t *port, char *path, int max_path_len)
{
  char *scheme_ptr = (char *)url;
  char *host_ptr = (char *)strstr(url, "://");
  if (host_ptr != NULL) {
    if (strncmp(scheme_ptr, "http://", 7)) {
      DBG("Bad scheme\r\n");
      return -1;
    }
    host_ptr += 3;
  } else {
    host_ptr = (char *)url;
  }

  int host_len = 0;
  char *port_ptr = strchr(host_ptr, ':');
  if (port_ptr != NULL) {
    host_len = port_ptr - host_ptr;
    port_ptr++;
    if (sscanf(port_ptr, "%hu", port) != 1) {
      DBG("Could not find port.\r\n");
      return -3;
    }
  } else {
    *port = HTTP_DEFAULT_PORT;
  }
  
  char *path_ptr = strchr(host_ptr, '/');
  if (host_len == 0) {
    host_len = path_ptr - host_ptr;
  }
  
  if (max_host_len < (host_len + 1)) {
    DBG("Host buffer is too small.\r\n");
    return -4;
  }
  memcpy(host, host_ptr, host_len);
  host[host_len] = '\0';
  
  int path_len;
  char *fragment_ptr = strchr(host_ptr, '#');
  if (fragment_ptr != NULL) {
    path_len = fragment_ptr - path_ptr;
  } else {
    path_len = strlen(path_ptr);
  }
  
  if (max_path_len < (path_len + 1)) {
    DBG("Path buffer is too small.\r\n");
    return -5;
  }
  memcpy(path, path_ptr, path_len);
  path[path_len] = '\0';
  
  return 0;
}


  
