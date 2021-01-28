#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include "commandsTable.c"
#include <signal.h>
using namespace std;

int port;
sockaddr_in server;


int servSock;
Net net;

int conn(){

  int sock;
  if ((sock = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror ("Eroare la socket().\n");
  }

  if (connect (sock, (sockaddr *) &server,sizeof (sockaddr)) == -1)
  {
    perror ("[client]Eroare la connect().\n");
  }
  return sock;
}


void waitComands(){

  bool logedIn = false;

  bool exit = false;

  while(exit == false){
    string command;

    printf("\nType a command:\n\n");
    getline(cin,command);
    cout << "[" << command << "]" << endl;

    if(string(command) == "exit"){
      exit = true;
    }else{
      if(string(command) == "reconnect"){
        net.sock = conn();
        net.connLost = false;
      }else{
        if(commandsTable.find(command)==commandsTable.end()){
            printf("Inexistent command!\n");
        }else{
            (*commandsTable[command])(net, logedIn);
        }
      }
    }
  }

  close(net.sock);
}

void handler(int s) {
  //printf("Caught SIGPIPE\n");
}


int main (int argc, char *argv[])
{			
  	 
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);

  servSock = conn();
  signal(SIGPIPE, handler);
  net.onConnLost = [](){
    cout << "Connection lost! Try the reconnect command.\n";
  };
  net.sock = servSock;
  waitComands();

  return 0;
}
