#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include "../net.cpp"
using namespace std;


int main(int argc, char const *argv[])
{
    int confToServ = open("confToServ",O_WRONLY);
    int servToConf = open("servToConf",O_RDONLY);

    bool exit = false;
    while(!exit){
        string command;
        cout << "waiting for a command\n";
        cin >> command;
        if(command=="signUpAdmin"){
            string name;
            cin >> name;  
            int nameLen = strlen(name.c_str())+1;
            char nameChr[nameLen];
            strcpy(nameChr,name.c_str());
            nameChr[nameLen-1] = '\0';
            write(confToServ,&nameLen,sizeof(int));
            write(confToServ,nameChr,nameLen);

            string pass;
            cin >> pass;
            int passLen = strlen(pass.c_str())+1;
            char passChr[passLen];
            strcpy(passChr,pass.c_str());
            passChr[passLen-1] = '\0';
            write(confToServ,&passLen,sizeof(int));
            write(confToServ,passChr,passLen);
    
            int status;
            read(servToConf,&status,sizeof(int));
            
            switch(status){
                case 0:
                    cout << "Succesful registration.\n";
                    break;
                case 2:
                    cout << "Name already taken!\n";
                    break;
            }
        }  
        if(command == "exit")
            exit = true;
    }    
    return 0;
}
