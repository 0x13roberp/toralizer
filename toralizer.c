#include "toralizer.h"

// request petition
Req *request(const char *dstip, const int dstport){
  Req *req;

  req = malloc(REQSIZE);

  // fields for the request. socks4 page
  req->vn = 4; 
  req->cd = 1;
  req->dstport = htons(dstport);
  req->dstip = inet_addr(dstip);
  strncpy(req->userid, USERNAME, 8);

  return req;
}

int main(int argc, char *argv[]){
  char *host;
  int port;
  int s; // socket file descriptor

  struct sockaddr_in sock; 
  int success; // we use this int as a predicate value. for true or false
  
  Req *req; // request pointer to work with
  char buf[RESPSIZE];

  Res *res; // response pointer to work with

  // to verify we dont more than 2 values in the cli
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);

    return -1;
  }

  host = argv[1];
  port = atoi(argv[2]); // atoi to conver str to int, because the argv are strings

  // af_inet is an address family that is used to designate the type of addresses that your socket can communicate with.
  // sock_stream for a tcp connection
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket ");
    return -1;
  }

  // socket configuration
  sock.sin_family = AF_INET;
  sock.sin_port = htons(PROXYPORT);
  sock.sin_addr.s_addr = inet_addr(PROXY);

  // manual of connect: if the connection succeeds zero is returned, otherwise -1 is returned (error)
  if (connect(s, (struct sockaddr *)&sock, sizeof(sock))){
    perror("connect");

    return -1;
  }
  
  printf("Connected to proxy\n");
  req = request(host, port);
  write(s, req, REQSIZE); // send the package
  
  memset(buf, 0, RESPSIZE);
  if (read(s, buf, RESPSIZE) < 1) {
    perror("read");
    free(req);
    close(s);

    return -1;
  }
  
  res = (Res *)buf;
  success = (res->cd == 90); // if cd = 90 then success true
  if (!success) {
    fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);
    close(s);
    free(req);

    return -1;
  }

  printf("Successfully Connected through the proxy to %s: %d\n", host, port);
  close(s);
  free(req);

  return 0;
}
