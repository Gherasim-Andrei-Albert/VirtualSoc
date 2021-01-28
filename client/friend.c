void reqFriend(Net net, bool &logedIn){
    string name;
    cout << "Type the name:\n";
    getline(cin,name);
    
    net.send("reqFriend");
    net.send(name);
    
    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Friend request sent.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "This person dosen't exists!\n";
                break;
            case 4:
                cout << "Can't request frienship to yourself!\n";
                break;
            case 5:
                cout << "You have already sent this request!\n";
                break;
            case 6:
                cout << "This person already sent you a request first!\n";
                break;
            case 7:
                cout << "This person is already in you friend list!\n";
                break;
        }
}

void reqTypeFriend(Net net, bool &logedIn){
    string name;
    cout << "Type the name:\n";
    getline(cin,name);
    cout << "Type the friend category\n";
    string type;
    getline(cin,type);

    net.send("reqTypeFriend");
    net.send(name);
    net.send(type);
    
    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Friend request sent.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "This person dosen't exists!\n";
                break;
            case 4:
                cout << "Can't request frienship to yourself!\n";
                break;
            case 5:
                cout << "You have already sent this request!\n";
                break;
            case 6:
                cout << "This person already sent you a request first!\n";
                break;
            case 7:
                cout << "This person is already a friend of this category\n";
                break;
            case 8:
                cout << "This person is already a friend, but of another category\n";
                break;
        }
}

void accFriend(Net net, bool &logedIn){
    
    net.send("accFriend");

    string name;
    cout << "Type the name:\n";
    getline(cin,name);
    net.send(name);    
        
    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "You are now friends.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "This person dosen't exists!\n";
                break;
            case 4:
                cout << "Can't accept frienship to yourself!\n";
                break;
            case 5:
                cout << "This person has not sent you a friend request!\n";
                break;
            case 6:
                cout << "You are already friends!\n";
                break;
        }
}

void accTypeFriend(Net net, bool &logedIn){
    string name, type;
    cout << "Type the name:\n"<<endl;
    getline(cin,name);
    cout<<"Type a friend category:\n"<<endl;
    getline(cin,type);
    net.send("accTypeFriend");
    net.send(name);
    net.send(type);    

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "You are now friends.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "This person dosen't exists!\n";
                break;
            case 4:
                cout << "Can't accept frienship to yourself!\n";
                break;
            case 5:
                cout << "This person has not sent you a friend request!\n";
                break;
            case 6:
                cout << "This person is already your friend, but in another category!\n";
                break;
            case 7:
                cout << "This person is already your friend of this category!\n";
                break;
        }
}

void rmFriend(Net net, bool &logedIn){
    string name;
    cout << "Type the name:\n"<<endl;
    getline(cin,name);
    net.send("rmFriend");
    net.send(name);

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Friendship removed succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "This person dosen't exists!\n";
                break;
            case 4:
                cout << "Can't remove frienship to yourself!\n";
                break;
            case 5:
                cout << "You are not friends!\n";
                break;
        }
}

void rmFriendType(Net net, bool &logedIn){
    string type;
    cout<<"Type the friend category:\n"<<endl;
    getline(cin,type);
    net.send("rmFriendType");
    net.send(type);

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Friendship removed succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "This person dosen't exists!\n";
                break;
            case 4:
                cout << "Can't remove frienship to yourself!\n";
                break;
            case 5:
                cout << "This person is not a friend of this category!\n";
                break;
        }
}

void rmAllFriends(Net net, bool &logedIn){
    net.send("rmAllFriends");
    
    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Friends removed succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
        }
}

void rmAllTypeFriends(Net net, bool &logedIn){
    string type;
    cout <<"Type the friend category:\n"<<endl;
    getline(cin,type);
    net.send("rmAllTypeFriends");
    net.send(type); 

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Friends removed succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
        }
}

void changeFriendType(Net net, bool &logedIn){
    string type;
    string name;
    cout << "Type the name:\n"<<endl;
    getline(cin,name);
    cout <<"Type a friend category:\n"<<endl;
    getline(cin,type);
    net.send("changeFriendType");
    net.send(name);
    net.send(type);  

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Friend category changed succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "This person dosen't exists!\n";
                break;
            case 4:
                cout << "Can't set a frienship category to yourself!\n";
                break;
            case 5:
                cout << "This person is not in your friend list!\n";
                break;
        }
}

void listFriends(Net net, bool &logedIn){
    net.send("listFriends");

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:{
                cout << "You're friends are:\n";
                int nrOfFriends = 0;
                net.recv(nrOfFriends);
                for(int i=0;i<nrOfFriends;++i){
                    string friendName;
                    net.recv(friendName);
                    cout << friendName << endl;
                }
                break;
            }
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "You don't have friends!\n";
                break;
        }
}

void listFriendTypes(Net net, bool &logedIn){
    net.send("listFriendTypes");   
    
    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:{
                cout << "The friend categories that you have used are:\n";
                int nrOfCategories = 0;
                net.recv(nrOfCategories);
                for(int i=0;i<nrOfCategories;++i){
                    string category;
                    net.recv(category);
                    cout << category << endl;
                }
                break;
            }
            case 1:{
                cout << "Inexistent command!\n";
                break;
            }
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "You don't have categories of friends!\n";
                break;
        }
}

void listTypeFriends(Net net, bool &logedIn){
    string type;
    cout<<"Type the friend category:\n"<<endl;
    getline(cin,type);
    net.send("listTypeFriends");
    net.send(type);

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:{
                cout << "You're friends are:\n";
                int nrOfFriends = 0;
                net.recv(nrOfFriends);
                for(int i=0;i<nrOfFriends;++i){
                    string friendName;
                    net.recv(friendName);
                    cout << friendName << endl;
                }
                break;
            }
            case 1:{
                cout << "Inexistent command!\n";
                break;
            }
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "You don't have friends of this category!\n";
                break;
        }
}

void listFriendRequests(Net net, bool &logedIn){
    net.send("listFriendRequests");
   
    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:{
                cout << "This users sent you friend request:\n";
                int nrOfReq = 0;
                net.recv(nrOfReq);
                for(int i=0;i<nrOfReq;++i){
                    string user;
                    net.recv(user);
                    cout << user << endl;
                }
                break;
            }
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "You don't have friend requests!\n";
                break;
        }
}