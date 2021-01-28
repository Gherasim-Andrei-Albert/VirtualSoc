void mkChatGr(Net net, MYSQL* conn, int &logedUserID){
    string chatName;
    net.recv(chatName);
    int personsNr;
    net.recv(personsNr);
    cout << personsNr << endl;
    vector<string> persons = {};
    for(int i=0;i<personsNr;++i){
        string person;
        net.recv(person);
        cout << person << endl;
        persons.push_back(person);
    }
    int status = 0;
    if(!logedUserID){
        status = 2;
    }
    else{
        for(int i=0;i<persons.size();++i){
            if(!DBexists("Users",{{"username",persons[i]}})){
                status = 4;
                break;
            }
        }
    }

    if(status==0){
        string query = "SELECT IFNULL(max(groupID),0)+1 FROM Groups";
        mysql_query(conn,query.c_str());
        MYSQL_ROW row = mysql_fetch_row(mysql_store_result(conn));
        int groupID;
        if(row[0] == NULL){
            groupID = 1;
        }else
            groupID = atoi(row[0]);
        DBadd("ChatGroups",{
            {"groupID",to_string(groupID)},
            {"name",chatName}});
        DBadd("Groups",{
            {"groupID",to_string(groupID)},
            {"userID",to_string(logedUserID)}});
        for(int i=0;i<persons.size();i++){
            int memberID = DBfindID("Users",{{"username",persons[i]}});
            DBadd("Groups",{
                {"groupID",to_string(groupID)},
                {"userID",to_string(memberID)}});
        }
    }
    
    net.send(status);

}

void sendMsg(Net net, MYSQL* conn, int &logedUserID){
    string type;
    net.recv(type);
    int status = 0;
    cout<<"|"<<type<<"|"<<endl;
    
    if(!logedUserID){
        status = 2;
    }
    else{
        if(type=="person"){
            string user;
            net.recv(user);
            string msg;
            net.recv(msg);
            int userID = DBfindID("Users",{{"username",user}});
            if(!userID)
                status = 3;
            if(status == 0){
                int chatID;
                if(DBexists("2PersonsChat",{{"user1ID",to_string(logedUserID)},{"user2ID",to_string(userID)}})){
                    chatID = DBfindID("2PersonsChat",{{"user1ID",to_string(logedUserID)},{"user2ID",to_string(userID)}});
                }else
                if(DBexists("2PersonsChat",{{"user2ID",to_string(logedUserID)},{"user1ID",to_string(userID)}})){
                    chatID = DBfindID("2PersonsChat",{{"user2ID",to_string(logedUserID)},{"user1ID",to_string(userID)}});
                }else
                {
                    DBadd("2PersonsChat",{{"user1ID",to_string(logedUserID)},{"user2ID",to_string(userID)}});
                    chatID = DBfindID("2PersonsChat",{{"user1ID",to_string(logedUserID)},{"user2ID",to_string(userID)}});
                }
                DBadd("2PersonsChatMessages",{
                    {"chatID",to_string(chatID)},
                    {"msg",msg},
                    {"authorID",to_string(logedUserID)},
                    {"date","NOW"}});
            }
        }
        if(type=="group"){ 
            int chatID = 0;
            net.recv(chatID);
            string msg;
            net.recv(msg);
            if(!DBexists("ChatGroups",
                {{"ID",to_string(chatID)}})){
                status = 3;
            }else{
                int chatGroupID = atoi(
                    DBfindOneValue("ChatGroups",
                        {{"ID",to_string(chatID)}},"groupID","0").c_str());
                if(!DBexists("Groups",{
                    {"groupID",to_string(chatGroupID)},
                    {"userID",to_string(logedUserID)}})){
                    status = 3;
                }
            }

            if(status == 0){
                DBadd("GroupChatMessages",{
                    {"chatID",to_string(chatID)},
                    {"authorID",to_string(logedUserID)},
                    {"msg",msg},
                    {"date","NOW"}});
            }
        }
    }
    
    net.send(status);
}

void seeChat(Net net, MYSQL* conn, int &logedUserID){

    string type;
    net.recv(type);
    int status = 0;
    cout<<"|"<<type<<"|"<<endl;
    if(!logedUserID)
        status = 2;
    else{
        if(type=="person"){
            string user;
            net.recv(user);
            int userID = DBfindID("Users",{{"username",user}});
            if(!userID)
                status = 3;
            if(status == 0){
                int chatID;
                if(DBexists("2PersonsChat",{{"user1ID",to_string(logedUserID)},{"user2ID",to_string(userID)}})){
                    chatID = DBfindID("2PersonsChat",{{"user1ID",to_string(logedUserID)},{"user2ID",to_string(userID)}});
                }else
                if(DBexists("2PersonsChat",{{"user2ID",to_string(logedUserID)},{"user1ID",to_string(userID)}})){
                    chatID = DBfindID("2PersonsChat",{{"user2ID",to_string(logedUserID)},{"user1ID",to_string(userID)}});
                }else
                {
                    status = 5;
                }

                net.send(status);

                if(status!=5){
                    vector<string> columns = {"msg","authorID","date"};
                    DBrows rows = DBfind("2PersonsChatMessages",{{"chatID",to_string(chatID)}},columns);
                    net.send(rows.size());
                    for(auto it = rows.begin();it!=rows.end();++it){
                        string authorID = (*it)["authorID"];
                        string author = DBfindOneValue("Users",{
                            {"ID",authorID}},"username","NULL");
                        cout << author << endl;
                        cout << (*it)["msg"] << endl;
                        cout << (*it)["date"] << endl;
                        net.send(author);
                        net.send((*it)["msg"]);
                        net.send((*it)["date"]);
                    }
                }
            }
        }

        if(type=="group"){
            int chatID;
            net.recv(chatID);
            if(!DBexists("ChatGroups",{{"ID",to_string(chatID)}})){
                status = 3;
            }else{
                int chatGroupID = atoi(
                    DBfindOneValue("ChatGroups",
                        {{"ID",to_string(chatID)}},"groupID","0").c_str());
                if(!DBexists("Groups",{
                    {"groupID",to_string(chatGroupID)},
                    {"userID",to_string(logedUserID)}})){
                    status = 3;
                }
            }

            DBrows rows = {};
            if(status == 0){
                vector<string> columns = {"msg","authorID","date"};
                rows = DBfind("GroupChatMessages",{
                    {"chatID",to_string(chatID)}},columns);
                if(rows.empty())
                    status = 5;
            }

            net.send(status);

            if(status == 0){
                cout <<"Nr. of message " << rows.size() << endl;
                net.send(rows.size());
                for(auto it=rows.begin();it!=rows.end();++it){
                    
                    string authorID = (*it)["authorID"];
                    string author = DBfindOneValue("Users",{
                        {"ID",authorID}},"username");
                        cout << author << endl;
                        cout << (*it)["msg"] << endl;
                        cout << (*it)["date"] << endl;
                    net.send(author);
                    net.send((*it)["msg"]);
                    net.send((*it)["date"]);
                }
            }
        }
    }

}

void leaveChat(Net net, MYSQL* conn, int &logedUserID){
    int chatID;
    net.recv(chatID);

    int status = 0;
    string groupID;

    if(!logedUserID)
        status= 2;
    else
    if(!DBexists("ChatGroups",{{"ID",to_string(chatID)}})){
        status = 3;
    }else{
        groupID = DBfindOneValue("ChatGroups",{
            {"ID",to_string(chatID)}},"groupID","0");
        if(!DBexists("Groups",{
            {"groupID",groupID},
            {"userID",to_string(logedUserID)}})){
            status = 3;
        }
    }

    if(status == 0){
        DBdelete("Groups",{
            {"groupID",groupID},
            {"userID",to_string(logedUserID)}});
    }

    net.send(status);
}

void addPersonToChat(Net net, MYSQL* conn, int &logedUserID){
    int chatID;
    net.recv(chatID);
    string newUser;
    net.recv(newUser);

    int status = 0;
    string groupID;

    if(!logedUserID)
        status= 2;
    else
    if(!DBexists("ChatGroups",{{"ID",to_string(chatID)}})){
        status = 3;
    }
    if(!DBexists("Users",{{"username",newUser}}))
        status = 5;
    else{
        groupID = DBfindOneValue("ChatGroups",{
            {"ID",to_string(chatID)}},"groupID","0");
        if(!DBexists("Groups",{
            {"groupID",groupID},
            {"userID",to_string(logedUserID)}})){
            status = 3;
        }
    }

    if(status == 0){
        int newUserID = DBfindID("Users",
            {{"username",newUser}});
        DBadd("Groups",{
            {"groupID",groupID},
            {"userID",to_string(newUserID)}});
    }

    net.send(status);
}

void changeChatName(Net net, MYSQL* conn, int &logedUserID){
    string newChatName;
    int chatID;
    net.recv(chatID);
    net.recv(newChatName);

    int status = 0;

    if(!logedUserID)
        status= 2;
    else
    if(!DBexists("ChatGroups",{{"ID",to_string(chatID)}})){
        status = 3;
    }
    else{
        string groupID = DBfindOneValue("ChatGroups",{
            {"ID",to_string(chatID)}},"groupID","0");
        if(!DBexists("Groups",{
            {"groupID",groupID},
            {"userID",to_string(logedUserID)}})){
            status = 3;
        }
    }

    if(status == 0){
        DBupdate("ChatGroups",
            {{"ID",to_string(chatID)}},
            {{"name",newChatName}});
    }

    net.send(status);
}

void listChatGroups(Net net, MYSQL* conn, int &logedUserID){
    
    int status = 0;
    vector<pair<int,string>> chatGroups = {};

    if(!logedUserID)
        status = 2;
    else{
        vector<string> columns = {"groupID","name","ID"};
        DBrows chatRows = DBfind("ChatGroups",{},columns);
        for(auto chatRow : chatRows){
            if(DBexists("Groups",{
                {"groupID",chatRow["groupID"]},
                {"userID",to_string(logedUserID)}}))
                cout << chatRow["name"] << endl;
                chatGroups.push_back({atoi(chatRow["ID"].c_str()),chatRow["name"]});
        }
        if(chatGroups.empty())
            status = 3;
    }

    net.send(status);

    if(status == 0){
        net.send(chatGroups.size());
        for(auto chatGroup : chatGroups){
            cout << chatGroup.second << endl;
            net.send(chatGroup.first);
            net.send(chatGroup.second);
        }
    }
}