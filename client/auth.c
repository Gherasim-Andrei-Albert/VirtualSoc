#include <iostream>
#include "../net.cpp"
#include "console.c"
#include <vector>
#include <functional>
using namespace std;

void signUp(Net net, bool &logedIn){

  string name,pass,role;
  cout << "Type the username:\n";
  getline(cin,name);
  cout << "Type the password:\n";
  getline(cin,pass);
  cout << "Type the role:\n";
  getline(cin,role);

  net.send("signUp");
  net.send(name);
  net.send(pass);
  net.send(role); 
  
  int status;
  net.recv(status);

  if(!net.connLost)
    switch (status)
    {
      case 1:
        cout << "Inexistent command!\n";
        break;
      case 3:
        cout << "Username already taken!\n";
        break;
      case 2:
        cout << "Inexistent role!\n";
        break;
      case 4:
        cout << "Can't register as admin!\n";
        break;
      case 0:
        logedIn = true;
        cout << "Wellcome!\n";
        break;
    } 
}

void logIn(Net net, bool &logedIn){

  string name;
  string pass;

  printf("Tastati numele:\n");
  getline(cin,name);


  printf("Tastati parola:\n");
  getline(cin,pass);
  
  net.send("logIn");
  net.send(name);
  net.send(pass);

  int status;
  net.recv(status);

  if(!net.connLost)
    switch (status)
    {
      case 1:
        cout << "Inexistent command!\n";
        break;
      case 2:
        cout << "Wrong username or password!\n";
        break;
      case 0:
        logedIn = true;
        cout << "Wellcome back!\n";
        break;
    } 
}

void logOut(Net net, bool &logedIn){

  if(logedIn == false){
    cout << "Already loged out!\n";
  }else{
    net.send("logOut");

    int status;
    net.recv(status);
    
    if(!net.connLost)
      switch (status)
      {
        case 1:
          cout << "Inexistent command!\n";
          break;
        case 2:
          cout << "Already loged out!\n";
          break;
        case 0:
          logedIn = false;
          cout << "Good bye!\n";
          break;
      }  
    }
}