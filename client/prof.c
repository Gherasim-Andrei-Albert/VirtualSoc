void seeProf(Net net, bool &logedIn){
    string name;
    cout << "Type the username:\n";
    getline(cin,name);
    net.send("seeProf");
    net.send(name);
    
    int status;
    net.recv(status);

    if(!net.connLost)
        switch (status)
        {
            case 5:
                cout<< "Inexistent user!\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 4:
                cout << "Server error!\n";
                break;
            case 3:
                cout << "Profile private to you!\n";
                break;
            case 2:
                cout << "Profile private to you! You're loged out!\n";
                break;
            case 0:
                cout << "Profile available.\n";
                string name, role, birthdate;
                net.recv(name);
                net.recv(role);
                net.recv(birthdate);
                cout << "name " << name << endl;
                cout << "role " << role << endl;
                if(birthdate!="NULL")
                    cout << "birthdate " << birthdate << endl;
                break;
        }

}

void setPrivacy(Net net, bool &logedIn){
    cout << "Type the privacy level (public, private, friends, group, or a friend category):\n";
    string privacy;
    getline(cin,privacy);
    net.send("setPrivacy");
    net.send(privacy);

    if(privacy=="group"){
        int membersNr;
        cout<<"Type the number of members:\n";
        cin>>membersNr;
        cin.get();
        net.send(membersNr);
        for(int i=0;i<membersNr;++i){
            string member;
            cout<<"Type member name:\n";
            getline(cin,member);
            net.send(member);
        }
    }

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch (status)
        {
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 4:
                cout << "Server error!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "Inexistent privacy level!\n";
                break;
            case 0:
                cout << "Privacy modified.\n";
                break;
        }
}

void editProf(Net net, bool &logedIn){
    int nrOfProps;
    cout << "Type the number of properties you want to change:\n" <<endl;
    cin>>nrOfProps;
    cin.get();
    net.send("editProf");
    for(int i=0;i<nrOfProps;++i){
        string propName,value;
        cout << "Type the property name:\n"<<endl;
        getline(cin,propName);
        cout << "Type the property value:\n"<<endl;
        getline(cin,value);
        net.send(propName);
        net.send(value);
    }

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch (status)
        {
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 4:
                cout << "Username already taken!\n";
                break;
            case 3:
                cout << "You are not loged in!\n";
                break;
            case 2:
                cout << "Invalid property!\n";
                break;
            case 0:
                cout << "Profile edited.\n";
                break;
        }
}