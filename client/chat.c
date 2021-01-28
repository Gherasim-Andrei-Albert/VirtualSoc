void mkChatGr(Net net, bool &logedIn){

    net.send("mkChatGr");
    cout<<"Type GroupName:\n";
    string name;
    getline(cin,name);
    net.send(name);
    cout<<"Type the number of members:\n";
    int membersNr;
    cin>>membersNr;
    cin.get();
    cout << membersNr << endl;
    net.send(membersNr);
    for(int i=0;i<membersNr;++i){
        string member;
        cout<<"Type member name:\n";
        getline(cin,member);
        cout << member << endl;
        net.send(member);
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
                cout << "Inexistent person!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 0:
                cout << "Group created succesfuly.\n";
                break;
        } 
}

void sendMsg(Net net, bool &logedIn){
    net.send("sendMsg");
    cout<<"Do you send to a group or a person? (Type person or group.)\n";
    string type;
    getline(cin,type);
    if(type=="person"){
        net.send(type);
        cout<<"Type the person name\n";
        string name;
        getline(cin,name);
        net.send(name);
        string msg = "";
        cout<<"Type the message\n";
        bool done = false;
        while(!done){
            string line;
            getline(cin,line);
            if(line == "done")
                done = true;
            else
                msg+=line + "\n";
        }
        msg.pop_back();
        net.send(msg);
    }
    if(type == "group"){
        net.send(type);
        cout<<"Type the group ID\n";
        int chatID;
        cin >> chatID;
        cin.get();
        net.send(chatID);
        string msg = "";
        cout<<"Type the message\n";
        bool done = false;
        while(!done){
            string line;
            getline(cin,line);
            if(line == "done")
                done = true;
            else
                msg+=line+"\n";
        }
        msg.pop_back();
        net.send(msg);
    }

    int status;
    net.recv(status);
    
    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Message sent.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                if(type=="person")
                    cout << "This person dosen't exists!\n";
                else
                    cout << "You are not in a chat group with this ID!\n";
                break;
        }
}

void leaveChat(Net net, bool &logedIn){
    net.send("leaveChat");
    cout<<"Type chat group ID:\n";
    int chatID;
    cin>>chatID;
    cin.get();
    net.send(chatID);

    int status;
    net.recv(status);

    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Chat leaved succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "You are not in a group with this ID!\n";
                break;
        }
}

void addPersonToChat(Net net, bool &logedIn){
    cout<<"Type the chat group ID:\n";
    int chatID;
    cin>>chatID;
    cin.get();
    cout<<"Type the name of the person:\n";
    string name;
    getline(cin,name);
    net.send("addPersonToChat");
    net.send(chatID);
    net.send(name);

    int status;
    net.recv(status);

    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Person added succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "You're not in a chat group with this ID!\n";
                break;
            case 5:
                cout << "Inexistent user!\n";
                break;
        }
}

void changeChatName(Net net, bool &logedIn){
    cout<<"Type chat group ID:\n";
    int chatID;
    cin>>chatID;
    cin.get();
    cout<<"Type the new name:\n";
    string newName;
    getline(cin,newName);
    net.send("changeChatName");
    net.send(chatID);
    net.send(newName);

    int status;
    net.recv(status);

    if(!net.connLost)
        switch(status){
            case 0:
                cout << "Group name changed succesfuly.\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You are not loged in!\n";
                break;
            case 3:
                cout << "You are not in a chat group with this ID!\n";
                break;
        }
}

void listChatGroups(Net net, bool &logedIn){
    net.send("listChatGroups");
    
    int status;
    net.recv(status);

    if(!net.connLost)
        switch(status){
            case 0:{
                cout << "You're groups are:\n";
                int nrOfGroups = 0;
                net.recv(nrOfGroups);
                for(int i=0;i<nrOfGroups;++i){
                    string name;
                    int chatID;
                    net.recv(chatID);
                    net.recv(name);
                    cout<<"ID:"<<chatID<<" Name:"<<name<<endl;
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
                cout << "You are not in any chat groups!\n";
                break;
        }
}

void seeChat(Net net, bool &logedIn){
    cout<<"Do you want to see chat from a group or a person? (Type person or group.)\n";
    string type;
    getline(cin,type);
    if(type=="person"){
        net.send("seeChat");
        net.send(type);
        cout<<"Type the name\n";
        string name;
        getline(cin,name);
        net.send(name);
    }
    else
    if(type=="group"){
        net.send("seeChat");
        net.send(type);
        cout<<"Type the chat group ID\n";
        int chatID;
        cin>>chatID;
        cin.get();
        net.send(chatID);
    }else {cout << "wrongResponse\n"; return;}
    
    int status;
    net.recv(status);

    if(!net.connLost)
        switch(status){
            case 0:{
                cout << "The messages in this chat are:\n";
                int nrOfMesages = 0;
                net.recv(nrOfMesages);
                for(int i=0;i<nrOfMesages;++i){
                    string author, msg, date;
                    net.recv(author);
                    net.recv(msg);
                    net.recv(date);
                    cout << "---------\n";
                    cout << author << ":\n";
                    cout << msg << endl;
                    cout << date << endl;
                    cout << "---------\n";
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
                if(type=="person")
                    cout << "This person dosen't exists!\n";
                else
                    cout << "You are not in a chat group with this ID!\n";
                break;
            case 5:
                if(type=="person")
                    cout << "No messages with this person\n";
                else
                    cout << "No messages in this group\n";
        }
}