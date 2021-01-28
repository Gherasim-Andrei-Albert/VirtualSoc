#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <my_global.h>
#include <mysql.h>
#include "auth.c"
#include "prof.c"
#include "friend.c"
#include "chat.c"
#include "posts.c"
#include "admin.c"
#include "handlersTable.c"
using namespace std;

#define PORT 2025
int servSock;

void *handleRequests(void *arg){

    Net net;
    net.sock = *(int *)arg;
    net.onConnLost = [](){
        cout << "A client connection was lost.\n";
    };
    
    int logedUserID = 0;

    while(!net.connLost){
        int found = 0;

        cout << pthread_self() << " waiting for requests\n";
        string reqMethod;
        net.recv(reqMethod);
        if(!net.connLost){
            cout <<"["<< reqMethod<<"]" << endl;

            if(handlersTable.find(reqMethod) == handlersTable.end()){
                cout<<"not found\n";
                int status = 1;
                cout<<1<<endl;
                /*if(write(cliSock, &status, sizeof(status)) == 0){
                    close(cliSock);
                    return 0;
                }*/
            }else{
                (*handlersTable[reqMethod])(net, conn, logedUserID);
            }
        }
    }
}

void* handleCommands(void *arg){
    int confToServ = open("confToServ",O_RDONLY);
    int servToConf = open("servToConf",O_WRONLY);
    while(true){
        int nameLen, passLen;

        read(confToServ,&nameLen,sizeof(int));
        char name[nameLen];
        read(confToServ,name,nameLen);

        read(confToServ,&passLen,sizeof(int));
        char pass[passLen];
        read(confToServ,pass,passLen);

        int status = 0;

        if(DBexists(
            "Users",
            {{"username",name}})){
            status = 2;
        }
        else{
            
            //sign up user

            int roleID = DBfindID("Roles",{{"name","admin"}});

            DBadd("Users",{
                {"username",name},
                {"pass",pass},
                {"roleID",to_string(roleID)},
                {"privacy","private"}});
        }

        write(servToConf,&status,sizeof(int));
    }
}


void initDB(){
    conn = mysql_init(NULL);
    if(conn==NULL){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
    if( mysql_real_connect(conn, "localhost", "root", "root_pswd",NULL,0,NULL,0)){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

    DBcreate("VirtualSoc");

    /* mysql_query(conn,
        "CREATE TABLE Roles(\
            ID INT NOT NULL AUTO_INCREMENT,\
            name TEXT,\
            PRIMARY KEY (ID))"); */

    mysql_query(conn,
        "INSERT INTO Roles VALUES\
            (NULL,'admin'),\
            (NULL,'user')");

    /* if(mysql_query(conn,
        "CREATE TABLE Users(\
            ID INT NOT NULL AUTO_INCREMENT,\
            username TEXT NOT NULL,\
            pass TEXT NOT NULL,\
            roleID INT NOT NULL,\
            token TEXT,\
            PRIMARY KEY (ID),\
            FOREIGN KEY (roleID) REFERENCES Roles(ID))")){
        fprintf(stderr,"%s\n",mysql_error(conn));
    } */

    /*if(mysql_query(conn,
        "CREATE TABLE POSTS(
            ID INT NOT NULL AUTO_INCREMENT, 
            privacy TEXT, 
            AUTHOR_ID INT, 
            DATA_BLOCK TEXT, 
            PRIMARY KEY (ID))")){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

    if(mysql_query(conn,"CREATE TABLE CHAT(ID INT NOT NULL AUTO_INCREMENT, PRIVACY TEXT, ID_AUTHOR INT, MESSAGE TEXT, PRIMARY KEY (ID))")){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

    if(mysql_query(conn,"CREATE TABLE GROUPS(ID INT NOT NULL AUTO_INCREMENT, NAME TEXT, PRIMARY KEY (ID))")){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

    if(mysql_query(conn,"CREATE TABLE FRIENDS(ID INT NOT NULL AUTO_INCREMENT, USER_ID1 INT, USER_ID2 INT, PRIMARY KEY (ID))")){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

    mysql_query(conn,"CREATE TABLE USER_GROUP_ASSIGN(ID INT NOT NULL AUTO_INCREMENT, USER_ID INT,GROUP_ID INT, PRIMARY KEY (ID))");
    */
}


void resetDB(){
    conn = mysql_init(NULL);
    if(conn==NULL){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
    if( mysql_real_connect(conn, "localhost", "root", "root_pswd",NULL,0,NULL,0)){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
    mysql_query(conn,"DROP DATABASE VirtualSoc");
    initDB();

}


int main(int argc, char **argv){

    DBdefSchema({
        {"Roles",{
                {"name",{"TEXT"}}
            }
        },
        {"Users",{
                {"username",{"TEXT"}},
                {"pass",{"TEXT"}},
                {"roleID",{"INT"}},
                {"privacy",{"TEXT"}},
                {"profGroupID",{"INT","nullable"}},
                {"birthdate",{"DATETIME","nullable"}}
            }
        },
        {"Groups",{
                {"groupID",{"INT"}},
                {"userID",{"INT"}}
            }
        },
        {"Friends",{
                {"userID",{"INT"}},
                {"groupID",{"INT"}},
                {"friendType",{"TEXT","nullable"}}
            }
        },
        {"Requests",{
                {"userID",{"INT"}},
                {"friendID",{"INT"}},
                {"friendType",{"TEXT","nullable"}}
            }
        },
        {"ChatGroups",{
                {"name",{"TEXT"}},
                {"groupID",{"INT"}},
                {"threadsListID",{"INT","nullable"}}
            }
        },
        {"ThreadsLists",{
                {"listID",{"INT"}},
                {"thread",{"INT","nullable"}}
            }
        },
        {"2PersonsChat",{
                {"user1ID",{"INT"}},
                {"user2ID",{"INT"}},
                {"thread1",{"INT","nullable"}},
                {"thread2",{"INT","nullable"}}
            }
        },
        {"2PersonsChatMessages",{
                {"chatID",{"INT"}},
                {"msg",{"TEXT"}},
                {"authorID",{"TEXT"}},
                {"date",{"DATETIME"}}
            }
        },
        {"GroupChatMessages",{
                {"chatID",{"INT"}},
                {"msg",{"TEXT"}},
                {"authorID",{"INT"}},
                {"date",{"DATETIME"}}
            }
        },
        {"Posts",{
                {"authorID",{"INT"}},
                {"privacy",{"TEXT"}},
                {"post",{"TEXT"}},
                {"editedPostID",{"INT","nullable"}},
                {"originalPostID",{"INT","nullable"}},
                {"date",{"DATETIME"}}
            }
        }
    });

    if(argc == 2 && string(argv[1])=="-initDB")
        initDB();
    else
    if(argc == 2 && string(argv[1])=="-resetDB")
        resetDB();
    else{
        DBconn("localhost", "root", "root_pswd","VirtualSoc");
    }
    servSock = socket(AF_INET, SOCK_STREAM, 0);

    int on = 1;

    setsockopt(servSock, SOL_SOCKET,
                SO_REUSEADDR, &on, 
                sizeof(on));

    sockaddr_in servAddr;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_family = AF_INET;

    bind(servSock, (sockaddr *) &servAddr, sizeof(servAddr));
    signal(SIGPIPE, SIG_IGN);

    unsigned int maxBackLog = 1;
    listen(servSock, maxBackLog);


    pthread_t tmpThreadId;

    pthread_create(
        &tmpThreadId,
        NULL,
        &handleCommands,
        (void*)&servSock
    );

    while(true){
        
        sockaddr_in cliAddr;
        socklen_t cliAddrLen = sizeof(cliAddr);

        cout<<"Waiting for connections\n";

        Net net;
        int cliSock = accept(servSock,(sockaddr *) &cliAddr, &cliAddrLen);

        pthread_t tmpThreadId;

        pthread_create(
            &tmpThreadId,
            NULL,
            &handleRequests,
            (void*)&cliSock
        );
    }


    return 0;
}