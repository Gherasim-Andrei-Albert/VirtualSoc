#include <string>

#include <iostream>


using namespace std;

bool isAdmin(int ID){
    int adminRoleID = 
        DBfindID("Roles",{{"name","admin"}});
    return DBexists("Users",{   
            {"ID",to_string(ID)},
            {"roleID",to_string(adminRoleID)}});
}

void listUsers(Net net, MYSQL* conn, int &logedUserID){

    int status = 0;
    if(!logedUserID)
        status = 2;
    else
    if(!isAdmin(logedUserID))
        status = 3;
    
    vector<string> users;
    if(status == 0){
        int adminRoleID = 
            DBfindID("Roles",{{"name","user"}});
        users = DBfind("Users",{
            {"roleID",to_string(adminRoleID)}
        },"username");
    }
    
    net.send(status);

    if(status == 0){
        net.send(users.size());
        for(auto user : users){
            net.send(user);
        }
    }
}

void listAdmins(Net net, MYSQL* conn, int &logedUserID){

    int status = 0;
    if(!logedUserID)
        status = 2;
    else
    if(!isAdmin(logedUserID))
        status = 3;
        
    vector<string> users;
    if(status == 0){
        int adminRoleID = 
            DBfindID("Roles",{{"name","admin"}});
        users = DBfind("Users",{
            {"roleID",to_string(adminRoleID)}
        },"username");
    }
    
    net.send(status);

    if(status == 0){
        net.send(users.size());
        for(auto user : users){
            net.send(user);
        }
    }
}

void deleteUser(Net net, MYSQL* conn, int &logedUserID){

    string name;
    net.recv(name);

    int userID;
    
    int status = 0;
    if(!logedUserID)
        status = 2;
    else
    if(!isAdmin(logedUserID))
        status = 3;
    else
    if(!DBexists("Users",{{"username",name}}))
        status = 4;
    else{
        userID = DBfindID("Users",{{"username",name}});
        if(isAdmin(userID))
            status = 5;
    }
    
    if(status == 0){
        DBdelete("Users",{{"ID",to_string(userID)}});
    }

    net.send(status);
}
