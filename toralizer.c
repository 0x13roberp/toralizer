#include "toralizer.h"

int main(int argc, char *argv[]){
  char *host;
  int port;
  int s; // socket file descriptor

  struct sockaddr_in sock; 


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
  close(s);

  return 0;
}
