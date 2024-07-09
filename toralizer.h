#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// socket(); -> sys/socket
// connect(); -> sys/socket
// close(); -> unistd
// htons(); -> arpa/inet
// inet_addr(); -> {sys/socket, netinet/in, arpa/inet}

// ip address
#define PROXY "127.0.0.1"
#define PROXYPORT 9050
#define REQSIZE sizeof(struct proxy_request)
#define RESPSIZE sizeof(struct proxy_response)
#define USERNAME "toraliz"

// https://www.openssh.com/txt/socks4.protocol

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

struct proxy_request {
  int8 vn;
  int8 cd;
  int16 dstport;
  int32 dstip;
  unsigned char userid[8];
};

struct proxy_response {
  int8 vn;
  int8 cd;
  int16 _;
  int32 __;
};

// not to call everytime proxy_request and proxy_response. just Req and Res.
typedef struct proxy_request Req;
typedef struct proxy_response Res;

Req *request(const char*, const int);
int main(int, char**);
