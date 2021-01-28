#include <algorithm>
using namespace std;

void seeProf(Net net, MYSQL* conn, int &logedUserID){
cout<<"seeProf\n";
    Input seeProfForm = 
        net.recv({"username","text"});

    int statusCode = 0;

    int wantedUserID = 
        DBfindID("Users",{{"username",seeProfForm["username"]}});

    if(!wantedUserID)
        statusCode = 5;
    else{
        if(wantedUserID!=logedUserID){
            string privacy = DBfindOneValue("Users",
                {{"username",seeProfForm["username"]}},
                "privacy","");
            
            if(privacy!="public"){
                if(privacy == ""){
                    statusCode = 4;
                }else
                if(privacy == "private"){
                    if(!logedUserID)
                        statusCode = 2;
                    else
                        statusCode = 3;   
                }else
                if(privacy=="friends"){
                    if(!logedUserID)
                        statusCode = 2;
                    else{
                        if(DBexists("Friends",{{"userID",to_string(wantedUserID)}})){
                            vector<string> rows = DBfind("Friends",{
                                {"userID",to_string(wantedUserID)}},"groupID");
                            
                            statusCode = 3;
                            cout<<"WantedID "<<wantedUserID<<endl;
                            for(auto it = rows.begin();it!=rows.end();++it){

                                string groupID = *it;
                                cout<<"GroupID: "<<groupID<<endl;
                                if(DBexists("Groups",{
                                    {"groupID",groupID},
                                    {"userID",to_string(logedUserID)}})){

                                    statusCode = 0;
                                    break;
                                }
                            }
                        }else{
                            statusCode = 3;
                        }
                    }
                }else
                if(privacy=="group"){
                    if(!logedUserID)
                        statusCode = 2;
                    else{
                        string strGroupID = DBfindOneValue("Users",
                            {{"ID",to_string(wantedUserID)}},"profGroupID","0");
                        //cout<<"group "<<strGroupID<<endl;
                        //cout<<"userID "<<to_string(logedUserID)<<endl;
                        if(!DBexists("Groups",{{"groupID",strGroupID},{"userID",to_string(logedUserID)}}))
                            statusCode = 3;
                        else cout<<"exists\n";
                    }
                }
                else{
                    if(DBexists("Friends",{
                        {"userID",to_string(wantedUserID)},
                        {"friendType",privacy}})){

                        string groupID = DBfindOneValue("Friends",{
                            {"userID",to_string(wantedUserID)},
                            {"friendType",privacy}
                        },"groupID","");
                        
                        if(!DBexists("Groups",{
                            {"groupID",groupID},
                            {"userID",to_string(logedUserID)}}))
                            
                            statusCode=3;
                    }else{
                        statusCode=3;
                    }
                }
            }
        }
    }

    net.send(statusCode);

    if(statusCode==0){

        vector<string> columns = {"roleID","birthdate"};
        DBrow profile = DBfindOne("Users",
            {{"username", seeProfForm["username"]}},
            columns);
        net.send(seeProfForm["username"]);
        string role = DBfindOneValue("Roles",
            {{"ID",profile["roleID"]}},"name","");
        net.send(role);
        net.send(profile["birthdate"]);
        
    }
}

void setPrivacy(Net net, MYSQL* conn, int &logedUserID){
cout<<"setPrivacy\n";
    string privacy;
    net.recv(privacy);

    int statusCode = 0;

    if(!logedUserID)
        statusCode = 2;
    
    if(statusCode == 0){
        DBupdate("Users",
            {{"ID",to_string(logedUserID)}},{{"privacy",privacy}});
        if(privacy=="group"){
            //citeste membrii grupului
            int membersNr=0;
            net.recv(membersNr);
            vector<string> members = {};
            for(int i=0;i<membersNr;i++){ 
                string member;
                net.recv(member);
                members.push_back(member);
            }
            vector<int> membersID = {};
            for(auto it=members.begin();it!=members.end();++it){
                int memberID = DBfindID("Users",{{"username",*it}});
                if(!memberID){
                    statusCode = 7;
                    break;
                }
                membersID.push_back(memberID);    
            }
            if(statusCode!=7){
                string currentGoupID = DBfindOneValue("Users",
                    {{"ID",to_string(logedUserID)}},"profGroupID","0");
                int groupID;
                if(currentGoupID=="NULL"){
                    string query = "SELECT IFNULL(max(groupID),0)+1 FROM Groups";
                    mysql_query(conn,query.c_str());
                    groupID = atoi(mysql_fetch_row(mysql_store_result(conn))[0]);
                    DBupdate("Users",
                        {{"ID",to_string(logedUserID)}},
                        {{"profGroupID",to_string(groupID)}});
                }else{
                    groupID = atoi(currentGoupID.c_str());
                    mysql_query(conn,("DELETE FROM Groups WHERE groupID="+to_string(groupID)).c_str());
                }
                for(auto it=membersID.begin();it!=membersID.end();++it){ 
                    DBadd("Groups",
                        {{"groupID",to_string(groupID)},
                        {"userID",to_string(*it)}});
                }
            }
        }
    }

    net.send(statusCode);
}


void editProf(Net net, MYSQL* conn, int &logedUserID){
cout<<"editProf\n";
    int nrOfProps = 0;
    vector<string> props = {};
    vector<string> values = {};
    net.recv(nrOfProps);
    for(int i=0;i<nrOfProps;++i){
        string prop;
        net.recv(prop);
        string value;
        net.recv(value);
        props.push_back(prop);
        values.push_back(value);
    }
    int statusCode = 0;
    if(!logedUserID)
        statusCode = 3;
    else{
        vector<string> validProps = {
            "username","password"
        };
        for(int i=0;i<nrOfProps;++i){
            if(find(validProps.begin(),validProps.end(),props[i])
                == validProps.end()){
                statusCode = 2;
                break;
            }
        }
    }

    if(statusCode == 0){
        for(int i=0;i<nrOfProps;++i){
            if(props[i] == "username"){
                if(DBexists("Users",{{"username",values[i]}}))
                    statusCode = 4;
            }
        }
    }

    if(statusCode == 0){
        for(int i=0;i<nrOfProps;++i){
            if(props[i] == "username"){
                DBupdate("Users",{{"ID",to_string(logedUserID)}},{{"username",values[i]}});
            }
            if(props[i] == "password"){
                DBupdate("Users",{{"ID",to_string(logedUserID)}},{{"pass",values[i]}});
            }
        }
    }
    
    net.send(statusCode);
}


/*
reserved keywords
data validation
*/