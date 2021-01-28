void post(Net net, bool &logedIn){
    net.send("post");
    string privacy;
    cout << "Type the privacy level (private,public,friends or a friend category):\n"<<endl;
    getline(cin,privacy);
    net.send(privacy);
    string post = "";
    cout << "Type the text you want to post and when you're done press done:\n"<<endl;
    bool done = false;
    while(!done){
        string line;
        getline(cin,line);
        if(line == "done")
            done = true;
        else
            post+=line+"\n";
    }
    post.pop_back();
    net.send(post);

    int status;
    net.recv(status);

    if(!net.connLost)
        switch (status)
        {
            case 0:
                cout << "Posted succesfuly!\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You're not loged in!\n";
                break;
        }
}

void deletePost(Net net, bool &logedIn){
    net.send("deletePost");
    int postID;
    cout << "Type the ID of the post you want to delete:\n"<<endl;
    cin >> postID;
    net.send(postID);

    int status;
    net.recv(status);

    if(!net.connLost)
        switch (status)
        {
            case 0:
                cout << "Post deleted succesfuly!\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You're not loged in!\n";
                break;
            case 3:
                cout << "You don't have a post with this ID!!\n";
                break;
        }
}

void editPost(Net net, bool &logedIn){
    net.send("editPost");
    int postID;
    cout << "Type the ID of the post you want to edit:\n"<<endl;
    cin >> postID;
    net.send(postID);
    string post = "";
    cout << "Type the new text for the post and when you're done type done\n"<<endl;
    bool done = false;
    while(!done){
        string line;
        getline(cin,line);
        if(line == "done")
            done = true;
        else
            post+=line+"\n";
    }
    post.pop_back();
    net.send(post);

    int status;
    net.recv(status);

    if(!net.connLost)
        switch (status)
        {
            case 0:
                cout << "Post edited succesfuly!\n";
                break;
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You're not loged in!\n";
                break;
            case 3:
                cout << "You don't have a post with this ID!\n";
                break;
        }
}

void seePosts(Net net, bool &logedIn){
    net.send("seePosts");
    string name;
    cout << "Type the name:\n"<<endl;
    getline(cin,name);
    net.send(name);

    int status;
    net.recv(status);

    if(!net.connLost)
        switch (status)
        {
            case 0:{
                cout << "This is the list of posts:\n";
                int nrOfPosts = 0;
                net.recv(nrOfPosts);
                for(int i=0;i<nrOfPosts;++i){
                    int postID,editedID,originalID;
                    string post,date;
                    
                    net.recv(postID);
                    net.recv(post);
                    net.recv(date);
                    net.recv(editedID);
                    net.recv(originalID);
                    
                    cout << "-------\n";
                    cout << "post with ID " << postID << endl;
                    cout << post << endl;
                    if(originalID)
                        cout << "edited at " << date << endl;
                    else
                        cout << "posted at " << date << endl;
                    cout << "-------\n";
                }
                break;
            }
            case 1:
                cout << "Inexistent command!\n";
                break;
            case 2:
                cout << "You're not loged in!\n";
                break;
            case 3:
                cout << "No posts found from this user!\n";
                break;
        }
}