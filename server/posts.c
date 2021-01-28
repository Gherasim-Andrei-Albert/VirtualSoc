void post(Net net, MYSQL* conn, int &logedUserID){
    int statusCode = 0;
    Input postForm = net.recv({
        "privacy","text",
        "post","text"
    });
    int status = 0;
    cout<<"|"<<postForm["privacy"]<<"|"<<endl;
    if(!logedUserID)
        status = 2;
    else
        DBadd("Posts",{
            {"authorID",to_string(logedUserID)},
            {"post",postForm["post"]},
            {"privacy",postForm["privacy"]},
            {"date","NOW"}});
    
    net.send(status);
}

void seePosts(Net net, MYSQL* conn, int &logedUserID){
    int status = 0;
    string user;
    net.recv(user);

    if(!DBexists("Users",{{"username",user}}))
        status = 2;

    struct postData{
        int ID;
        string post;
        int editedPostID;
        int originalPostID;
        string date;
    };
    vector<postData> availablePosts = {};

    if(status == 0){
        int userID = DBfindID("Users",{{"username",user}});

        vector<string> columns = 
            {"privacy","post","editedPostID","originalPostID","date","ID"};
        DBrows posts = DBfind("Posts",{
            {"authorID",to_string(userID)},
            {"editedPostID","NULL"}
        },columns);
        for(auto post : posts){

            if(post["privacy"]=="public" || logedUserID==userID){
                availablePosts.push_back({
                    atoi(post["ID"].c_str()),
                    post["post"],
                    post["editedPostID"] == "NULL" ? 0 : atoi(post["editedPostID"].c_str()),
                    post["originalPostID"] == "NULL" ? 0 : atoi(post["originalPostID"].c_str()),
                    post["date"]});
            }else
            if(post["privacy"]=="friends"){ 
                if(logedUserID)
                    if(DBexists("Friends",{{"userID",to_string(userID)}})){

                        vector<string> rows = DBfind("Friends",{
                            {"userID",to_string(userID)}},"groupID");
                        
                        for(auto friendsIt = rows.begin();friendsIt!=rows.end();++friendsIt){

                            string groupID = *friendsIt;
                            cout<<"GroupID: "<<groupID<<endl;
                            cout<<"MyID: "<<logedUserID<<endl;
                            if(DBexists("Groups",{
                                {"groupID",groupID},
                                {"userID",to_string(logedUserID)}})){
                                cout<<"Should push "<< post["post"]<<endl;
                                
                                availablePosts.push_back({
                                    atoi(post["ID"].c_str()),
                                    post["post"],
                                    post["editedPostID"] == "NULL" ? 0 : atoi(post["editedPostID"].c_str()),
                                    post["originalPostID"] == "NULL" ? 0 : atoi(post["originalPostID"].c_str()),
                                    post["date"]});
                            }
                        }  
                    }
            }else
            if(post["privacy"] != "private"){
                if(logedUserID)
                    if(DBexists("Friends",{
                        {"userID",to_string(userID)},
                        {"friendType",post["privacy"]}})) {

                        string groupID = DBfindOneValue("Friends",{
                            {"userID",to_string(userID)},
                            {"friendType",post["privacy"]}
                        },"groupID","");
                        
                        if(DBexists("Groups",{
                            {"groupID",groupID},
                            {"userID",to_string(logedUserID)}}))
                                    
                            availablePosts.push_back({
                                atoi(post["ID"].c_str()),
                                post["post"],
                                post["editedPostID"] == "NULL" ? 0 : atoi(post["editedPostID"].c_str()),
                                post["originalPostID"] == "NULL" ? 0 : atoi(post["originalPostID"].c_str()),
                                post["date"]});
                    }
            }
        }
        if(availablePosts.empty()){
            status = 3;
        }
    }

    net.send(status);

    if(status == 0){
        net.send(availablePosts.size());
        for(auto post : availablePosts){

            net.send(post.ID);
            net.send(post.post);
            net.send(post.date);
            net.send(post.editedPostID);
            net.send(post.originalPostID);
        }
    }
}

void deletePost(Net net, MYSQL* conn, int &logedUserID){
    int postID;
    net.recv(postID);

    int status = 0;

    if(!logedUserID)
        status = 2;
    else{
        if(!DBexists("Posts",{
            {"ID",to_string(postID)},
            {"authorID",to_string(logedUserID)}
        }))
            status = 3;
    }

    if(status == 0){
        DBdelete("Posts",{
            {"ID",to_string(postID)},
            {"authorID",to_string(logedUserID)}
        });
    }

    net.send(status);
}

void editPost(Net net, MYSQL* conn, int &logedUserID){
    int postID;
    net.recv(postID);
    string newPost;
    net.recv(newPost);

    int status = 0;

    if(!logedUserID)
        status = 2;
    else
    if(!DBexists("Posts",{
        {"ID",to_string(postID)},
        {"authorID",to_string(logedUserID)}
    }))
        status = 3;
    

    if(status == 0){
        DBadd("Posts",{
            {"authorID",to_string(logedUserID)},
            {"post",newPost},
            {"originalPostID",to_string(postID)},
            {"date","NOW"}});
        string newPostID;
        newPostID = DBfindOneValue("Posts",{
            {"authorID",to_string(logedUserID)},
            {"post",newPost},
            {"originalPostID",to_string(postID)}
        },"ID");
        DBupdate("Posts",{
            {"ID",to_string(postID)},
            {"authorID",to_string(logedUserID)}
        },{{"editedPostID",newPostID}});
    }

    net.send(status);
}

/*
reserved keywords
*/