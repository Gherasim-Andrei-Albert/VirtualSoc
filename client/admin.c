void listUsers(Net net, bool &logedIn){

    net.send("listUsers");

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:{
                int nrOfUsers = 0;
                net.recv(nrOfUsers);
                for(int i=0;i<nrOfUsers;++i){
                    string name;
                    net.recv(name);
                    cout << name << endl;
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
                cout << "You are not an admin!\n";
                break;
        }
}

void listAdmins(Net net, bool &logedIn){

    net.send("listAdmins");

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:{
                int nrOfUsers = 0;
                net.recv(nrOfUsers);
                for(int i=0;i<nrOfUsers;++i){
                    string name;
                    net.recv(name);
                    cout << name << endl;
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
                cout << "You are not an admin!\n";
                break;
        }
}

void deleteUser(Net net, bool &logedIn){
    
    net.send("deleteUser");

    string name;
    cout << "Give the user's name:\n";
    cin >> name;
    net.send(name); 
    
    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "User deleted succesfuly!\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 4:
                cout << "This person dosen't exists!\n";
                break;
            case 3:
                cout << "You are not an admin!\n";
                break;
            case 5:
                cout << "Can't delete an admin!\n";
                break;
        }
}