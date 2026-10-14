#ifndef CURLINC_CURLVER_H
#define CURLINC_CURLVER_H

#define LIBCURL_COPYRIGHT "1996 - 2021 Daniel Stenberg, <daniel@haxx.se>."

#define LIBCURL_VERSION "7.79.1"

#define LIBCURL_VERSION_MAJOR 7
#define LIBCURL_VERSION_MINOR 79
#define LIBCURL_VERSION_PATCH 1

#define LIBCURL_VERSION_NUM 0x074f01

#define LIBCURL_TIMESTAMP "2021-09-22"

#define CURL_VERSION_BITS(x,y,z) ((x)<<16|(y)<<8|(z))
#define CURL_AT_LEAST_VERSION(x,y,z) \
  (LIBCURL_VERSION_NUM >= CURL_VERSION_BITS(x, y, z))

#endif
