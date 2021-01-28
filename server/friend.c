using namespace std;

bool areFriends(int ID1, int ID2){
    cout << "before are friends check"<<endl;
    vector<string> friendGroupsID = 
        DBfind("Friends",{
            {"userID",to_string(ID1)}
        },"groupID");
    for(auto friendGroupID : friendGroupsID){
        if(DBexists("Groups",{
            {"groupID",friendGroupID},
            {"userID",to_string(ID2)}}))
            return true;
    }
    return false;
}

bool hasFriendType(int userID, int friendID, string type){
    cout << "befor has type"<<endl;
    string friendGroupID = 
        DBfindOneValue("Friends",{
            {"userID",to_string(userID)},
            {"friendType",type}
        },"groupID");
    
    if(friendGroupID == "")
        return false;

    if(DBexists("Groups",{
        {"groupID",friendGroupID},
        {"userID",to_string(friendID)}}))
        return true;
    
    return false;
}

void reqTypeFriend(Net net, MYSQL* conn, int &logedUserID){
    string name;
    net.recv(name);
    string type;
    net.recv(type);
    int statusCode = 0; 
    int friendID;
    if(!logedUserID){
        statusCode = 2;
        cout<<"not loged\n";
    }
    else{
        cout<<"before check user"<<endl;
        if(!DBexists("Users",{{"username",name}})){
            statusCode = 3;
            cout<<"after check user\n";
        }
        else{
            cout<< "before check self req\n";
            if(logedUserID == DBfindID("Users",{{"username",name}}))
                {statusCode = 4;}
            else{
                cout<<"before friend id"<<endl;
                friendID = DBfindID("Users",{{"username",name}});
                cout<<"before check req exists"<<endl;
                if(hasFriendType(logedUserID,friendID,type))
                    statusCode = 7;
                else
                if(areFriends(logedUserID,friendID))
                    statusCode = 8;
                else
                if(DBexists("Requests",{{"userID",to_string(logedUserID)},{"friendID",to_string(friendID)}}))
                    statusCode = 5;
                else
                if(DBexists("Requests",{{"userID",to_string(friendID)},{"friendID",to_string(logedUserID)}}))
                    statusCode = 6;
            }
        }
    }
    cout<<"before add\n";

    if(statusCode == 0){ 
        cout<<"should add"<<endl;
        DBadd("Requests",{
            {"userID",to_string(logedUserID)},
            {"friendID",to_string(friendID)},
            {"friendType",type}});
    }
    
    net.send(statusCode);
}

void reqFriend(Net net, MYSQL* conn, int &logedUserID){
    string name;
    net.recv(name);
    int statusCode = 0; 
    int friendID;
    if(!logedUserID){
        statusCode = 2;
        cout<<"not loged\n";
        }
    else{
        cout<<"before check user"<<endl;
        if(!DBexists("Users",{{"username",name}})){
            statusCode = 3;
            cout<<"after check user\n";
        }
        else{
            cout<< "before check self req\n";
            if(logedUserID == DBfindID("Users",{{"username",name}}))
                {statusCode = 4;}
            else{
                cout<<"before friend id"<<endl;
                friendID = DBfindID("Users",{{"username",name}});
                cout<<"before check req exists"<<endl;
                if(areFriends(logedUserID,friendID))
                    statusCode = 7;
                if(DBexists("Requests",{{"userID",to_string(logedUserID)},{"friendID",to_string(friendID)}}))
                    {statusCode = 5;}
                else
                if(DBexists("Requests",{{"userID",to_string(friendID)},{"friendID",to_string(logedUserID)}}))
                    statusCode = 6;
            }
        }
    }
    cout<<"before add\n";

    if(statusCode == 0){ 
        cout<<"should add"<<endl;
        DBadd("Requests",{
            {"userID",to_string(logedUserID)},
            {"friendID",to_string(friendID)}});
    }
    
    net.send(statusCode);
}

void accFriend(Net net, MYSQL* conn, int &logedUserID){
    string name;
    net.recv(name);
    int statusCode = 0;
    int friendID;
    if(!logedUserID){
        statusCode = 2;
        cout<<"not loged\n";
    }
    else{
        
        cout<<"before check user"<<endl;
        if(!DBexists("Users",{{"username",name}})){
            statusCode = 3;
            cout<<"after check user\n";
        }
        else{
            cout<< "before check self req\n";
            if(logedUserID == DBfindID("Users",{{"username",name}}))
                statusCode = 4;
            else{
                cout<<"before friend id"<<endl;
                friendID = DBfindID("Users",{{"username",name}});
                cout<<"before check req exists"<<endl;
                if(areFriends(logedUserID,friendID))
                    statusCode = 6;
                else
                if(!DBexists("Requests",{
                    {"friendID",to_string(logedUserID)},
                    {"userID",to_string(friendID)}})){
                    statusCode = 5;
                }
            }
        }
    }
    cout<<"before add\n";

    if(statusCode == 0){ cout<<"should add"<<endl;
        
        string typeWantedByFriend = DBfindOneValue("Requests",{
            {"friendID",to_string(logedUserID)},
            {"userID",to_string(friendID)}
        },"friendType","");
        string query = "DELETE FROM Requests WHERE friendID="+to_string(logedUserID)+" AND userID="+to_string(friendID);
        if(mysql_query(conn, query.c_str())){
            fprintf(stderr,"%s\n",mysql_error(conn));
        }


        int friendsID = DBfindID("Friends",{
            {"userID",to_string(logedUserID)},
            {"friendType","NULL"}
        });
        int groupID;    
        if(!friendsID){
            groupID = DBnextValue("Groups","groupID");
        }else{
            groupID = atoi(DBfindOneValue("Friends",{
                {"userID",to_string(logedUserID)},
                {"friendType","NULL"}
            },"groupID","0").c_str());
        }
        DBadd("Groups",{
            {"groupID",to_string(groupID)},
            {"userID",to_string(friendID)}});
        if(!friendsID){
            DBadd("Friends",{
                {"userID",to_string(logedUserID)},
                {"groupID",to_string(groupID)}});
        }

        int friendsIDofFriend = DBfindID("Friends",{
            {"userID",to_string(friendID)},
            {"friendType",typeWantedByFriend}
        });
        int groupIDofFriend;    
        if(!friendsIDofFriend){
            groupIDofFriend = DBnextValue("Groups","groupID");
        }else{
            groupIDofFriend = atoi(DBfindOneValue("Friends",{
                {"userID",to_string(friendID)},
                {"friendType",typeWantedByFriend}
            },"groupID","0").c_str());
        }
        DBadd("Groups",{
            {"groupID",to_string(groupIDofFriend)},
            {"userID",to_string(logedUserID)}});
        if(!friendsIDofFriend){
            DBadd("Friends",{
                {"userID",to_string(friendID)},
                {"groupID",to_string(groupIDofFriend)},
                {"friendType",typeWantedByFriend}});
        }
    }

    net.send(statusCode);
}

void accTypeFriend(Net net, MYSQL* conn, int &logedUserID){
    string name;
    net.recv(name);
    string type;
    net.recv(type);
    int statusCode = 0;
    int friendID;
    if(!logedUserID){
        statusCode = 2;
        cout<<"not loged\n";
    }
    else{

        cout<<"before check user"<<endl;
        if(!DBexists("Users",{{"username",name}})){
            statusCode = 3;
            cout<<"after check user\n";
        }
        else{
            cout<< "before check self req\n";
            if(logedUserID == DBfindID("Users",{{"username",name}}))
                {statusCode = 4;}
            else{
                cout<<"before friend id"<<endl;
                friendID = DBfindID("Users",{{"username",name}});
                cout<<"before check req exists"<<endl;
                if(hasFriendType(logedUserID,friendID,type))
                    statusCode = 6;
                else
                if(areFriends(logedUserID,friendID))
                    statusCode = 7;
                else
                if(!DBexists("Requests",{
                    {"friendID",to_string(logedUserID)},
                    {"userID",to_string(friendID)}})){
                    statusCode = 5;
                }
            }
        }
    }
    cout<<"before add\n";

    if(statusCode == 0){ cout<<"should add"<<endl;
        
        string typeWantedByFriend = DBfindOneValue("Requests",{
            {"friendID",to_string(logedUserID)},
            {"userID",to_string(friendID)}
        },"friendType","");
        string query = "DELETE FROM Requests WHERE friendID="+to_string(logedUserID)+" AND userID="+to_string(friendID);
        if(mysql_query(conn, query.c_str())){
            fprintf(stderr,"%s\n",mysql_error(conn));
        }


        int friendsID = DBfindID("Friends",{
            {"userID",to_string(logedUserID)},
            {"friendType",type}
        });
        int groupID;    
        if(!friendsID){
            groupID = max(DBnextValue("Groups","groupID"),
                DBnextValue("Friends","groupID"));
        }else{
            groupID = atoi(DBfindOneValue("Friends",{
                {"userID",to_string(logedUserID)},
                {"friendType",type}
            },"groupID","0").c_str());
        }
        DBadd("Groups",{
            {"groupID",to_string(groupID)},
            {"userID",to_string(friendID)}});
        if(!friendsID){
            DBadd("Friends",{
                {"userID",to_string(logedUserID)},
                {"groupID",to_string(groupID)},
                {"friendType",type}});
        }

        int friendsIDofFriend = DBfindID("Friends",{
            {"userID",to_string(friendID)},
            {"friendType",type}
        });
        int groupIDofFriend;    
        if(!friendsIDofFriend){
            groupIDofFriend = max(DBnextValue("Groups","groupID"),
                DBnextValue("Friends","groupID"));
        }else{
            groupIDofFriend = atoi(DBfindOneValue("Friends",{
                {"userID",to_string(friendID)},
                {"friendType",typeWantedByFriend}
            },"groupID","0").c_str());
        }
        DBadd("Groups",{
            {"groupID",to_string(groupIDofFriend)},
            {"userID",to_string(logedUserID)}});
        if(!friendsIDofFriend){
            DBadd("Friends",{
                {"userID",to_string(friendID)},
                {"groupID",to_string(groupIDofFriend)},
                {"friendType",typeWantedByFriend}});
        }
    }
    
    net.send(statusCode);
}

void changeFriendType(Net net, MYSQL* conn, int &logedUserID){
    string name,newType;
    net.recv(name);
    net.recv(newType);

    int status = 0;

    string groupRowID;
    int friendID;

    if(!logedUserID)
        status = 2;
    else
    if(!DBexists("Users",{{"username",name}}))
        status = 3;
    else{
        friendID = DBfindID("Users",{{"username",name}});
        cout << "before groupsIDs search\n";
        vector<string> groups = DBfind("Friends",{
            {"userID",to_string(logedUserID)}
        },"groupID");
        cout << "groupsIDs found\n";
        bool isFriend = false;
        for(auto groupIt = groups.begin();
            groupIt != groups.end(); ++groupIt){
            string groupID = *groupIt;
            cout << "before usersIDs search\n";
            vector<string> columns = {"userID","ID"};
            DBrows users = DBfind("Groups",{
                {"groupID",groupID}
            },columns);
            cout << "before usersIDs found\n";
            for(auto userIt = users.begin();
                userIt != users.end(); ++userIt){
                int userID = atoi((*userIt)["userID"].c_str());
                if(userID == friendID){
                    groupRowID = (*userIt)["ID"];
                    cout << "Storing the group rec ID: ["+groupRowID+"]\n";
                    isFriend = true;
                    break;
                }
            }
            if(isFriend)
                break;
        }
        if(logedUserID == friendID)
            status = 4;
        else
        if(!isFriend)
            status = 5;
    }

    if(status == 0){
        DBdelete("Groups",{{"ID",groupRowID}});
        int friendsID = DBfindID("Friends",{
            {"userID",to_string(logedUserID)},
            {"friendType",newType}
        });
        int groupID;    
        if(!friendsID){
            groupID = max(DBnextValue("Groups","groupID"),
                DBnextValue("Friends","groupID"));
        }else{
            groupID = atoi(DBfindOneValue("Friends",{
                {"userID",to_string(logedUserID)},
                {"friendType",newType}
            },"groupID","0").c_str());
        }
        DBadd("Groups",{
            {"groupID",to_string(groupID)},
            {"userID",to_string(friendID)}});
        if(!friendsID){
            DBadd("Friends",{
                {"userID",to_string(logedUserID)},
                {"groupID",to_string(groupID)},
                {"friendType",newType}});
        }
    }
    
    net.send(status);
}

void rmFriendType(Net net, MYSQL* conn, int &logedUserID){
    string name;
    net.recv(name);

    string type;
    net.recv(type);

    int status = 0;

    string groupRowID;
    string oldGroupID;
    int friendID;

    if(!logedUserID)
        status = 2;
    else
    if(!DBexists("Users",{{"username",name}}))
        status = 3;
    else{
        friendID = DBfindID("Users",{{"username",name}});
        cout << "before groupsIDs search\n";
        vector<string> groups = DBfind("Friends",{
            {"userID",to_string(logedUserID)}
        },"groupID");
        cout << "groupsIDs found\n";
        bool isFriend = false;
        for(auto groupIt = groups.begin();
            groupIt != groups.end(); ++groupIt){
            oldGroupID = *groupIt;
            cout << "before usersIDs search\n";
            vector<string> columns = {"userID","ID"};
            DBrows users = DBfind("Groups",{
                {"groupID",oldGroupID}
            },columns);
            cout << "before usersIDs found\n";
            for(auto userIt = users.begin();
                userIt != users.end(); ++userIt){
                int userID = atoi((*userIt)["userID"].c_str());
                if(userID == friendID){
                    groupRowID = (*userIt)["ID"];
                    cout << "Storing the group rec ID: ["+groupRowID+"]\n";
                    isFriend = true;
                    break;
                }
            }
            if(isFriend)
                break;
        }
        if(!isFriend)
            status = 5;
        else
        if(logedUserID == friendID)
            status = 4;
    }

    if(status == 0){
        DBdelete("Groups",{{"ID",groupRowID}});
        if(!DBexists("Groups",{{"groupID",oldGroupID}}))
            DBdelete("Friends",{
                {"userID",to_string(logedUserID)},
                {"friendType",type}});
        int friendsID = DBfindID("Friends",{
            {"userID",to_string(logedUserID)},
            {"friendType","NULL"}
        });
        int groupID;    
        if(!friendsID){
            groupID = max(DBnextValue("Groups","groupID"),
                DBnextValue("Friends","groupID"));
        }else{
            groupID = atoi(DBfindOneValue("Friends",{
                {"userID",to_string(logedUserID)},
                {"friendType","NULL"}
            },"groupID","0").c_str());
        }
        DBadd("Groups",{
            {"groupID",to_string(groupID)},
            {"userID",to_string(friendID)}});
        if(!friendsID){
            DBadd("Friends",{
                {"userID",to_string(logedUserID)},
                {"groupID",to_string(groupID)}});
        }
    }

    net.send(status);
}

void rmFriend(Net net, MYSQL* conn, int &logedUserID){
    string name;
    net.recv(name);
    int status = 0;

    string groupRowID;
    int friendID;

    if(!logedUserID)
        status = 2;
    else
    if(!DBexists("Users",{{"username",name}}))
        status = 3;
    else{
        friendID = DBfindID("Users",{{"username",name}});
        cout << "before groupsIDs search\n";
        vector<string> groups = DBfind("Friends",{
            {"userID",to_string(logedUserID)}
        },"groupID");
        cout << "groupsIDs found\n";
        bool isFriend = false;
        for(auto groupIt = groups.begin();
            groupIt != groups.end(); ++groupIt){
            string groupID = *groupIt;
            cout << "before usersIDs search\n";
            vector<string> columns = {"userID","ID"};
            DBrows users = DBfind("Groups",{
                {"groupID",groupID}
            },columns);
            cout << "before usersIDs found\n";
            for(auto userIt = users.begin();
                userIt != users.end(); ++userIt){
                int userID = atoi((*userIt)["userID"].c_str());
                if(userID == friendID){
                    groupRowID = (*userIt)["ID"];
                    cout << "Storing the group rec ID: ["+groupRowID+"]\n";
                    isFriend = true;
                    break;
                }
            }
            if(isFriend)
                break;
        }
        if(!isFriend)
            status = 5;
        else
        if(logedUserID == friendID)
            status = 4;
    }

    if(status == 0){
        DBdelete("Groups",{{"ID",groupRowID}});
    }
    
    net.send(status);
}

void listFriends(Net net, MYSQL* conn, int &logedUserID){
    int status = 0;
    vector<string> friends = {};

    if(!logedUserID)
        status = 2;
    else{
        vector<string> friendGroupsID = DBfind("Friends",
            {{"userID",to_string(logedUserID)}}, "groupID");

        for(auto groupID : friendGroupsID){
            vector<string> friendsIDInGroup = DBfind("Groups",{
                {"groupID",groupID}
            },"userID");
            for(auto friendID : friendsIDInGroup){
                string friendName = DBfindOneValue("Users",
                    {{"ID",friendID}},"username");
                friends.push_back(friendName);
            }
        }

        if(friends.empty())
            status = 3;
    }

    net.send(status);

    if(status == 0){
        net.send(friends.size());
        for(string friendName : friends)
            net.send(friendName);
    }
}


void listTypeFriends(Net net, MYSQL* conn, int &logedUserID){
    string type;
    net.recv(type);

    int status = 0;
    vector<string> friends = {};

    if(!logedUserID)
        status = 2;
    else{
        vector<string> friendGroupsID = DBfind("Friends",{
            {"userID",to_string(logedUserID)},
            {"friendType",type}
        }, "groupID");

        for(auto groupID : friendGroupsID){
            vector<string> friendsIDInGroup = DBfind("Groups",{
                {"groupID",groupID}
            },"userID");
            for(auto friendID : friendsIDInGroup){
                string friendName = DBfindOneValue("Users",
                    {{"ID",friendID}},"username");
                friends.push_back(friendName);
            }
        }

        if(friends.empty())
            status = 3;
    }

    net.send(status);

    if(status == 0){
        net.send(friends.size());
        for(string friendName : friends)
            net.send(friendName);
    }
}

void listFriendTypes(Net net, MYSQL* conn, int &logedUserID){

    int status = 0;
    vector<string> types = {};

    if(!logedUserID)
        status = 2;
    else{
        types = DBfind("Friends",{
            {"userID",to_string(logedUserID)}}, "friendType");

        if(types.empty())
            status = 3;
    }

    net.send(status);

    if(status == 0){
        net.send(types.size());
        for(string type : types)
            net.send(type);
    }
}

void rmAllFriends(Net net, MYSQL* conn, int &logedUserID){
    int status = 0;
    vector<string> types = {};

    if(!logedUserID)
        status = 2;
    else{
        vector<string> groupsID = DBfind("Friends",{
            {"userID",to_string(logedUserID)}},"groupID");
        for(auto groupID : groupsID){
            DBdelete("Groups",{{"groupID",groupID},{"userID",to_string(logedUserID)}});
        }
    }
    
    net.send(status);
}

void rmAllTypeFriends(Net net, MYSQL* conn, int &logedUserID){
    string type; 
    net.recv(type);
     
    int status = 0;
    vector<string> types = {};

    if(!logedUserID)
        status = 2;
    else{

        vector<string> columns = {"groupID","ID"};
        DBrows friendRows = DBfind("Friends",{
            {"userID",to_string(logedUserID)},
            {"friendType",type}    
        },columns);
        for(auto friendRow : friendRows){
            DBdelete("Groups",{{"groupID",friendRow["groupID"]}});
            DBdelete("Friends",{{"ID",friendRow["ID"]}});
        }
    }
    
    net.send(status);
}

void listFriendRequests(Net net, MYSQL* conn, int &logedUserID){
    int status = 0;
    vector<string> requests = {};
    if(!logedUserID)
        status = 2;
    else{
        string query = "SELECT username from Users WHERE ID IN(SELECT userID FROM Requests WHERE friendID="+to_string(logedUserID)+")";
        mysql_query(conn,query.c_str());
        MYSQL_RES* result = mysql_store_result(conn);
        while(MYSQL_ROW row = mysql_fetch_row(result)){
            requests.push_back(row[0]);
        }
        /* int reqSize = requests.size();
        write(cliSock,&reqSize,sizeof(int));
        for(int i=0;i<requests.size();++i){
            sendOutput(cliSock, requests[i]);
        } */
        if(requests.empty())
            status = 3;
    }

    net.send(status);

    if(status == 0){
        net.send(requests.size());
        for(auto req: requests)
            net.send(req);
    }
}

/*TO-DO
logedIn
self-req
friend already added
friend already sent request
user already sent req
change friend type
remove friend 
request with type and without type
*/
/*REQUIRED
user exists
*/