#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <utility>
using namespace std;


typedef vector<string> InputSchema;
typedef map<string,string> Input;


class Net{
    public:
        bool connLost = false;
        void (*onConnLost)() = NULL;
        int sock;

        void recv(int &field){
            if(!connLost){
                connLost = (read(sock,&field,sizeof(int)) <= 0);
                if(connLost){
                    close(sock);
                    if(onConnLost!=NULL)
                        onConnLost();
                }
            }
        }

        void recv(string &field){
            if(!connLost){
                int fieldSize;
                recv(fieldSize);
                if(!connLost){
                    char chrField[fieldSize+1];
                    connLost = (read(sock, chrField, fieldSize) <= 0);
                    chrField[fieldSize] = '\0';
                    field = string(chrField);
                    if(connLost){
                        close(sock);
                        if(onConnLost!=NULL)
                            onConnLost();
                    }
                }
            }
        }


        Input recv(InputSchema schema){
            Input input = {};
            if(!connLost){
                for(auto it = schema.begin(); it!=schema.end(); ++it){
                    string fieldName = *it;
                    string type = *(++it);
                    cout<<"READ "<<fieldName<<endl;
                    if(type == "text"){
                        string value;
                        recv(value);
                        if(!connLost)
                            input.insert({fieldName,value});
                    }else{
                        int value;
                        recv(value);
                        if(!connLost)
                            input.insert({fieldName,to_string(value)});
                    }
                }
            }
            return input;
        }


        void send(int field){
            if(!connLost){
                connLost = (write(sock,&field,sizeof(int)) < 0);
                if(connLost){
                    close(sock);
                    if(onConnLost!=NULL)
                        onConnLost();
                }
            }
        }

        void send(string field){
            if(!connLost){
                int fieldSize = strlen(field.c_str());
                send(fieldSize);
                if(!connLost){
                    connLost = (write(sock, field.c_str(), fieldSize) < 0);      
                    if(connLost){
                        close(sock);
                        if(onConnLost!=NULL)
                            onConnLost();
                    }
        
                }
            }
        }
};