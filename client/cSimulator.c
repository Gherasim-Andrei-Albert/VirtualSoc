#include <unistd.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
using namespace std;


int main(int argc, char const *argv[])
{
    pid_t child;
    int p[2];
    pipe(p);
    int sleepTime = 500000;
    if(argc==2){
        sleepTime = atoi(argv[1]);
    }

    child = fork ();
    if (child != 0) 
    {
        bool exit = false;
        while(!exit){
            //sleep(2);
            string command;
            getline(cin,command);
            int commandLen = strlen(command.c_str())+1;
            char charComm[commandLen];
            strcpy(charComm,command.c_str());
            charComm[commandLen-1] = '\n';
            //charComm[commandLen-1] = '\0';
            for(int i=0;i<commandLen;++i){
                usleep(sleepTime);
                write(1,charComm+i,1);
            }
            write(p[1],charComm,commandLen);

            if(command == "exit")
                exit = true;
        }
    }
    else
    {
        dup2(p[0],0);
        
        execl("c","c","127.0.0.1","2025",NULL);

    }


    return 0;
}
