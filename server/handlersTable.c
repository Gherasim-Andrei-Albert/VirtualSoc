#include <my_global.h>
#include <mysql.h>
#include <map>
#include <string>
using namespace std;

typedef void (*requestHandler)(Net, MYSQL*, int&);
map<string, requestHandler> handlersTable = {
    {"signUp",&signUp},
    {"logIn",&logIn},
    {"logOut",&logOut},

    {"listUsers",&listUsers},
    {"listAdmins",&listAdmins},
    {"deleteUser",&deleteUser},

    {"seeProf",&seeProf},
    {"editProf",&editProf},
    {"setPrivacy",&setPrivacy},

    {"reqFriend",&reqFriend},
    {"reqTypeFriend",&reqTypeFriend},
    {"accFriend",&accFriend},
    {"accTypeFriend",&accTypeFriend},
    {"rmFriend",&rmFriend},
    {"rmFriendType",&rmFriendType},
    {"rmAllFriends",&rmAllFriends},
    {"rmAllTypeFriends",&rmAllTypeFriends},
    {"changeFriendType",&changeFriendType},
    {"listFriends",&listFriends},
    {"listTypeFriends",&listTypeFriends},
    {"listFriendTypes",&listFriendTypes},
    {"listFriendRequests",&listFriendRequests},

    {"mkChatGr",&mkChatGr},
    {"leaveChat",&leaveChat},
    {"addPersonToChat",&addPersonToChat},
    {"changeChatName",&changeChatName},
    {"listChatGroups",&listChatGroups},
    {"sendMsg",&sendMsg},
    {"seeChat",&seeChat},

    {"post",&post},
    {"deletePost",&deletePost},
    {"editPost",&editPost},
    {"seePosts",&seePosts}
};