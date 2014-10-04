#ifndef NETWORK_H
#define NETWORK_H

#include "includes.h"

const int MAXLEN = 1024;

class Client
{
public:
    Client(TCPsocket s=NULL, std::string n="", bool a=false)
        : sock(s), name(n), active(a), player_num(-1)
    {}

    TCPsocket get_socket() { return sock; }
    std::string get_name() { return name; }
    bool get_active() { return active; }
    int get_player_num() { return player_num; }
    
private:
    TCPsocket sock;
    std::string name;
    bool active;
    int player_num;
};

class Server
{
public:
private:
};

#endif

