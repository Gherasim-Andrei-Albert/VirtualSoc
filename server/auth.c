#include <string>
//#include <my_global.h>
//#include <mysql.h>
#include "DB.c"
#include "../net.cpp"
using namespace std;

int findUserID(string username, string pass){
    return DBfindID("Users",{   
            {"username",username},
            {"pass",pass}});
}

int findRoleID(string name){
    return DBfindID(
        "Roles",{   
            {"name",name}});
}

void signUp(Net net, MYSQL* conn, int &logedUserID){

    cout << "Thread " << pthread_self()
         << " is handling a sign up request\n";

    Input signUpForm = 
        net.recv({
            "username","text",
            "pass","text",
            "role","text"});

    int statusCode = 0;

    bool existUsername = DBexists(
        "Users",
        {{"username",signUpForm["username"]}});

    if(existUsername){
        statusCode = 3;
    }
    else{
        int existsRole = DBexists(
            "Roles",
            {{"name",signUpForm["role"]}});

        if(!existsRole){
            statusCode = 2;
        }

        if(signUpForm["role"]=="admin")
            statusCode = 4;
    }

    if(statusCode == 0){
        
        //sign up user

        int roleID = findRoleID(signUpForm["role"]);

        DBadd("Users",{
            {"username",signUpForm["username"]},
            {"pass",signUpForm["pass"]},
            {"roleID",to_string(roleID)},
            {"privacy","private"}});

        logedUserID = findUserID(
            signUpForm["username"], signUpForm["pass"]);
    }
    //cout << pthread_self() << " database work ended\n";

    net.send(statusCode);
    //cout << pthread_self() << " writing ended\n";

    cout << "Thread " << pthread_self()
         << " finished the sign up request\n";
}

void logIn(Net net, MYSQL* conn, int &logedUserID){
cout<<"logIn\n";
    Input credentials = 
        net.recv({
            "username","text",
            "pass","text"});

    int statusCode = 0;

    int ID = findUserID(
        credentials["username"],
        credentials["pass"]);

    if(ID == 0){
        statusCode = 2;
    }else{     
        logedUserID = ID;
    }

    net.send(statusCode);
    //write(cliSock, &statusCode, sizeof(int));
}


void logOut(Net net, MYSQL* conn, int &logedUserID){
cout<<"logOut\n";
    int statusCode = 0;
    if(logedUserID == 0){
        statusCode = 2;
    }else{
        logedUserID = 0;
    }
    
    net.send(statusCode);
}